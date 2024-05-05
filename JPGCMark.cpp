// JPGCMark.cpp: implementation of the CJPGCMark class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCmark.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码处理MARK的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGCMark.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGCMark::CJPGCMark()
{

}

CJPGCMark::~CJPGCMark()
{

}

extern int iReturn;

int CJPGCMark::iWriteJPGHeader(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{
	WORD wSOI_Mark;
	// ** Write SOI marker
	wSOI_Mark = (WORD)0xD8FF;
	m_DstFile->Write((LPSTR)&wSOI_Mark,2);
	iReturn = iWrite_APP0(m_DstFile,lpJCVAR);
	iReturn = iWrite_DQT(m_DstFile,lpJCVAR);
	// ** 本程序在JPEG编码时并未考虑重入间隔问题，也即不将图象分段，
	// ** 所以读取该JPEG文件时，也只能全部读取，不能部分读取；
	if(lpJCVAR->wRestart_Interval)
		iReturn = iWrite_DRI(m_DstFile,lpJCVAR);
	iReturn = iWrite_SOF(m_DstFile,lpJCVAR);
	iReturn = iWrite_DHT(m_DstFile,lpJCVAR);
	iReturn = iWrite_SOS(m_DstFile,lpJCVAR);
	
	return 0;
}

// ** 写APP0、APP1、APP2标记；
int CJPGCMark::iWrite_APP0(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{
	struct jpg_app0
	{
		BYTE byFill; 
		BYTE byMarker;
		WORD wMarkerLen;
		BYTE byString[5];
		WORD wVersion;
		BYTE byDensityUnit;
		WORD wXDensity;
		WORD wYDensity;
		BYTE byXThumbnail;
		BYTE byYThumbnail;
	} JPG_APP0;
	WORD wDataLen;
	wDataLen = 18 ; //sizeof(JPG_APP0);
	JPG_APP0.byMarker = M_APP0;
	JPG_APP0.byFill = (BYTE)0xFF;

	// ** (-2)because not include 0xF and M_APP0 marker
	JPG_APP0.wMarkerLen = wDataLen - 2;
	lstrcpy((char*)JPG_APP0.byString, "JFIF");
	JPG_APP0.wVersion = (WORD)0x0101;
	JPG_APP0.byDensityUnit = (BYTE)0;
	JPG_APP0.wXDensity = 1;
	JPG_APP0.wYDensity = 1;
	JPG_APP0.byXThumbnail = (BYTE)0;
	JPG_APP0.byYThumbnail = (BYTE)0;
	m_utility.vSwapByte((LPSTR)&JPG_APP0.wMarkerLen,1);
	m_utility.vSwapByte((LPSTR)&JPG_APP0.wVersion,1);
	m_utility.vSwapByte((LPSTR)&JPG_APP0.wXDensity,1);
	m_utility.vSwapByte((LPSTR)&JPG_APP0.wYDensity,1);
	// ** 采用m_DstFile->Write((LPSTR)&JPG_APP0,wDataLen)将产生错误,
	// ** 没有道理!
	m_DstFile->Write((LPSTR)&JPG_APP0.byFill,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.byMarker,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.wMarkerLen,2);
	m_DstFile->Write((LPSTR)&JPG_APP0.byString,5);
	m_DstFile->Write((LPSTR)&JPG_APP0.wVersion,2);
	m_DstFile->Write((LPSTR)&JPG_APP0.byDensityUnit,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.wXDensity,2);
	m_DstFile->Write((LPSTR)&JPG_APP0.wYDensity,2);
	m_DstFile->Write((LPSTR)&JPG_APP0.byXThumbnail,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.byYThumbnail,1);

	// ** 借用JPG_APP0标识图象是16位彩色还是24位彩色；
	JPG_APP0.byMarker = M_APP1;
	JPG_APP0.byFill = (BYTE)0xFF;
	JPG_APP0.wMarkerLen = 3; //2+1
	m_utility.vSwapByte((LPSTR)&JPG_APP0.wMarkerLen,1);
	char cTempFlag;
	if(lpJCVAR->Is16)
		cTempFlag = (char)0xFF;
	else 
		cTempFlag = (WORD)0;
	m_DstFile->Write((LPSTR)&JPG_APP0.byFill,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.byMarker,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.wMarkerLen,2);
	m_DstFile->Write((LPSTR)&cTempFlag,1);

	// ** 借用JPG_APP0标识图象是否为本软件压缩；
	JPG_APP0.byMarker = M_APP2;
	JPG_APP0.byFill = (BYTE)0xFF;
	JPG_APP0.wMarkerLen = 12; // 2+10 (IsJetSoft为9位);
	m_utility.vSwapByte((LPSTR)&JPG_APP0.wMarkerLen,1);
	char cTempRight[10] = "IsJetSoft";
	m_DstFile->Write((LPSTR)&JPG_APP0.byFill,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.byMarker,1);
	m_DstFile->Write((LPSTR)&JPG_APP0.wMarkerLen,2);
	m_DstFile->Write((LPSTR)cTempRight,10);

	return 0;
}


int CJPGCMark::iWrite_DQT(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{
	struct jpg_dqt
	{
		BYTE byFill;
		BYTE byMarker;
		WORD wMarkerLen;
	} JPG_DQT;

	LPJPG_QUANT_TBL lpQuantPtr;
	WORD wMarkerLen, wDataLen, wi;
	WORD wQuantUsed[NUM_QUANT_TABLES];
	BYTE byPrecFlag[NUM_QUANT_TABLES];
	BYTE byQuantData[DCT_SQUARE];
	BYTE byIndex;
	register int i;

	for(wi=0; wi<NUM_QUANT_TABLES; wi++)
	{
		wQuantUsed[wi] = 0;
		byPrecFlag[wi] = 0;
	}
	for(wi=0; wi<lpJCVAR->wComps_SOF; wi++)
		wQuantUsed[lpJCVAR->Component[wi].wQuant_TBL_No] = 1;
	for(wi=0; wi<NUM_QUANT_TABLES; wi++)
	{
		if(wQuantUsed[wi])
		{
			lpQuantPtr = lpJCVAR->QuantTBL[wi];
			for(i=0; i<DCT_SQUARE; i++)
			{	
				// ** 若byPrecFlag[i]值为1，则表明该量化表的数据精确度为16位（即量化表中有的数据值超过255）。
				if(lpQuantPtr[i] > 255)
				{
					byPrecFlag[wi] = 1;
					break;
				}
			}
		}
	}
	// ** Calculate marker length
	wMarkerLen = 0;
	for(wi=0; wi<NUM_QUANT_TABLES; wi++)
	{
		if(wQuantUsed[wi])
		{
			wMarkerLen += (DCT_SQUARE + 1);
			if(byPrecFlag[wi])
				wMarkerLen += DCT_SQUARE;
		}
	}

	// ** (+2)because include itself
	JPG_DQT.byFill = (BYTE)0xFF;
	JPG_DQT.byMarker = M_DQT;
	JPG_DQT.wMarkerLen = wMarkerLen + 2;
	m_utility.vSwapByte((LPSTR)&JPG_DQT.wMarkerLen,1);
	wDataLen = sizeof(JPG_DQT);
	m_DstFile->Write((LPSTR)&JPG_DQT,wDataLen);
	for(wi=0; wi< NUM_QUANT_TABLES; wi++)
	{
		if(wQuantUsed[wi])
		{
			// ** 若byPrecFlag[i]值为1，则表明该量化表的数据精确度为16位（即量化表中有的数据值超过255）。
			byIndex = (BYTE)(wi + (WORD)((byPrecFlag[wi] << 4) & (BYTE)0xFF));
			m_DstFile->Write((LPSTR)&byIndex, 1);//写入(Pq,Tq)
			lpQuantPtr = lpJCVAR->QuantTBL[wi];
			if(byPrecFlag[wi])
			{
				m_utility.vSwapByte((LPSTR)lpQuantPtr,(WORD)DCT_SQUARE);
				wDataLen = DCT_SQUARE << 1;
				m_DstFile->Write((LPSTR)lpQuantPtr,wDataLen);
				m_utility.vSwapByte((LPSTR)lpQuantPtr,(WORD)DCT_SQUARE);
			}
			else
			{
				for(i=0; i< DCT_SQUARE; i++)
				{
					// **  数据结构JPG_QUANT_TBL中的元素是整形(16位)，这是因为量化表中的数据有可能超过255。
					byQuantData[i] = (BYTE)lpQuantPtr[i];
				}
				wDataLen = DCT_SQUARE;
				m_DstFile->Write((LPSTR)byQuantData, wDataLen);
			}
		}
	}
	return 0;
}


int CJPGCMark::iWrite_DRI(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{

	struct jpg_dri
	{
		BYTE byFill;
		BYTE byMarker;
		WORD wMarkerLen;
		WORD wInterval;
	} JPG_DRI;
	WORD wDataLen;

	JPG_DRI.byFill = (BYTE)0xFF;
	JPG_DRI.byMarker = M_DRI;
	JPG_DRI.wMarkerLen = 4;
	JPG_DRI.wInterval = lpJCVAR->wRestart_Interval;
	m_utility.vSwapByte((LPSTR)&JPG_DRI.wMarkerLen,1);
	m_utility.vSwapByte((LPSTR)&JPG_DRI.wInterval,1);
	wDataLen = sizeof(JPG_DRI);
	m_DstFile->Write((LPSTR)&JPG_DRI,wDataLen);
	return 0;
}


int CJPGCMark::iWrite_SOF(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{
	struct jpg_sof
	{
		BYTE byFill;
		BYTE byMarker;
		WORD wMarkerLen;
		BYTE byBitPrec;
		WORD wDepth;
		WORD wWidth;
		BYTE byComponent;
	} JPG_SOF;
	struct jpg_sof_tmp
	{
		BYTE byComponent_ID;
		BYTE bySampleFactor;
		BYTE byQuant_TBL_No;
	} JPG_SOF_TMP;
	LPJPG_COMP_INFO lpCompPtr;
	WORD wDataLen, wi;

	JPG_SOF.byFill = (BYTE)0xFF;
	JPG_SOF.byMarker = M_SOF0;
	JPG_SOF.wMarkerLen = 3 * lpJCVAR->wComps_SOF + 8;
	JPG_SOF.byBitPrec = 8;
	JPG_SOF.wDepth = lpJCVAR->wDepth;
	JPG_SOF.wWidth = lpJCVAR->wWidth;
	JPG_SOF.byComponent = (BYTE)lpJCVAR->wComps_SOF;
	m_utility.vSwapByte((LPSTR)&JPG_SOF.wMarkerLen,1);
	m_utility.vSwapByte((LPSTR)&JPG_SOF.wDepth,1);
	m_utility.vSwapByte((LPSTR)&JPG_SOF.wWidth,1);
	wDataLen = sizeof(JPG_SOF);
	//m_DstFile->Write((LPSTR)&JPG_SOF,wDataLen);
	m_DstFile->Write((LPSTR)&JPG_SOF.byFill,1);
	m_DstFile->Write((LPSTR)&JPG_SOF.byMarker,1);
	m_DstFile->Write((LPSTR)&JPG_SOF.wMarkerLen,2);
	m_DstFile->Write((LPSTR)&JPG_SOF.byBitPrec,1);
	m_DstFile->Write((LPSTR)&JPG_SOF.wDepth,2);
	m_DstFile->Write((LPSTR)&JPG_SOF.wWidth,2);
	m_DstFile->Write((LPSTR)&JPG_SOF.byComponent,1);

	for(wi=0; wi<lpJCVAR->wComps_SOF; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];
		JPG_SOF_TMP.byComponent_ID = (BYTE)lpCompPtr->wComponent_ID;
		JPG_SOF_TMP.bySampleFactor = (BYTE)((lpCompPtr->wHor_SampleFactor << 4) + lpCompPtr->wVer_SampleFactor);
		JPG_SOF_TMP.byQuant_TBL_No = (BYTE)lpCompPtr->wQuant_TBL_No;
		wDataLen = sizeof(JPG_SOF_TMP);
		m_DstFile->Write((LPSTR)&JPG_SOF_TMP,wDataLen);
	}
	return 0;
}


int CJPGCMark::iWrite_DHT(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{
	struct jpg_dht
	{
		BYTE byFill;
		BYTE byMarker;
		WORD wMarkerLen;
	} JPG_DHT;
	HANDLE hHuffTBL;
	LPJPGC_HUFF_TBL lpHuffPtr;
	LPSTR lpTemp;
	WORD wDCUsed[NUM_HUFF_TABLES];
	WORD wACUsed[NUM_HUFF_TABLES];
	WORD wDCBits[NUM_HUFF_TABLES];
	WORD wACBits[NUM_HUFF_TABLES];
	WORD wMarkerLen, wDataLen, wi, wj;
	BYTE byIndex;

	for(wi=0; wi<NUM_HUFF_TABLES; wi++)
	{
		wDCUsed[wi] = 0;
		wACUsed[wi] = 0;
	}
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
	{
		wDCUsed[lpJCVAR->Component[wi].wDC_TBL_No] = 1;
		wACUsed[lpJCVAR->Component[wi].wAC_TBL_No] = 1;
	}
	// ** Calculate marker length，lpHuffPtr->byBits[wj]已经填过值；
	wMarkerLen = 0;
	for(wi=0; wi<NUM_HUFF_TABLES; wi++)
	{
		if(wDCUsed[wi])
		{
			hHuffTBL = lpJCVAR->hDC_HuffTBL[wi];
			lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(hHuffTBL);
			wDCBits[wi] = 0;
			for(wj=1; wj<=16; wj++)
				wDCBits[wi] += (WORD)(lpHuffPtr->byBits[wj] & (BYTE)0xFF); //计算总共有多少个HUFF码，这里为12个；
			// ** 17 means (1+16)
			wMarkerLen += (wDCBits[wi] + 17);
			GlobalUnlock(hHuffTBL);
		}
		if(wACUsed[wi])
		{
			hHuffTBL = lpJCVAR->hAC_HuffTBL[wi];
			lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(hHuffTBL);
			wACBits[wi] = 0;
			for(wj=1; wj<=16; wj++)
				wACBits[wi] += (WORD)(lpHuffPtr->byBits[wj] & (BYTE)0xFF); //计算总共有多少个HUFF码，这里为162个；
			// ** 17 means (1+16)
			wMarkerLen += (wACBits[wi] + 17);
			GlobalUnlock(hHuffTBL);
		}
	}
	// ** (+2)because include itself
	JPG_DHT.byFill  = (BYTE)0xFF;
	JPG_DHT.byMarker = M_DHT;
	JPG_DHT.wMarkerLen = wMarkerLen + 2;
	m_utility.vSwapByte((LPSTR)&JPG_DHT.wMarkerLen,1);
	wDataLen = sizeof(JPG_DHT);
	m_DstFile->Write((LPSTR)&JPG_DHT,wDataLen);

	for(wi=0; wi<NUM_HUFF_TABLES; wi++)
	{
		if(wDCUsed[wi])
		{

			byIndex = (BYTE)wi;// 写入（Tc,Th),0x00表示DC所使用的表格；
			m_DstFile->Write((LPSTR)&byIndex,1);
			hHuffTBL = lpJCVAR->hDC_HuffTBL[wi];
			lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(hHuffTBL);
			m_DstFile->Write((LPSTR)&lpHuffPtr->byBits[1],16);
			lpTemp = (char*)&lpHuffPtr->byHuffVal[0]; //lpHuffPtr->byHuffVal[]已经填过值；
			m_DstFile->Write((LPSTR)lpTemp,wDCBits[wi]);
			GlobalUnlock(hHuffTBL);
		}
		if(wACUsed[wi])
		{
			byIndex = (BYTE)(wi+(WORD)0x10);// 写入（Tc,Th),0x10表示AC所使用的表格；
			m_DstFile->Write((LPSTR)&byIndex,1);
			hHuffTBL = lpJCVAR->hAC_HuffTBL[wi];
			lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(hHuffTBL);
			m_DstFile->Write((LPSTR)&lpHuffPtr->byBits[1],16);
			lpTemp = (char*)&lpHuffPtr->byHuffVal[0];
			m_DstFile->Write((LPSTR)lpTemp,wACBits[wi]);
			GlobalUnlock(hHuffTBL);
		}
	}
	return 0;
}



int CJPGCMark::iWrite_SOS(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{
	struct jpg_sos
	{
		BYTE byFill;
		BYTE byMarker;
		WORD wMarkerLen;
		BYTE byComps_SOS;
	} JPG_SOS;

	struct jpg_sos_tmp1
	{
		BYTE byComponent_ID;
		BYTE byHuf_TBL_No;
	} JPG_SOS_TMP1;

	struct jpg_sos_tmp2
	{
		BYTE bySpectralBgn;
		BYTE bySpectralEnd;
		BYTE byStep;
	} JPG_SOS_TMP2;
	LPJPG_COMP_INFO lpCompPtr;
	WORD wi;
	
	JPG_SOS.byFill  = (BYTE)0xFF;
	JPG_SOS.byMarker = M_SOS;
	JPG_SOS.wMarkerLen = (lpJCVAR->wComps_SOS << 1) + 6;  // Ls、Ns、Ss 、Se和(Ah,Al)的长度为6；
	JPG_SOS.byComps_SOS = (BYTE)lpJCVAR->wComps_SOS;
	m_utility.vSwapByte((LPSTR)&JPG_SOS.wMarkerLen,1);
	//m_DstFile->Write((LPSTR)&JPG_SOS,sizeof(JPG_SOS));
	m_DstFile->Write((LPSTR)&JPG_SOS.byFill,1);
	m_DstFile->Write((LPSTR)&JPG_SOS.byMarker,1);
	m_DstFile->Write((LPSTR)&JPG_SOS.wMarkerLen,2);
	m_DstFile->Write((LPSTR)&JPG_SOS.byComps_SOS,1);

	// ** 写入CSns和(TDns,TAns)
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];
		JPG_SOS_TMP1.byComponent_ID = (BYTE)lpCompPtr->wComponent_ID;
		JPG_SOS_TMP1.byHuf_TBL_No = (BYTE)((lpCompPtr->wDC_TBL_No << 4) + lpCompPtr->wAC_TBL_No);
		m_DstFile->Write((LPSTR)&JPG_SOS_TMP1,sizeof(JPG_SOS_TMP1));
	}

	// ** 写入Ss ,Se和(Ah,Al)
	JPG_SOS_TMP2.bySpectralBgn = (BYTE)0x00;
	JPG_SOS_TMP2.bySpectralEnd = (BYTE)(DCT_SQUARE - 1);
	JPG_SOS_TMP2.byStep = (BYTE)0x00;
	m_DstFile->Write((LPSTR)&JPG_SOS_TMP2,sizeof(JPG_SOS_TMP2));
	return 0;
}




