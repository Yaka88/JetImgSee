// TIF_LZW.cpp: implementation of the CTIF_LZW class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: TIFF_LZW.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIFF��LZWѹ����ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "TIF_LZW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTIF_LZW::CTIF_LZW()
{
}

CTIF_LZW::~CTIF_LZW()
{

}

extern int iReturn ;
// ��������	: TIF��LZWѹ������
// ����		: LPBYTE pInBmp,ͼ���� LPTIFC_VAR lpTIFCVar, TIF�ṹWORD wWidthBytes���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:

int CTIF_LZW::iEncodeTIF_LZW(LPBYTE& pInBmp, LPTIFC_VAR lpTIFCVar, WORD wWidthBytes)
{
	//��LPBYTE& pInBmp��֤pInBmp��������static�����ʣ�����ÿ��������ģ��ʱ��pInBmp��ֵ��ָ�;
	// hPrefix�����ױ���hSuffix����β����
	HANDLE hTableNdx, hPrefix, hSuffix, hImage;
	LPWORD lpwTableNdx, lpwPrefix;
	LPBYTE lpbySuffix;
	LPSTR lpImage, lpImageBgn,lpImageEnd;// ���һ������
	DWORD dwi;
	WORD wLZW_CLEAR, wLZW_EOI, wPreTableNdx, wNowTableNdx, wTopTableNdx, wHashGap, wHashNdx, wPrefix, wi;
	BOOL bStart;
	BYTE bySuffix, byCurrentBits;

	hTableNdx = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE << 1));
	hPrefix = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE << 1));
	hSuffix = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE));
	hImage = GlobalAlloc(GHND,(DWORD)wWidthBytes);
	lpwTableNdx = (LPWORD)GlobalLock(hTableNdx);
	lpwPrefix = (LPWORD)GlobalLock(hPrefix);
	lpbySuffix = (LPBYTE)GlobalLock(hSuffix);
	lpImage = (LPSTR)GlobalLock(hImage);
	if((lpwTableNdx==NULL) || (lpwPrefix==NULL) || (lpbySuffix==NULL) || (lpImage == NULL))
	{
		AfxMessageBox("LZW���������ڴ����!");
		iReturn = ENLZW_ALLOCMEMERR;
		return iReturn;
	}
	lpImageBgn = lpImage;
	lpImageEnd = lpImage + lpTIFCVar->wLineBytes;
	bStart = TRUE;

	// ** Initialize the table

	byCurrentBits = 9; //����ѹ������ĳ��ȸ��ģ��ִ�9��ʼ����󲻳���12λ����
	// ** TIFF��LZW����ֻ����8λ�Ͻ��в�����
	wLZW_CLEAR = 256; // ����룻
	wLZW_EOI = 257;   // �����룻
	wNowTableNdx = 258; // ��һ��δ�����ֵ���Ŀ�����ַ��Ӵ���
	wTopTableNdx = (1 << byCurrentBits) - 1;
	for (wi=0; wi<MAX_HASH_SIZE; wi++)
		*(lpwTableNdx +wi) = 0xFFFF;

	// ** Write this code
	iTIF_LZW_WriteCode(wLZW_CLEAR,&byCurrentBits,lpTIFCVar);

	for(dwi=0; dwi<lpTIFCVar->dwRowsPerStrip; dwi++)
	{
		/*try{ m_SrcFile->Read(lpImageBgn,wWidthBytes);}
		catch(CFileException pe){
			pe.ReportError();
			m_SrcFile->Close();
			iReturn = ENLZW_READBMPERR;
			return iReturn;}*/
		memcpy(lpImageBgn,pInBmp,wWidthBytes);
		pInBmp -= wWidthBytes;

		lpImage = lpImageBgn;
		// ** ע�⣺�˴�16ΪӦ�ֿ�����������һ���ط���
		if(lpTIFCVar->wBits == 24 ) 
		{
			for(wi=0; wi<lpTIFCVar->wLineBytes; wi+=3)
			{
				SWAP(*lpImage,*(lpImage+2));
				lpImage += 3;
			}
		}
		if(lpTIFCVar->wBits == 16)
		{
			;
		}

		lpImage = lpImageBgn;

		if(bStart)
		{
			wPrefix = (WORD)(*lpImage++ & 0xFF);
			bStart = FALSE;
		}
		while(lpImage < lpImageEnd)
		{
			bySuffix = *lpImage++;
			// ** See if the string is in the table
			wHashNdx = wPrefix^((WORD)bySuffix << 4);//ʵ���Ͻ�bySuffix��wPrefixǰ��ϲ�,��������.
			wHashGap = (wHashNdx ? (MAX_HASH_SIZE - wHashNdx) : 1);
			while(TRUE)
			{
				if(*(lpwTableNdx + wHashNdx) == 0xFFFF)
					break;// ** The string is not in the table
				if((*(lpwPrefix + wHashNdx) == wPrefix) && (*(lpbySuffix + wHashNdx) == bySuffix))
					break;// ** String found

				// ** Second Hash
				if(wHashNdx < wHashGap)
					wHashNdx += MAX_HASH_SIZE;
				wHashNdx -= wHashGap;
			}

			if(*(lpwTableNdx + wHashNdx) != 0xFFFF) // ** The string is in the table
				wPrefix = *(lpwTableNdx + wHashNdx); 
			else // ** The string is not in the table;
			{
				iTIF_LZW_WriteCode(wPrefix,&byCurrentBits,lpTIFCVar);// ** Write this code
				wPreTableNdx = wNowTableNdx;
				if(wNowTableNdx < MAX_TABLE_SIZE)
				{
					*(lpwTableNdx + wHashNdx) = wNowTableNdx++;
					*(lpwPrefix + wHashNdx) = wPrefix;
					*(lpbySuffix + wHashNdx) = bySuffix;
				}
				if(wPreTableNdx == wTopTableNdx)
				{
					if(byCurrentBits < 12)
					{
						byCurrentBits++;
						wTopTableNdx = (1 << byCurrentBits) - 1;
						wTopTableNdx -= ((byCurrentBits == 12) ? 1:0);
					}
					else
					{
						// ** Write this code
						iTIF_LZW_WriteCode(wLZW_CLEAR, &byCurrentBits, lpTIFCVar);
						// ** Initialize the table again
						byCurrentBits =9;
						wLZW_CLEAR = 256;
						wLZW_EOI = 257;
						wNowTableNdx = 258;
						wTopTableNdx = (1 << byCurrentBits) - 1;
						for(wi=0; wi<MAX_HASH_SIZE; wi++)
							*(lpwTableNdx + wi) = 0xFFFF;
					}
				}
				wPrefix = (WORD)bySuffix;
			}
		} //end of while
		lpTIFCVar->dwRowCount++;//iRatio=(int)((100L*(LONG)lpTIFCVar->dwRowCount++)/(LONG)lpTIFCVar->dwDepth); vStatusProcess(iRatio);
	} //end of for
	
	// ** Write this code
	iTIF_LZW_WriteCode(wPrefix,&byCurrentBits,lpTIFCVar);
	iTIF_LZW_WriteCode(wLZW_EOI,&byCurrentBits,lpTIFCVar);
	if(lpTIFCVar->byLeftBits)
	{
		*lpTIFCVar->lpEndBuff++ = (BYTE)(lpTIFCVar->dwTempCode << (8-lpTIFCVar->byLeftBits));
		lpTIFCVar->wByteCount++;
	}

	lpTIFCVar->dwTempCode = 0UL;
	lpTIFCVar->byLeftBits = 0x00;
	GlobalUnlock(hTableNdx);
	GlobalUnlock(hPrefix);
	GlobalUnlock(hSuffix);
	GlobalUnlock(hImage);
	GlobalFree(hTableNdx);
	GlobalFree(hPrefix);
	GlobalFree(hSuffix);
	GlobalFree(hImage);
	return SUCCESS;
}


int CTIF_LZW::iTIF_LZW_WriteCode(WORD wCode, LPBYTE lpbyCurrentBits, LPTIFC_VAR lpTIFCVar)
{
	// ** put This Code
	lpTIFCVar->dwTempCode <<= (*lpbyCurrentBits);
	lpTIFCVar->dwTempCode |= (DWORD)wCode;
	lpTIFCVar->byLeftBits += (*lpbyCurrentBits);
	while(lpTIFCVar->byLeftBits >= 0x08)
	{
		// ** put this char
		*lpTIFCVar->lpEndBuff++ = (BYTE)(lpTIFCVar->dwTempCode >> (lpTIFCVar->byLeftBits - 0x08));
									
		lpTIFCVar->wByteCount ++;//�൱���Ѿ����ļ�д����һ���ַ�
		lpTIFCVar->byLeftBits -= 0x08;
	}
	return SUCCESS;
}
// ��������	: TIF��LZWѹ������
// ����		: CFile *m_SrcFile,ԭ�ļ� LPBYTE pOutTiff,ͼ���� LPTIFD_VAR lpTIFDVar, TIF�ṹWORD wWidthBytes���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:

int CTIF_LZW::iDecodeTIF_LZW(CFile *m_SrcFile, LPBYTE pOutTiff, LPTIFD_VAR lpTIFDVar, WORD wWidthBytes)
{
	HANDLE hPrefix, hSuffix, hStack, hImage;
	LPWORD lpwPrefix;
	LPBYTE lpbySuffix, lpbyStack, lpbyStackBgn;
	LPSTR lpImage, lpImageEnd, lpImageBgn;
	DWORD dwDataNdx,dwRowNum,dwData;//dwDataBgn
	WORD wCode, wOldCode,wLZW_CLEAR,wLZW_EOI,wNowTableNdx,wTopTableNdx,wTempNdx,wRowCnt,wi,wj;
	WORD wCodeMask[13] = {0x0000,0x0001,0x0003,0x0007,0x000F,0x001F,0x003F,0x007F,0x00FF,0x01FF,0x03FF,0x07FF,0x0FFF};
	BYTE byCurrentBits, byLeftBits, byFirstChar, byCode;

	hPrefix = GlobalAlloc(GHND,(DWORD)(MAX_TABLE_SIZE << 1));
	hSuffix = GlobalAlloc(GHND,(DWORD)(MAX_TABLE_SIZE));
	hStack = GlobalAlloc(GHND,(DWORD)(MAX_TABLE_SIZE));
	hImage = GlobalAlloc(GHND,(DWORD)wWidthBytes);
	lpwPrefix = (LPWORD)GlobalLock(hPrefix);
	lpbySuffix = (LPBYTE)GlobalLock(hSuffix);
	lpbyStack = (LPBYTE)GlobalLock(hStack);
	lpImage = (LPSTR)GlobalLock(hImage);
	if((lpbyStack==NULL) || (lpwPrefix==NULL) || (lpbySuffix==NULL) || (lpImage == NULL))
	{
		AfxMessageBox("LZW���������ڴ����!");
		iReturn = DELZW_ALLOCMEMERR;
		return iReturn;
	}

	lpbyStackBgn = lpbyStack;
	lpImageBgn = lpImage;
	lpImageEnd = lpImage + lpTIFDVar->wLineBytes;

	//dwDataBgn = (DWORD)m_DstFile->Seek(0L,CFile::current);
	dwRowNum = (lpTIFDVar->bVerMirror ? 0UL : (lpTIFDVar->dwDepth - 1UL));
	wRowCnt = 1;

	for(wi = 0;wi<lpTIFDVar->wOffsetsLen; wi++)//ѭ������Ϊ���ݴ�����
	{
		/*//add by yaka 20010604
		wRowCnt = lpTIFDVar->dwRowsPerStrip * wi + 1;
		if (lpTIFDVar->bVerMirror)
			dwRowNum = lpTIFDVar->dwRowsPerStrip * wi;
		else
			dwRowNum = lpTIFDVar->dwDepth - 1UL - lpTIFDVar->dwRowsPerStrip * wi;
		//add end
		*/
		vProcessPrePare(m_SrcFile,lpTIFDVar,wi);
		lpTIFDVar->bEOF = FALSE;
		// ** ����һ�����ݴ�������
		if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
			return iReturn;
		lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
		lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
		byCurrentBits = 9;
		wLZW_CLEAR = 256;
		wLZW_EOI = 257;
		wNowTableNdx = 258;
		wTopTableNdx = (1<<byCurrentBits) - 1;
		wCode = 0;
		wOldCode = 0xFFFF;
		byLeftBits = 0x00;

		// ** Get Next Code
		while(byLeftBits < byCurrentBits)
		{
			// ** Get next char
			byCode = *lpTIFDVar->lpBgnBuff++;
			if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
			{
				if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
					return iReturn;
				lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
				lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
			}
			dwData = ((dwData << 8) | (DWORD)byCode);
			byLeftBits += 0x08;
		}
		wCode = (WORD)(dwData >> (byLeftBits - byCurrentBits)) & wCodeMask[byCurrentBits];
		byLeftBits -= byCurrentBits;

		while(wCode != wLZW_EOI)
		{
			if(wCode == wLZW_CLEAR)//*************************************
			{
				// ** Initialize the table
				for(wj=0; wj<wLZW_CLEAR;wj++)
				{
					*(lpwPrefix + wj) = 0xFFFF;
					*(lpbySuffix + wj) = (BYTE)wj;
				}
				for(wj=wNowTableNdx;wj<MAX_TABLE_SIZE;wj++)
				{
					*(lpwPrefix +wj) = 0xFFFF;
					*(lpbySuffix + wj) = 0x00;
				}
				byCurrentBits = 9;
				wNowTableNdx = 258;
				wTopTableNdx = 511;
				wOldCode = 0xFFFF;
				// ** Get Next Code
				while(byLeftBits < byCurrentBits)
				{
					// ** Get Next Char
					byCode = *lpTIFDVar->lpBgnBuff++;
					if((lpTIFDVar->lpBgnBuff == lpTIFDVar ->lpEndBuff) && !lpTIFDVar->bEOF)
					{
						if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
							return iReturn;
						lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
						lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
					}
					dwData = ((dwData << 8) | (DWORD)byCode);
					byLeftBits += 0x08;
				}
				wCode = (WORD)(dwData >> (byLeftBits - byCurrentBits)) & wCodeMask[byCurrentBits];
				byLeftBits -= byCurrentBits;

				if(wCode != wLZW_EOI)
				{
					// ** lpbyStack[0] is not used
					lpbyStack++;
					// ** Put data into stack
					while((*(lpwPrefix + wCode)) != 0xFFFF)
					{
						*lpbyStack++ = *(lpbySuffix + wCode);
						wCode = *(lpwPrefix+wCode);
					}
					*lpbyStack = *(lpbySuffix + wCode);
					byFirstChar = *lpbyStack;
					// ** write data
					while(lpbyStack>lpbyStackBgn)
					{
						*lpImage++ = *lpbyStack--;
						if(lpImage == lpImageEnd)
						{
							if(lpTIFDVar->wNewBits == 24)
							{
								lpImage = lpImageBgn;
								for(wj=0;wj<lpTIFDVar->wLineBytes;wj+=3)
								{
									SWAP(*lpImage, *(lpImage+2));
									lpImage += 3;
								}
							}
							// **��ͼ���һ������,����ˮƽ��ת;
							if(lpTIFDVar->bHorMirror)
								m_utility.vSuperInvertLine(lpImageBgn,(WORD)lpTIFDVar->wLineBytes,lpTIFDVar->wNewBits);

							dwDataNdx = dwRowNum * (DWORD)wWidthBytes;
							//m_DstFile->Seek((LONG)(dwDataBgn + dwDataNdx),CFile::begin);
							//m_DstFile->Write(lpImageBgn,wWidthBytes);
							memcpy(pOutTiff+dwDataNdx,lpImageBgn,wWidthBytes);
							wRowCnt++;//iRatio = (int)((100L*(LONG)wRowCnt++)/(LONG)lpTIFDVar->dwDepth);
							//vStatusProcess(iRatio);
							lpImage = lpImageBgn;
							//add by yaka 20010606
							if (dwRowNum == 0)
								goto exitloop;
							//add end
							(lpTIFDVar->bVerMirror ? dwRowNum++ : dwRowNum--);
						}
					}//end while
				} //end if
			}// end if********************************************

			else //***************************************************
			{
				// ** lpbyStack[0] is not used
				lpbyStack++;
				if(wCode < wNowTableNdx)
				{
					// ** The string is in the table
					wTempNdx = wCode;
				}
				else
				{
					wTempNdx = wOldCode;
					*lpbyStack++ = byFirstChar;
				}
				// ** put data into stack
				while((*(lpwPrefix + wTempNdx)) != 0xFFFF)
				{
					*lpbyStack++ = *(lpbySuffix + wTempNdx);
					wTempNdx = *(lpwPrefix + wTempNdx);
				}
				*lpbyStack = *(lpbySuffix + wTempNdx);
				byFirstChar = *lpbyStack;

				// ** Add string to the table
				if(wNowTableNdx > MAX_TABLE_SIZE)  //add by yaka 20010606
				{
					AfxMessageBox("�����LZWѹ���ļ���");
						iReturn = DELZW_ERROR;
						return DELZW_ERROR;
				}			//add end
				*(lpwPrefix + wNowTableNdx) = wOldCode;
				*(lpbySuffix + wNowTableNdx) = byFirstChar;
				if( ++wNowTableNdx == wTopTableNdx && byCurrentBits < 12)
				{
					byCurrentBits++;
					wTopTableNdx = (1<<byCurrentBits) - 1;
				}
				// ** Write Data
				while(lpbyStack > lpbyStackBgn)
				{
					*lpImage++ = *lpbyStack--;
					if(lpImage == lpImageEnd)
					{
						if(lpTIFDVar->wNewBits == 24)
						{
							lpImage = lpImageBgn;
							for(wj=0; wj<lpTIFDVar->wLineBytes; wj+=3)
							{
								SWAP(*lpImage, *(lpImage+2));
								lpImage += 3;
							}
						}
						if(lpTIFDVar->wNewBits == 16)
						{
							;
						}
						// **��ͼ���һ������,����ˮƽ��ת;
						if(lpTIFDVar->bHorMirror)
							m_utility.vSuperInvertLine(lpImageBgn,(WORD)lpTIFDVar->wLineBytes,lpTIFDVar->wNewBits);

						dwDataNdx = dwRowNum * (DWORD)wWidthBytes;
						//m_DstFile->Seek((LONG)(dwDataBgn + dwDataNdx),CFile::begin);
						//m_DstFile->Write(lpImageBgn, wWidthBytes);
						memcpy(pOutTiff+dwDataNdx,lpImageBgn,wWidthBytes);
						wRowCnt++;//iRatio = (int)((100L*(LONG)wRowCnt++)/(LONG)lpTIFDVar->dwDepth);
						//vStatusProcess(iRatio);
						lpImage = lpImageBgn;
						//add by yaka 20010606
							if (dwRowNum == 0)
								goto exitloop;
							//add end
						(lpTIFDVar->bVerMirror ? dwRowNum++ : dwRowNum--);
					}
				} // end of while     
			}// end of else ***************************************************
			wOldCode = wCode;

			// ** Get next code
			while(byLeftBits < byCurrentBits)
			{
				// ** Get next char
				byCode = *lpTIFDVar->lpBgnBuff++;
				if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
				{
					if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
						return iReturn;
					lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
					lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
				}
				dwData = ((dwData << 8) | (DWORD)byCode);
				byLeftBits += 0x08;
			}
			wCode = (WORD)(dwData >> (byLeftBits - byCurrentBits)) & wCodeMask[byCurrentBits];
			byLeftBits -= byCurrentBits;
		} // **end of while****************************************
	} // **end of for********************************************************************
exitloop:
	GlobalUnlock(hPrefix);
	GlobalUnlock(hSuffix);
	GlobalUnlock(hStack);
	GlobalUnlock(hImage);
	GlobalFree(hPrefix);
	GlobalFree(hSuffix);
	GlobalFree(hStack);
	GlobalFree(hImage);

	return SUCCESS;
}



// ** ����������ǰ��׼��������
void CTIF_LZW::vProcessPrePare(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, WORD wi)
{
	LPDWORD lpdwTemp;
	LPWORD lpwTemp;
	LPSTR lpTemp;
	DWORD dwOffset, dwByteCnt;
	lpTemp = (char*)GlobalLock(lpTIFDVar->hStripOffsets);
	if((lpTIFDVar->wOffsetsType == DTN_LONG) || (lpTIFDVar->wOffsetsType == DTN_SLONG))
	{
		lpdwTemp = (LPDWORD)lpTemp;
		dwOffset = *(lpdwTemp + wi);
	}
	else
	{
		lpwTemp = (LPWORD)lpTemp;
		dwOffset = (DWORD) (*lpwTemp + wi);
	}
	GlobalUnlock(lpTIFDVar->hStripOffsets);
	lpTemp = (char*) GlobalLock(lpTIFDVar->hStripByteCount);
	if((lpTIFDVar->wByteCntType == DTN_LONG) || (lpTIFDVar->wByteCntType == DTN_SLONG))
	{
		lpdwTemp = (LPDWORD)lpTemp;
		dwByteCnt = *(lpdwTemp + wi);
	}
	else
	{
		lpwTemp = (LPWORD)lpTemp;
		dwByteCnt = (DWORD) (*lpwTemp + wi);
	}
	GlobalUnlock(lpTIFDVar->hStripByteCount);
	
	lpTIFDVar->dwDataLen = dwByteCnt;
	lpTIFDVar->wMemLen = ((lpTIFDVar->dwDataLen > (DWORD)MAX_BUFF_SIZE) ? 
						 (DWORD)MAX_BUFF_SIZE : (DWORD)lpTIFDVar->dwDataLen);
	m_SrcFile->Seek(dwOffset,CFile::begin);
}