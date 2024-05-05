// JPGDMark.cpp: implementation of the CJPGDMark class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDmark.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码处理MARK的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGDMark.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGDMark::CJPGDMark()
{
	gbGray = FALSE;
}

CJPGDMark::~CJPGDMark()
{

}

extern int iReturn;

int CJPGDMark::iRead_App0(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	LPWORD lpwTemp;
	LPSTR lpTemp;
	WORD wMarkerLen, wTemp;

	m_SrcFile->Read((LPSTR)&wMarkerLen, 2);
	m_utility.vSwapByte((LPSTR)&wMarkerLen,1);

	// ** wMarkerLen include itself
	wMarkerLen -= 2;
	lpTemp = (LPSTR)GlobalLock(hTemp);
	m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);
	if(strcmp((LPSTR)lpTemp,"JFIF") > 0 || strcmp((LPSTR)lpTemp,"JFIF") < 0)  
	{
		GlobalUnlock(hTemp);
		iReturn = NOJPEG;
		AfxMessageBox("不是JPEG文件!");
		return iReturn ;
	}
	// ** (lpTemp+5) means JPEG major version
	wTemp = *((WORD*)(lpTemp + 5));
	//rem by yaka 20010530
	//if((wTemp != (WORD)0x0101) && (wTemp != (WORD)0x0201))
	//	AfxMessageBox("JPEG文件版本错误!");
	//rem end
	lpwTemp = (LPWORD)(lpTemp + 8);
	m_utility.vSwapByte((LPSTR)lpwTemp, 2);
	lpJDVAR->byDensityUnit = (BYTE) (*(lpTemp + 7));
	lpJDVAR->wXDensity = (WORD) (*(lpwTemp));
	lpJDVAR->wYDensity = (WORD) (*(lpwTemp+ 1));

	GlobalUnlock(hTemp);
	return 0;
}

int CJPGDMark::iRead_App1(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	WORD wMarkerLen;
	LPSTR lpTemp;

	m_SrcFile->Read((LPSTR)&wMarkerLen, 2);
	m_utility.vSwapByte((LPSTR)&wMarkerLen,1);
	wMarkerLen -= 2;
	lpTemp = (LPSTR)GlobalLock(hTemp);
	m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);

	if(*lpTemp == (char)0xFF)
		lpJDVAR->Is16 = TRUE;

	GlobalUnlock(hTemp);
	return 0;
}

int CJPGDMark::iRead_App2(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	WORD wMarkerLen;
	LPSTR lpTemp;

	m_SrcFile->Read((LPSTR)&wMarkerLen, 2);
	m_utility.vSwapByte((LPSTR)&wMarkerLen,1);
	wMarkerLen -= 2;
	lpTemp = (LPSTR)GlobalLock(hTemp);
	m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);

	if(!strcmp((LPSTR)lpTemp,"IsSunyard") || !strcmp((LPSTR)lpTemp,"IsJetSoft"))
		lpJDVAR->IsJetSoft = TRUE;
	else
		lpJDVAR->Is16 = FALSE; // It isn't JetSoft's product,so the APP1 marker isn't reliable.

	GlobalUnlock(hTemp);
	return 0;
}


int CJPGDMark::iRead_DQT(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	LPJPG_QUANT_TBL lpQuantPtr;
	LPMYWORD lpiTemp;
	LPSTR lpTemp;
	WORD wMarkerLen;
	WORD wTBL_Ndx;
	WORD wi;
	BYTE byChar;
	BYTE byPrec;

	m_SrcFile->Read((LPSTR)&wMarkerLen,2);
	m_utility.vSwapByte((LPSTR)&wMarkerLen,1);
	// ** wMarkerLen include itself
	wMarkerLen -= 2;
	lpTemp = (LPSTR)GlobalLock(hTemp);
	m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);
	while(wMarkerLen > 0)
	{
		byChar = *lpTemp ++;
		lpiTemp = (LPMYWORD)lpTemp;
		byPrec = (BYTE)(byChar >> 4);
		wTBL_Ndx = (WORD)(byChar & (BYTE)0x0F);
		lpQuantPtr = lpJDVAR->QuantTBL[wTBL_Ndx];

		// ** 若数据精度为16位，则将64个量化值前后字节颠倒；
		if(byPrec)
			m_utility.vSwapByte((LPSTR)lpiTemp,(WORD)DCT_SQUARE);

		for(wi=0; wi<DCT_SQUARE; wi++)
		{
			// ** (*lpQuantPtr++) means (lpQuantPtr[wi])
			if(byPrec)
				lpQuantPtr[wi] = *lpiTemp++;
			else
				lpQuantPtr[wi] = (MYWORD)(*lpTemp++ & (BYTE)0xFF); // int 
		}
		wMarkerLen -= (DCT_SQUARE + 1);

		if(byPrec)
		{
			wMarkerLen -= DCT_SQUARE;
			lpTemp = (LPSTR)lpiTemp; //使两者同步!
		}
	}
	GlobalUnlock(hTemp);
	return 0;
}


int CJPGDMark::iRead_DRI(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	LPWORD lpwTemp;
	DWORD dwTemp;

	m_SrcFile->Read((LPSTR)&dwTemp, 4);
	lpwTemp = (LPWORD) ((LPSTR)&dwTemp);
	lpwTemp++;
	m_utility.vSwapByte((LPSTR)lpwTemp,1);
	lpJDVAR->wRestart_Interval = *lpwTemp;
	return 0;
}


int CJPGDMark::iRead_SOF(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	LPJPG_COMP_INFO lpCompPtr;
	LPWORD lpwTemp;
	LPSTR lpTemp;
	WORD wMarkerLen, wID0, wID1, wID2;
	WORD wi;
	BYTE byChar, byBitPrec;

	m_SrcFile->Read((LPSTR)&wMarkerLen,2);
	m_utility.vSwapByte((LPSTR)&wMarkerLen,1);
	// ** wMarkerLen include itself
	wMarkerLen -= 2;
	lpTemp = (LPSTR)GlobalLock(hTemp);
	m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);
	byBitPrec = *lpTemp++;

	lpwTemp = (LPWORD)lpTemp;
	m_utility.vSwapByte((LPSTR)lpwTemp,2);
	lpJDVAR->wDepth = *lpwTemp++;
	lpJDVAR->wWidth = *lpwTemp++;

	lpTemp = (LPSTR)lpwTemp;
	lpJDVAR->wComps_SOF = (WORD)(*lpTemp++ & (BYTE)0xFF);

	// ** We don't support files in which the image height is initially specified as 0 and 
	// ** is later redefined by DNL. As long as we have to check that ,might as well have a
	// ** general sanity check.
	if(lpJDVAR->wWidth == 0 || lpJDVAR->wDepth == 0 || byBitPrec != (BYTE)0x08)
	{
		// ** only support 8 bits
		GlobalUnlock(hTemp);
		iReturn = NOTSUPPORT_16PRECISION;
		return iReturn;
	}

	for(wi=0; wi<lpJDVAR->wComps_SOF;wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];
		lpCompPtr->wComponent_Index = wi;
		lpCompPtr->wComponent_ID = (WORD)(*lpTemp++ & (BYTE)0xFF);
		byChar = *lpTemp++;
		lpCompPtr->wHor_SampleFactor = (WORD)((byChar >> 4) & (BYTE)0x0F);
		lpCompPtr->wVer_SampleFactor = (WORD)(byChar & (BYTE)0x0F);
		lpCompPtr->wQuant_TBL_No = (WORD)(*lpTemp++ &(BYTE)0xFF);
	}

	iReturn = 0;
	gbGray = FALSE;
	switch(lpJDVAR->wComps_SOF)
	{
	case 1:
		{
			lpJDVAR->ColorSystem = CS_GRAY;
			gbGray = TRUE;
			break;
		}
	case 3:
		{
			wID0 = lpJDVAR->Component[0].wComponent_ID;
			wID1 = lpJDVAR->Component[1].wComponent_ID;
			wID2 = lpJDVAR->Component[2].wComponent_ID;
			if(wID0 == 1 && wID1 == 2 && wID2 == 3)
				lpJDVAR->ColorSystem = CS_YCbCr;
			if(wID0 == 1 && wID1 == 4 && wID2 == 5)
			{
				lpJDVAR->ColorSystem = CS_YIQ;
				iReturn = NOTSUPPORT_YIQ;
			}
			break;
		}
	case 4:
		{
			lpJDVAR->ColorSystem = CS_CMYK;
			iReturn = NOTSUPPORT_CMYK;
			break;
		}
	default:
		iReturn = NOSUCHSYSTEM;
	}
	GlobalUnlock(hTemp);
	return iReturn;
}
			

int CJPGDMark::iRead_DHT(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	HANDLE hHuffTBL;
	LPJPGD_HUFF_TBL lpHuffPtr;
	LPSTR lpTemp;
	WORD wMarkerLen, wTBL_Ndx, wTotalBits, wi;

	m_SrcFile->Read((LPSTR)&wMarkerLen,2);
	m_utility.vSwapByte((LPSTR)&wMarkerLen,1);
	// ** wMarkerLen include itself
	wMarkerLen -= 2;
	lpTemp = (LPSTR)GlobalLock(hTemp);
	m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);

	while(wMarkerLen > 0)
	{
		wTBL_Ndx = (WORD)(*lpTemp++ & (BYTE)0xFF);
		if(wTBL_Ndx & (BYTE)0x10) //AC所用HUFF表
		{
			wTBL_Ndx -= (WORD)0x10;
			hHuffTBL = lpJDVAR->hAC_HuffTBL[wTBL_Ndx];
		}
		else//DC所用HUFF表
		{
			hHuffTBL = lpJDVAR->hDC_HuffTBL[wTBL_Ndx];
		}
		lpHuffPtr = (LPJPGD_HUFF_TBL)GlobalLock(hHuffTBL);
		lpHuffPtr->byBits[0] = 0;
		wTotalBits = 0;
		for(wi=1; wi<=16 ;wi++)
		{
			lpHuffPtr->byBits[wi] = *lpTemp++;
			wTotalBits += (WORD)lpHuffPtr->byBits[wi];
		}
		for(wi=0; wi<wTotalBits; wi++)
		{
			lpHuffPtr->byHuffVal[wi] = *lpTemp++;
		}

		GlobalUnlock(hHuffTBL);
		wMarkerLen -= (wTotalBits + 17); // 17 = 1 + 16;
	}
	GlobalUnlock(hTemp);
	return 0;
}

	
int CJPGDMark::iRead_SOS(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp)
{
	LPJPG_COMP_INFO lpCompPtr;
	LPSTR lpTemp;
	WORD wMarkerLen;
	WORD wi, wj;
	WORD wSOS_Comp_ID;
	BYTE byChar;


	m_SrcFile->Read((LPSTR)&wMarkerLen,2);
	m_utility.vSwapByte((LPSTR)&wMarkerLen,1);
	// ** wMarkerLen include itself
	wMarkerLen -= 2;
	lpTemp = (LPSTR)GlobalLock(hTemp);
	m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);

	lpJDVAR->wComps_SOS = (WORD) (*lpTemp++ & (BYTE)0xFF);
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		wSOS_Comp_ID = (WORD)(*lpTemp++ & (BYTE)0xFF);
		byChar = *lpTemp++;
		for(wj=0; wj<lpJDVAR->wComps_SOF;wj++)
		{
			if(wSOS_Comp_ID == lpJDVAR->Component[wj].wComponent_ID)
				break;
		}
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wj];
		lpCompPtr->wDC_TBL_No = (WORD) ((byChar >> 4) & (BYTE)0x0F);
		lpCompPtr->wAC_TBL_No = (WORD)(byChar & (BYTE)0x0F);
	}
	GlobalUnlock(hTemp);
	return 0;
}


int CJPGDMark::iReadJPGHeader(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR)
{
	HANDLE hTemp;
	LPSTR lpTemp;
	WORD wMarkerLen;
	BYTE byMarker;
	hTemp = GlobalAlloc(GHND,MAX_BUFF_SIZE);
	// ** 将两个标志位初始化。
	lpJDVAR->Is16 = FALSE;
	lpJDVAR->IsJetSoft = FALSE;

	iReturn = 0;
	while(TRUE)
	{
		// ** Repeat if it was a staffed 0xFF,0x00
		do{
			// ** Skip any non --FF bytes
			do
			{
				m_SrcFile->Read((LPSTR)&byMarker,1);
			} while(byMarker != (BYTE)0xFF);

			// ** Skip any duplicate FFs
			do
			{
				m_SrcFile->Read((LPSTR)&byMarker,1);
			} while(byMarker == (BYTE)0xFF);
		} while(byMarker == (BYTE)0x00);

		switch(byMarker)
		{
		case M_APP0:
			iReturn = iRead_App0(m_SrcFile,lpJDVAR,hTemp);
			break;
		case M_APP1:
			iReturn = iRead_App1(m_SrcFile,lpJDVAR,hTemp);
			break;
		case M_APP2:
			iReturn = iRead_App2(m_SrcFile,lpJDVAR,hTemp);
			break;
		case M_DQT:
			iReturn = iRead_DQT(m_SrcFile,lpJDVAR,hTemp);
			break;
		case M_DRI:
			iReturn = iRead_DRI(m_SrcFile,lpJDVAR,hTemp);
			break;
		case M_SOF0:
		case M_SOF1:
			iReturn = iRead_SOF(m_SrcFile,lpJDVAR,hTemp);
			break;
		case M_SOF2: //add by yaka 20010525
		case M_SOF3:
			iReturn = NOTSUPPORT_DCT;
			break;
		case M_DHT:
			iReturn = iRead_DHT(m_SrcFile,lpJDVAR,hTemp);
			break;
		case M_SOS:
			iReturn = iRead_SOS(m_SrcFile,lpJDVAR,hTemp);
			break;
		default:
			{
				// ** Skip unsupported marker
				m_SrcFile->Read((LPSTR)&wMarkerLen,2);
				m_utility.vSwapByte((LPSTR)&wMarkerLen,1);

				// ** wMarkerLen include itself
				wMarkerLen -= 2;
				lpTemp = (LPSTR)GlobalLock(hTemp);
				m_SrcFile->Read((LPSTR)lpTemp,wMarkerLen);
				GlobalUnlock(hTemp);

			}
		}
		if(iReturn || (byMarker == M_SOS)) //SOS是一个scan中最后一个标记；
		{
			GlobalFree(hTemp);
			return iReturn;
		}
	}
}



