// ImportJPG.cpp: implementation of the CImportJPG class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ImportJPG.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��JPG��ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "ImportJPG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern int g_ImageCount;
extern BYTE g_ImageFormat;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImportJPG::CImportJPG()
{

}

CImportJPG::~CImportJPG()
{

}

extern int iReturn;

// ��������	: ��JPEG�ļ�
// ����		: LPSTR lpszSrcFName��long HeadOffsetͷ��ƫ�ƣ�����TIFF)
// ����ֵ	: HDIB ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB CImportJPG::iTmportJPGDecode(LPSTR lpszSrcFName, long HeadOffset)
{
	JPGD_VAR JDVAR;
	HANDLE hBI_Struct;
	LPBITMAPINFOHEADER lpBIH;
	LPBITMAPINFO lpBI;
	WORD wMarker, wBI_Size, wPalSize, wColors, wBits, wWidthBytes, wi;
	BYTE  byTemp;  
	CFile*			 m_SrcFile = new CFile();
	CFileException m_error;

	if(!m_SrcFile->Open(lpszSrcFName,CFile::modeRead ,&m_error))
	{
		m_error.ReportError();
		iReturn = READSOURCEIMG_ERR;
		return NULL;
	}
	//modify by yaka 20010524
	if (HeadOffset == 0)
		m_SrcFile->SeekToBegin();
	else
		m_SrcFile->Seek(HeadOffset,CFile::begin);

		//modify end


	m_SrcFile->Read((LPSTR)&wMarker,2);
	if(!CHECK_JPG_SOI(wMarker))
	{
		m_SrcFile->Close();
		iReturn = NOJPEG;
		AfxMessageBox("����JPEG��ʽ!");
		return NULL ;
	}
	iReturn = m_jpgdinit.iDecode_Init(&JDVAR);
	iReturn = m_jpgdmark.iReadJPGHeader(m_SrcFile, &JDVAR);
	
	if(iReturn)
	{		
		m_SrcFile->Close();
		vFreeDecodeMem(&JDVAR);
		iReturn = UNSUPPORT_JPG;
		AfxMessageBox("��֧�ֵ�JPEG�ļ���ʽ!");
		return NULL;
	}
	if(JDVAR.Is16)
		wBits = 16;
	else
		wBits = (JDVAR.ColorSystem == CS_GRAY) ? 8 : 24;
	wColors = IMAGE_COLORS(wBits,1);
	wWidthBytes = (WORD)DWORD_WBYTES((DWORD)JDVAR.wWidth * (DWORD)wBits);
	wPalSize = wColors * sizeof(RGBQUAD);
	wBI_Size = sizeof(BITMAPINFOHEADER) + wPalSize;
	hBI_Struct = GlobalAlloc(GHND, (DWORD)wBI_Size);
	lpBIH = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);

	if (lpBIH == NULL)		//add by yaka 20010528
	{
		iReturn = GLOBALLOCKERROR;
		return NULL;
	}					 //add  end
	lpBIH->biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	lpBIH->biWidth = (DWORD)JDVAR.wWidth;
	lpBIH->biHeight = (DWORD)JDVAR.wDepth;
	lpBIH->biPlanes = 1;
	lpBIH->biBitCount = wBits;

	lpBIH->biCompression = BI_RGB;
	//add by yaka 20010709
	switch(JDVAR.byDensityUnit)
	{
	case 1:	lpBIH->biXPelsPerMeter = (LONG)((JDVAR.wXDensity/25.4)*1000 + 0.5);
			lpBIH->biYPelsPerMeter = (LONG)((JDVAR.wYDensity/25.4)*1000 + 0.5);break;
	case 2: lpBIH->biXPelsPerMeter = (LONG)JDVAR.wYDensity*100;
		    lpBIH->biYPelsPerMeter = (LONG)JDVAR.wYDensity*100;break;
	default:lpBIH->biXPelsPerMeter = 0;
			lpBIH->biYPelsPerMeter = 0;
	}
	//add end
	lpBIH->biClrUsed = wColors;
	lpBIH->biClrImportant = 0;
	lpBIH->biSizeImage = (DWORD)wWidthBytes * lpBIH->biHeight;
	lpBI = (LPBITMAPINFO)lpBIH;
	g_ImageCount = 1;
	g_ImageFormat = JPG;
	if(wColors == 256) //�Ҷ�ͼ��
	{
		for(wi=0;wi<wColors;wi++)
		{
			byTemp = (BYTE)(wi & 0xFF);
			lpBI->bmiColors[wi].rgbRed = byTemp;
			lpBI->bmiColors[wi].rgbGreen = byTemp;
			lpBI->bmiColors[wi].rgbBlue = byTemp;
			lpBI->bmiColors[wi].rgbReserved = (BYTE)0;
		}
	}
	HDIB hOutTiff;
	LPBYTE pOutTiff;
	hOutTiff = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBI->bmiHeader.biSizeImage+wBI_Size);
	pOutTiff = (LPBYTE) ::GlobalLock((HGLOBAL) hOutTiff);	
	if (pOutTiff == NULL)
	{//add by yaka 20010525
		AfxMessageBox("����ռ����!");
		m_SrcFile->Close();
		iReturn = GLOBALLOCKERROR;
		return NULL;
	}//add end
	memcpy(pOutTiff,lpBI,wBI_Size); 
	/*// ** Write the bmp file header
	m_TempFile->Write(&BFH,wBFH_Size);
	// ** Write the bmp info header
	m_TempFile->Write(lpBIH,wBI_Size);*/
	GlobalUnlock(hBI_Struct);
	GlobalFree(hBI_Struct);

	if(JDVAR.Is16)
		wWidthBytes = (WORD)DWORD_WBYTES((DWORD)JDVAR.wWidth * (DWORD)24);

	if(iReturn = iCreatTmpJPG(m_SrcFile,pOutTiff+wBI_Size,&JDVAR, wWidthBytes))// Is a EOI Marker?
	{
		AfxMessageBox("�Ҳ����ļ�������־ !");
		m_SrcFile->Close();
		::GlobalUnlock((HGLOBAL) hOutTiff);
		::GlobalFree((HGLOBAL) hOutTiff);	
		return NULL;
	}
	// ** ��ѹ�Ǳ����ѹ����JPEGͼ���轫�õ���BMPͼ��ֱ�ߵ���
	//rem by yaka 20010531
//	if(!JDVAR.IsJetSoft)
//		if(m_utility.Ver_ReverseDib(pOutTiff))
//			AfxMessageBox("�ڴ治����ͼ��ֱ��תʧ��!");

	m_SrcFile->Close();
	delete m_SrcFile;
	vFreeDecodeMem(&JDVAR);
	::GlobalUnlock((HGLOBAL) hOutTiff);
	return hOutTiff;
}


// ��������	: ����HDIB�ļ�
// ����		: CFile *m_SrcFile, LPBYTE lpOUTJPEG, LPJPGD_VAR lpJDVar, WORD wWidthBytes
//				Դ�ļ�            ͼ����			GIF�ṹ					���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CImportJPG::iCreatTmpJPG(CFile *m_SrcFile, LPBYTE pOutJpeg, LPJPGD_VAR lpJDVAR, WORD wWidthBytes)
{
	LPJPG_COMP_INFO lpCompPtr;
	LPSTR lpRealData[MAX_COMPS_IN_SCAN];
	LPSTR lpDstBuff[MAX_COMPS_IN_SCAN];
	LPSTR lpImage, lpDstTmp0, lpDstTmp1, lpDstTmp2;
	LPSTR lpTmp0, lpTmp1;
	HANDLE hSrcBuff;
	HANDLE hCompData[MAX_COMPS_IN_SCAN];
	HANDLE hRealData[MAX_COMPS_IN_SCAN];
	HANDLE hDstBuff[MAX_COMPS_IN_SCAN];
	HANDLE hImage;
	DWORD dwDataBgn;
	DWORD dwDataLen;
	WORD wH_MCUS_PerLoop;
	WORD wProcRows, wPassRows, wFullWidth, wMarker;
	DWORD dwPartDataLen;
	WORD wi, wj, wk;
	WORD wWidthBytesDIB;  //add by yaka 20010531
	int iY, iCb, iCr, iR, iG, iB;
	iReturn = 0;

	m_jpgdinit.vDecode_Component_Init(lpJDVAR);
	m_jpgdinit.vDecode_HuffTBL_Init(lpJDVAR);

	if(lpJDVAR->wComps_SOS == 1)
	{
		m_jpgdinit.vDecode_NonInterleaved_Init(lpJDVAR);
		wH_MCUS_PerLoop = lpJDVAR->Component[0].wVer_SampleFactor;
	}
	else
	{
		m_jpgdinit.vDecode_Interleaved_Init(lpJDVAR);
		wH_MCUS_PerLoop = 1;
	}
	//add by yaka 20010531
	if (lpJDVAR->Is16)
		wWidthBytesDIB = (WORD)DWORD_WBYTES((DWORD)lpJDVAR->wWidth * (DWORD)16);
	else
		wWidthBytesDIB = wWidthBytes;
	//add end




	// ** Computer dimensions of full-size pixel buffers .Note these are the same whither
	// ** interleaved or not
	wProcRows = lpJDVAR->wMaxVer_SampleFactor << 3;
	wFullWidth = m_utility.wRoundUp(lpJDVAR->wWidth,lpJDVAR->wMaxHor_SampleFactor << 3);

	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];
		// **(<<4) means (*DCT_SIZE * 2UL), 2UL means sizeof(int);
		// ** �óɷ���һ��MCU���е����ݴ�С��
		hCompData[wi] = GlobalAlloc(GHND,(DWORD)lpCompPtr->wSubsampled_Width *
									((DWORD)lpCompPtr->wVer_SampleFactor << 4));//5
	}
	// ** ��Ž����ÿ���ɷֵ�����,��������ɫ�ȳɷֵ����ݻ�û������;
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];
		// ** (<<3) means (*DCT_SIZE)
		hRealData[wi] = GlobalAlloc(GHND,(DWORD)lpCompPtr->wSubsampled_Width *
									((DWORD)lpCompPtr->wVer_SampleFactor << 3));
	}

	// ** ���ڴ�Ž����������ɷֵ�����,���ݷ�Χ�൱�ڽ���ǰ��һ��MCU��,����������ɫ�ȳɷֵ������Ѿ�����(��������);
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
		hDstBuff[wi] = GlobalAlloc(GHND,(DWORD)wProcRows * (DWORD)wFullWidth);

	dwPartDataLen = wWidthBytes * wProcRows;
	hImage = GlobalAlloc(GHND,dwPartDataLen);
	dwDataBgn = (DWORD)m_SrcFile->Seek(0L,CFile::current);
	dwDataLen = (DWORD)m_SrcFile->Seek(0L,CFile::end);
	dwDataLen -= dwDataBgn;
	m_SrcFile->Seek((LONG)dwDataBgn,CFile::begin);
	hSrcBuff = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
	lpJDVAR->dwDataLen = dwDataLen;
	lpJDVAR->wMemLen = MAX_BUFF_SIZE;
	lpJDVAR->lpDataBuff = (LPSTR)GlobalLock(hSrcBuff);
	if (lpJDVAR->lpDataBuff == NULL)		//add by yaka 20010528
	{
		iReturn = GLOBALLOCKERROR;
		return iReturn;
	}					 //add  end

	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		lpRealData[wi] = (LPSTR)GlobalLock(hRealData[wi]);
		lpDstBuff[wi] = (LPSTR)GlobalLock(hDstBuff[wi]);
		if (lpRealData[wi] == NULL || lpDstBuff[wi] == NULL)  //add by yaka 20010528
		{
			iReturn = GLOBALLOCKERROR;
			return iReturn;
		}				//add end 

	}
	lpImage = (LPSTR)GlobalLock(hImage);

	if (lpImage == NULL)		//add by yaka 20010528
	{
		iReturn = GLOBALLOCKERROR;
		return iReturn;
	}					 //add  end

	iReturn = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
	lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
	lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
	wPassRows = 0;


	//add by yaka 20010531
	 //��HDIB�ײ���ʼд����
   pOutJpeg = pOutJpeg + (lpJDVAR->wDepth - 1)*wWidthBytesDIB;
 

	// ** wH_MCUS_PerLoop��ʾÿһ��ѭ��������MUC��;
	for(wi=0; wi<lpJDVAR->wH_MCUs; wi+=wH_MCUS_PerLoop)
	{
		// ** Obtain wVer_SampleFactor block rows of each component in the scan.This is a 
		// ** single MCU row if interleaved,multiple MCU rows if not. In the noninterleaed
		// ** case there might be fewer than wVer_SampleFactor block rows remaining;
		for(wj=0; wj<wH_MCUS_PerLoop; wj++)
		{
			if((wi+wj) < lpJDVAR->wH_MCUs)
			{	
				// ** ��lpJDVAR->lpDataBuff��ָ���ѹ�������н�ѹ��һ��MCU��(����lpJDVAR->wW_MCUs��MCU��Ԫ),
				// ** ����ŵ�hCompData��ָ���ڴ����,�����Ѿ���������;
				iReturn = m_jpgdmcu.iDisassembleMCU(m_SrcFile,lpJDVAR,hCompData);
				// ** ��hCompData(���ڴ����2�ֽ�Ϊ��λ)��ָ���ڴ���ж�ȡ����,������DCT�任,Ȼ��ÿ��Դ������int��
				// ** ��ֵ����128��ȷ������0-255��Ӷ���ɵ��ֽ���,����ŵ�lpRealData(���ڴ����1�ֽ�Ϊ��λ)��;
				m_jpgdmcu.vReverseDCT(lpJDVAR,hCompData,lpRealData,wj);
			}
			else 
			{
				// ** ����ԻҶ�ͼ��,���һ��ѭ����,�����MCU��������С�ڼȶ���wH_MCUS_PerLoop;	
				break;
			}
		}
		
		// ** Expand row groups of this set
		iReturn = m_jpgdexpa.iExpand(lpJDVAR,wFullWidth,lpRealData,lpDstBuff);
		wProcRows = (WORD)MIN(wProcRows,(lpJDVAR->wDepth-wPassRows));
		if(lpJDVAR->ColorSystem == CS_YCbCr)
		{
			// ** RGB output
			lpDstTmp0 = lpDstBuff[0];
			lpDstTmp1 = lpDstBuff[1];
			lpDstTmp2 = lpDstBuff[2];
		}
		else
		{
			// ** Gray output
			lpDstTmp0 = lpDstBuff[0];
		}
		lpTmp0 = lpImage;
		for(wj=0; wj<wProcRows; wj++)
		{
			if(lpJDVAR->ColorSystem == CS_YCbCr)
			{
				// ** RGB output
				lpTmp1 = lpTmp0;
				for(wk=0; wk<lpJDVAR->wWidth; wk++)
				{
					iY = (int)(lpDstTmp0[wk] & (BYTE)0xFF);
					iCb = (int)(lpDstTmp1[wk] & (BYTE)0xFF) - 128;
					iCr = (int)(lpDstTmp2[wk] & (BYTE)0xFF) - 128;
					iR = iY + ((179 * iCr) >> 7);
					iG = iY - (((88 * iCb) + (183 * iCr)) >> 8);
					iB = iY + ((227 * iCb) >> 7);
					*lpTmp1++ = (BYTE)MAX(MIN(iB,255),0);
					*lpTmp1++ = (BYTE)MAX(MIN(iG,255),0);
					*lpTmp1++ = (BYTE)MAX(MIN(iR,255),0);
				}
				lpDstTmp0 += wFullWidth;
				lpDstTmp1 += wFullWidth;
				lpDstTmp2 += wFullWidth;
			}
			else
			{
				// ** Gray output
				memcpy(lpTmp0,lpDstTmp0,lpJDVAR->wWidth);
				lpDstTmp0 += wFullWidth;
			}
			lpTmp0 += wWidthBytes;
		}
		wPassRows += wProcRows;
		if (lpJDVAR->Is16)// ���16λ��ɫͼ��
		{
				//add by yaka 20010531
			Turn24To16(lpImage,lpJDVAR->wWidth,wProcRows);
		}
			for (int i1 = 0; i1 < wProcRows;i1++)
			{
				memcpy(pOutJpeg,lpImage + i1 * wWidthBytesDIB,wWidthBytesDIB);
				pOutJpeg -= wWidthBytesDIB;
			}
			//add end
		//	WORD wTempDataLen = (WORD)DWORD_WBYTES((DWORD)lpJDVAR->wWidth * (DWORD)16) * wProcRows;
		//	
		//	memcpy(pOutTiff,lpImage,wTempDataLen);
		//	pOutTiff += wTempDataLen;
		//}
		//else
		//{
		//	memcpy(pOutTiff,lpImage,wWidthBytes * wProcRows);
		//	pOutTiff += wWidthBytes * wProcRows;
		//}
	}
	memcpy((LPSTR)&wMarker,lpJDVAR->lpBgnBuff,2);

	// ** Is a EOI Marker?
//	if(!CHECK_JPG_EOI(wMarker))
//		iReturn = NO_EOI_JPG;

	GlobalUnlock(hSrcBuff);
	GlobalUnlock(hImage);
	GlobalFree(hSrcBuff);
	GlobalFree(hImage);
	for(wj=0; wj<lpJDVAR->wComps_SOS; wj++)
	{
		GlobalUnlock(hDstBuff[wj]);
		GlobalUnlock(hRealData[wj]);
		GlobalFree(hDstBuff[wj]);
		GlobalFree(hRealData[wj]);
		GlobalFree(hCompData[wj]);
	}

	return iReturn;
}



void CImportJPG::vFreeDecodeMem(LPJPGD_VAR lpJDVAR)
{
	WORD wi;
	for(wi=0; wi<NUM_HUFF_TABLES;wi++)
	{
		GlobalFree(lpJDVAR->hDC_HuffTBL[wi]);
		GlobalFree(lpJDVAR->hAC_HuffTBL[wi]);
	}
}

// ��������	: ���ڽ�24λDIBת��Ϊ16λDIB;
// ����		: LPSTR lpImage, WORD biWidth, WORD wDefRows
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL CImportJPG::Turn24To16(LPSTR lpImage, WORD biWidth, WORD wDefRows)
{
	WORD wWidthBytes = (WORD) DWORD_WBYTES(biWidth * (DWORD)24);
	WORD wWidthBytes16 = (WORD) DWORD_WBYTES(biWidth * (DWORD)16);
	WORD wValue;
	int nByte, nLine;
	LPSTR lpImageTemp, lpChangeTemp;
	HGLOBAL hChange = GlobalAlloc(GHND,wWidthBytes16*wDefRows);
	LPSTR lpChange = (LPSTR)GlobalLock(hChange);
	if(lpChange == NULL)
		return 1;
	lpChangeTemp = lpChange;
	lpImageTemp = lpImage;
	for(nLine = 1; nLine < wDefRows+1; nLine++)
	{
		for(nByte = (int) biWidth; nByte; nByte -= 1)
		{
			wValue = ((WORD)(*lpImage++ & 0xF8) >> 3);
			wValue |= ((WORD)(*lpImage++ & 0xF8) << 2);
			wValue |= ((WORD)(*lpImage++ & 0xF8) << 7 ) ;
			*lpChange ++ = (BYTE) (wValue & 0x00FF);
			*lpChange ++ = (BYTE) (wValue >> 8);
		}
		lpChange = lpChangeTemp + wWidthBytes16*nLine;
		lpImage = lpImageTemp + wWidthBytes*nLine;
	}
	memcpy(lpImageTemp,lpChangeTemp,wWidthBytes16*wDefRows);
	GlobalUnlock(hChange);
	GlobalFree(hChange);
	return 0;
}


