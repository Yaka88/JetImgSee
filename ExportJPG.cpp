// ExportJPG.cpp: implementation of the CExportJPG class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ExportJPG.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����ΪJPG��ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "ExportJPG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExportJPG::CExportJPG()
{

}

CExportJPG::~CExportJPG()
{

}

extern int iReturn;
// ��������	: ����HDIBΪJPG�ļ�
// ����		: HDIB hInBmp,ͼ���� LPSTR lpszDstFNameĿ���ļ�,short CompFactor,ѹ����short HeadOffsetͷ��ƫ�ƣ�����TIFF)
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:
int CExportJPG::iExportJPGEncode(HDIB hInBmp, LPSTR lpszDstFName, short CompFactor,short HeadOffset)
{
	if ((CompFactor > 0) && (CompFactor <= 65535))
		m_jpginit.gwQuality = CompFactor;
	else
		m_jpginit.gwQuality = 50; 

	BITMAPINFOHEADER BIH;
	JPGC_VAR JCVAR;
	HANDLE hBI_Struct, hImage;
	LPSTR lpImage;
	LPBITMAPINFOHEADER lpBIH;
	RGBQUAD *lpRGB;
	WORD wBI_Size, wPalSize, wColors, wBits, wWidthBytes, wHeight;
	DWORD dwDataLen;
	WORD wDefRows, wProcRows, wPassRows;
	CFile*			 m_TempFile = new CFile();
	CFileException m_error;

	LPBYTE lpInBmp = (LPBYTE)::GlobalLock(hInBmp);

	if (lpInBmp == NULL)		//add by yaka 20010528
	{
		iReturn = GLOBALLOCKERROR;
		return iReturn;
	}
	BIH = *((LPBITMAPINFOHEADER)lpInBmp);
	lpInBmp += sizeof(BITMAPINFOHEADER);

	if(BIH.biCompression != BI_RGB)
	{	AfxMessageBox("��BMP�ļ��Ѿ�ѹ����!");
		iReturn = BMP_HAVECOMPRESS;	
		return iReturn;
	}
	wBits = BIH.biBitCount;
	if((wBits < 8) || (wBits > 24) )
	{
		AfxMessageBox("JPEG��֧�ַ����ɫ�ͺڰ�ͼ��!");
		iReturn = NOTSUPPORT_BW;
		return iReturn;
	}
	if (wBits == 16)
	{
		JCVAR.Is16 = TRUE;
		wBits = 24;
	}
	else 
		JCVAR.Is16 = FALSE;
	wWidthBytes = (WORD) DWORD_WBYTES(BIH.biWidth * (DWORD)wBits);
	wColors = IMAGE_COLORS(wBits,1);
	if(BIH.biSizeImage == 0)
		BIH.biSizeImage = (DWORD)wWidthBytes * BIH.biHeight;
	if(BIH.biClrUsed == 0)
		BIH.biClrUsed = (DWORD)wColors;
	wColors = (WORD)BIH.biClrUsed;

	// ** Allocate for the BITMAPINFO structure and the color table
	wPalSize = wColors * sizeof(RGBQUAD);
	wBI_Size = (WORD)BIH.biSize + wPalSize;
	hBI_Struct = GlobalAlloc(GHND, (DWORD)wBI_Size);
	lpBIH = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);

	if (lpBIH == NULL)		//add by yaka 20010528
	{
		iReturn = GLOBALLOCKERROR;	
		return iReturn;
	}  //add end
	*lpBIH = BIH;
	lpRGB = (RGBQUAD FAR*)((LPSTR)lpBIH + BIH.biSize);

	if(wColors > 0)
	{
		// ** read palette from BMP
		//m_SrcFile->Read((LPSTR)lpRGB,wPalSize); //BMP color ����ÿ����ɫҪ�ĸ��ֽڣ�һ��Ϊ�գ�
		memcpy(lpRGB,lpInBmp,wPalSize);
		lpInBmp += wPalSize;
	}
	if(!m_TempFile->Open(lpszDstFName,CFile::modeCreate | CFile::modeWrite,&m_error))
	{
		m_error.ReportError();
		::GlobalUnlock(hInBmp);
//		::GlobalFree(hInBmp);  //rem by yaka 20010525
		iReturn = CREATEJPG_FAILED;
		return iReturn;
	}
	//add by yaka 20010621
	if (HeadOffset) 
		m_TempFile->Write("TEMPJPEG",HeadOffset);
	else
		m_TempFile->SeekToBegin();
	//add end
	m_jpginit.gbGray = (wBits == 8) ? TRUE : FALSE;
	JCVAR.wWidth = (WORD)BIH.biWidth;
	JCVAR.wDepth =  (WORD)BIH.biHeight;

	iReturn = m_jpginit.iEncode_Init(&JCVAR);
	iReturn = m_jpgcmark.iWriteJPGHeader(m_TempFile,&JCVAR);

	wHeight = (WORD)BIH.biHeight;
	wDefRows = JCVAR.wMaxVer_SampleFactor << 3; // һ��MCU������(�����ȳɷ�Ϊ׼)��
	dwDataLen = wWidthBytes * wDefRows;
	hImage = GlobalAlloc(GHND,dwDataLen);
	lpImage = (LPSTR)GlobalLock(hImage);
	if (lpImage == NULL)		//add by yaka 20010528
	{
		iReturn = GLOBALLOCKERROR;	
		return iReturn;	
	}  //add end
	if (BIH.biBitCount == 16)
	{
		wBits = 16;
		wWidthBytes = (WORD) DWORD_WBYTES(BIH.biWidth * (DWORD)wBits);
		dwDataLen = wWidthBytes * wDefRows;
	}

	// ** ÿ�δ���һMCU�У�
	WORD wTempCnt = wHeight/wDefRows;
	wProcRows = wDefRows;

	//add by yaka 20010531
	//��HDIB�ײ���ʼ������
   lpInBmp = lpInBmp + (wHeight - 1) * wWidthBytes;
   //add end
	for(wPassRows=0; wPassRows< wTempCnt; wPassRows ++)
	{
		try
		{
			//m_SrcFile->Read((LPSTR)lpImage,wDataLen); //ÿ�ζ�ȡһMCU�У�
			//add by yaka 20010531
			for (int i1 = 0; i1 < wDefRows;i1++)
			{
				memcpy(lpImage + i1 * wWidthBytes,lpInBmp,wWidthBytes);
				lpInBmp -= wWidthBytes;
			}
			//add end
				//memcpy(lpImage,lpInBmp,dwDataLen);
				//lpInBmp += dwDataLen;
			if(BIH.biBitCount == 16)
				Turn16To24(lpImage,BIH.biWidth,wDefRows);

		}
		catch(CFileException pe)
		{
			pe.ReportError();
			m_TempFile->Close();
			::GlobalUnlock(hInBmp);
			//::GlobalFree(hInBmp); //rem by yaka 20010525
			iReturn = READSOURCEIMG_ERR;
			return iReturn;
		}
		if (BIH.biBitCount == 16)
			wWidthBytes = (WORD) DWORD_WBYTES(BIH.biWidth * (DWORD)24);
		iReturn = iCreateJPG(m_TempFile,&JCVAR,lpImage,wProcRows,wWidthBytes,FALSE);
	}
	if(wProcRows = wHeight % wDefRows)
	{
		try
		{
			//m_SrcFile->Read((LPSTR)lpImage,dwDataLen); //ÿ�ζ�ȡһMCU�У�
			//add by yaka 20010531
			for (int i1 = 0; i1 < wProcRows;i1++)
			{
				memcpy(lpImage + i1 * wWidthBytes,lpInBmp,wWidthBytes);
				lpInBmp -= wWidthBytes;
			}
			//add end

			//memcpy(lpImage,lpInBmp,wWidthBytes * wProcRows);//dwDataLen
			//lpInBmp += wWidthBytes * wProcRows;
			if(BIH.biBitCount == 16)
				Turn16To24(lpImage,BIH.biWidth,wProcRows);
		}
		catch(CFileException pe)
		{
			pe.ReportError();//pe.Delete();
			m_TempFile->Close();
			::GlobalUnlock(hInBmp);
			//::GlobalFree(hInBmp);//rem by yaka 20010525
			iReturn = READSOURCEIMG_ERR;
			return iReturn;
		}
		iReturn = iCreateJPG(m_TempFile,&JCVAR,lpImage,wProcRows,wWidthBytes,TRUE);
	}
	else
	{
		// ** ��֤���õı����ܹ���ȫд���ļ�(ͨ������һ���ֽ�ʵ�֣���ӵ�7��1�У�����ܻ����м�λ�ڻ������ж���������)
		iReturn = m_jpgchuff.iBits_Encode(m_TempFile,&JCVAR,(WORD)0x7F,(WORD)7);
		if(!iReturn && JCVAR.dwBuffCnt)
		{
			m_TempFile->Write(JCVAR.lpDataBuff,JCVAR.dwBuffCnt);
		}
		if(!iReturn)
		{
			WORD wMarker = (WORD)0xD9FF;// ** Write EOI marker 
			m_TempFile->Write(&wMarker,2);
		}
	}
	vFreeEncodeMem(&JCVAR);
	GlobalUnlock(hImage);
	GlobalFree(hImage);
	GlobalUnlock(hBI_Struct);
	GlobalFree(hBI_Struct);

	m_TempFile->Close();
	::GlobalUnlock(hInBmp);
//	::GlobalFree(hInBmp);  //rem by yaka 20010525
	return 0;
}

// ��������	:���ڽ�16λDIBת��Ϊ24λDIB;
// ����		: LPSTR lpImage,LONG biWidthͼ����,LONG wDefRowsͼ��߶�
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:
BOOL CExportJPG::Turn16To24(LPSTR lpImage,LONG biWidth,LONG wDefRows)
{
	WORD wWidthBytes = (WORD) DWORD_WBYTES(biWidth * (DWORD)16);
	WORD wWidthBytes24 = (WORD) DWORD_WBYTES(biWidth * (DWORD)24);
	WORD wValue;
	int nByte, nLine;
	LPSTR lpImageTemp, lpChangeTemp;
	HGLOBAL hChange = GlobalAlloc(GHND,wWidthBytes*wDefRows);
	LPSTR lpChange = (LPSTR)GlobalLock(hChange);
	if(lpChange == NULL)
		return FALSE;
	memcpy(lpChange,lpImage,wWidthBytes*wDefRows);
	lpChangeTemp = lpChange;
	lpImageTemp = lpImage;
	for(nLine = 1; nLine < wDefRows+1; nLine++)
	{
		for(nByte = (int) biWidth; nByte; nByte -= 1)
		{
		    wValue = *((WORD *) (lpChange+0));
            *lpImage++ = (BYTE) ((wValue & 0x001f)<<3);
			*lpImage++ = (BYTE) ((wValue & 0x03e0)>>2);   
			*lpImage++ = (BYTE) ((wValue & 0x7c00)>>7);  
			lpChange++;
			lpChange++;
		}
		while((lpImage-lpImageTemp) < wWidthBytes24*nLine)
			*lpImage++ = 0;
		lpChange = lpChangeTemp + wWidthBytes*nLine;
	}
	GlobalUnlock(hChange);
	GlobalFree(hChange);
	return TRUE;
}

// ��������	: ����JPG�ļ�
// ����		: CFile *m_DstFile, LPBYTE lpSrcImage, LPJPGC_VAR lpJCVar, WORD wWidthBytes,WORD wProcRows,BOOL bEOI
//				Ŀ���ļ�            Դͼ����			JPG�ṹ					���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 


int CExportJPG::iCreateJPG(CFile *m_DstFile, LPJPGC_VAR lpJCVAR, LPSTR lpImage, WORD wProcRows, WORD wWidthBytes,BOOL bEOI )
{
	LPJPG_COMP_INFO lpCompPtr;
	LPSTR lpSrcBuff[MAX_COMPS_IN_SCAN];
	LPSTR lpSrcTmp0, lpSrcTmp1, lpSrcTmp2;
	LPSTR lpImageTmp;
	HANDLE hSrcBuff[MAX_COMPS_IN_SCAN];
	HANDLE hCompData[MAX_COMPS_IN_SCAN];
	WORD wH_MCUs_PerLoop;
	WORD wFullWidth;
	WORD wDefRows,  wMarker;
	DWORD dwBuffCnt;
	WORD wi, wj;
	MYWORD iR, iG, iB, iY, iCb, iCr;

	if(lpJCVAR->wComps_SOS == 1)
		wH_MCUs_PerLoop = lpJCVAR->Component[0].wVer_SampleFactor;
	else
		wH_MCUs_PerLoop = 1;
	
	// ** Compute dimensions of full-size pixel buffers. Note these are the same whether interleaved or not.
	wDefRows = lpJCVAR->wMaxVer_SampleFactor << 3;
	wFullWidth = m_utility.wRoundUp(lpJCVAR->wWidth,lpJCVAR->wMaxHor_SampleFactor << 3);

	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];
		hCompData[wi] = GlobalAlloc(GHND,(DWORD)lpCompPtr->wSubsampled_Width * 
										 ((DWORD)lpCompPtr->wMCU_Depth << 3));
	}
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		hSrcBuff[wi] = GlobalAlloc(GHND,(DWORD)wDefRows * (DWORD)wFullWidth);
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
	{
		lpSrcBuff[wi] = (LPSTR)GlobalLock(hSrcBuff[wi]);
		if (lpSrcBuff[wi] == NULL)		//add by yaka 20010528
		{
			iReturn = GLOBALLOCKERROR;
			return iReturn;
		}   //add  end
	}

	if(lpJCVAR->ColorSystem == CS_YCbCr)
	{
		// ** RGB input
		lpSrcTmp0 = lpSrcBuff[0];
		lpSrcTmp1 = lpSrcBuff[1];
		lpSrcTmp2 = lpSrcBuff[2];
	}
	else
	{
		// ** Gray input
		lpSrcTmp0 = lpSrcBuff[0];
	}
	for(wi=0; wi<wProcRows; wi++)
	{
		lpImageTmp = lpImage;
		if(lpJCVAR->ColorSystem == CS_YCbCr)
		{
			// ** RGB input
			for(wj=0; wj<lpJCVAR->wWidth; wj++)
			{
				// ** Convert BGR to YCbCr;
				iB		= (MYWORD)(*lpImageTmp++ & (BYTE)0xFF);
				iG		= (MYWORD)(*lpImageTmp++ & (BYTE)0xFF);
				iR		= (MYWORD)(*lpImageTmp++ & (BYTE)0xFF);
				iY		= ((38 * iR) + (75 * iG) + ( 15 * iB)) >> 7;
				iCb		= (((-22 * iR) - (42 * iG) + (64 * iB)) >> 7) + 128;
				iCr		= (((64 * iR) - (54 * iG) - (10 * iB)) >> 7) + 128;
				lpSrcTmp0[wj] = (BYTE)iY;
				lpSrcTmp1[wj] = (BYTE)iCb;
				lpSrcTmp2[wj] = (BYTE)iCr;
			}
			lpSrcTmp0 += wFullWidth;
			lpSrcTmp1 += wFullWidth;
			lpSrcTmp2 += wFullWidth;
		}
		else
		{
			// ** GRAY input
			memcpy(lpSrcTmp0, lpImage, lpJCVAR->wWidth);   
			lpSrcTmp0 += wFullWidth;
		}    
		lpImage += wWidthBytes;
	}

	// ** �����������������е���������ͺ��������һ��������MCU��,
	// ** (MCU�У���ΪwFullWidth,��ΪwDefRows).��������Դ����lpSrcBuff�У�
	if((lpJCVAR->wWidth < wFullWidth) || bEOI)
		vEdgeExpand(lpJCVAR,wFullWidth, wProcRows, lpSrcBuff);
	
	// ** ��ɫ���������������ɷֵ����ݴ����hCompData�У�
	iReturn = m_jpgcsamp.iSubSample(lpJCVAR,wFullWidth,lpSrcBuff,hCompData); 
	// ** ���ˣ�һ��MCU�����������ݿ�(block)��δ����
	iReturn = m_jpgcmcu.iExtractMCU(m_DstFile, lpJCVAR,hCompData,wH_MCUs_PerLoop);

	if(bEOI)
	{
		// ** ��֤���õı����ܹ���ȫд���ļ�(ͨ������һ���ֽ�ʵ�֣���ӵ�7��1�У�����ܻ����м�λ�ڻ������ж���������)
		iReturn = m_jpgchuff.iBits_Encode(m_DstFile,lpJCVAR,(WORD)0x7F,(WORD)7);
		if(!iReturn && lpJCVAR->dwBuffCnt)
		{
			dwBuffCnt = lpJCVAR->dwBuffCnt;
			m_DstFile->Write(lpJCVAR->lpDataBuff,dwBuffCnt);
		}
		if(!iReturn)
		{
			// ** Write EOI marker
			wMarker = (WORD)0xD9FF;
			m_DstFile->Write((LPSTR)&wMarker,2);
		}
	}
	for(wi=0;wi<lpJCVAR->wComps_SOS; wi++)
	{
		GlobalUnlock(hSrcBuff[wi]);
		GlobalFree(hSrcBuff[wi]);
		GlobalFree(hCompData[wi]);
	}

	return iReturn;
}


// ��������	:�߽���չ
// ����		: LPJPGC_VAR lpJCVAR, WORD wFullWidth, WORD wProcRows, LPSTR *lpSrcBuff
// ����ֵ	: 
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:
void CExportJPG::vEdgeExpand(LPJPGC_VAR lpJCVAR, WORD wFullWidth, WORD wProcRows, LPSTR *lpSrcBuff)
{
	LPSTR lpRowTemp, lpColTemp;
	WORD wDefRows, wLeftRows, wLeftCols, wColOff;
	WORD wi, wj, wk;
	BYTE byChar;

	// ** Expand an image so that it is a multiple of the MCU dimensions.
	// ** This is to be accomplished by duplicating the rightmost column subsampled,
	// ** so all components have the same dimensions .
	// ** Expand horizontally
	if(lpJCVAR->wWidth < wFullWidth)
	{
		wLeftCols = wFullWidth - lpJCVAR->wWidth;
		wColOff = lpJCVAR->wWidth - 1;
		for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		{
			lpColTemp = lpSrcBuff[wi] + wColOff;
			for(wj=0; wj<wProcRows; wj++)
			{
				byChar = lpColTemp[0];
				for(wk=1; wk<=wLeftCols; wk++)
					lpColTemp[wk] = byChar;
				lpColTemp += wFullWidth; //�Ƶ���һ�У�
			}
		}
	}
	// ** This happens only once at the bottom of the image, so
	// ** it needn't be super-efficient.
	// ** Expand vertically
	wDefRows = lpJCVAR->wMaxVer_SampleFactor << 3;
	if(wProcRows < wDefRows)
	{
		wLeftRows = wDefRows - wProcRows;
		for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		{
			lpRowTemp = lpSrcBuff[wi] + (wProcRows - 1) * wFullWidth;
			for(wj=0; wj<wLeftRows; wj++)
			{
				memcpy(lpRowTemp + wFullWidth, lpRowTemp, wFullWidth);
				lpRowTemp += wFullWidth;
			}
		}
	}
}


// ��������	:�ͷ��ڴ�
// ����		: LPJPGC_VAR lpJCVAR
// ����ֵ	: 
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:
void CExportJPG::vFreeEncodeMem(LPJPGC_VAR lpJCVAR)
{
	WORD wi;
	for(wi=0; wi<NUM_HUFF_TABLES; wi++)
	{
		GlobalFree(lpJCVAR->hDC_HuffTBL[wi]);
		GlobalFree(lpJCVAR->hAC_HuffTBL[wi]);
	}
	GlobalUnlock(lpJCVAR->hEncodeBuff);
	GlobalFree(lpJCVAR->hEncodeBuff);

}

