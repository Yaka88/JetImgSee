// JPGDInit.cpp: implementation of the CJPGDInit class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDinit.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码初始化的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGDInit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGDInit::CJPGDInit()
{

}

CJPGDInit::~CJPGDInit()
{

}

extern int iReturn;

int CJPGDInit::iDecode_Init(LPJPGD_VAR lpJDVAR)
{
	DWORD dwTemp;
	WORD wi;
	dwTemp = (DWORD)sizeof(JPGD_HUFF_TBL);
	for(wi=0; wi<NUM_HUFF_TABLES; wi++)
	{
		lpJDVAR->hDC_HuffTBL[wi] = GlobalAlloc(GHND, dwTemp);
		lpJDVAR->hAC_HuffTBL[wi] = GlobalAlloc(GHND,dwTemp);
	}
	// ** Initial default values
	lpJDVAR->wXDensity = 1;
	lpJDVAR->wYDensity = 1;
	lpJDVAR->byDensityUnit = (BYTE)0x00;
	lpJDVAR->wMaxHor_SampleFactor = 1;
	lpJDVAR->wMaxVer_SampleFactor = 1;
	lpJDVAR->wRestart_Interval = 0;
	lpJDVAR->wRestart_Num = 0;
	lpJDVAR->wRestart_Cnt = 0;
	lpJDVAR->bEOF = FALSE;
	lpJDVAR->lHuffBuff = 0;
	lpJDVAR->iHuffBits = 0;
	lpJDVAR->Is16 = FALSE;
	return 0;
}


void CJPGDInit::vDecode_Component_Init(LPJPGD_VAR lpJDVAR)
{
	LPJPG_COMP_INFO lpCompPtr;
	WORD wHorFactor;
	WORD wVerFactor;
	WORD wi;

	// ** Compute maximum sampling factors
	for(wi=0; wi<lpJDVAR->wComps_SOF; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];
		lpJDVAR->wMaxHor_SampleFactor = MAX(lpJDVAR->wMaxHor_SampleFactor, lpCompPtr->wHor_SampleFactor);
		lpJDVAR->wMaxVer_SampleFactor = MAX(lpJDVAR->wMaxVer_SampleFactor, lpCompPtr->wVer_SampleFactor);
	}
	// ** Compute logical subsampled dimensions of components
	for(wi=0; wi<lpJDVAR->wComps_SOF; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];
		wHorFactor = lpCompPtr->wHor_SampleFactor;
		wVerFactor = lpCompPtr->wVer_SampleFactor;
		lpCompPtr->wTrue_Comp_Width = (lpJDVAR->wWidth * wHorFactor +
										lpJDVAR->wMaxHor_SampleFactor -1 ) /
										lpJDVAR->wMaxHor_SampleFactor;
		lpCompPtr->wTrue_Comp_Depth = (lpJDVAR->wDepth * wVerFactor + 
										lpJDVAR->wMaxVer_SampleFactor - 1) /
										lpJDVAR->wMaxVer_SampleFactor;
	}
}


void CJPGDInit::vDecode_HuffTBL_Init(LPJPGD_VAR lpJDVAR)
{
	LPJPG_COMP_INFO lpCompPtr;
	WORD wDCFlag[NUM_HUFF_TABLES];
	WORD wACFlag[NUM_HUFF_TABLES];
	WORD wi;

	lpJDVAR->wRestart_Cnt = lpJDVAR->wRestart_Interval;
	for(wi=0; wi<NUM_HUFF_TABLES; wi++)
	{
		wDCFlag[wi] = 1;
		wACFlag[wi] = 1;
	}
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		lpCompPtr =(LPJPG_COMP_INFO)&lpJDVAR->Component[wi];
		// ** Computer derived values for Hufffman tables
		if(wDCFlag[lpCompPtr->wDC_TBL_No])
		{
			vDecode_Generate_HuffTBL(lpJDVAR->hDC_HuffTBL[lpCompPtr->wDC_TBL_No]);
			wDCFlag[lpCompPtr->wDC_TBL_No] = 0;
		}
		if(wACFlag[lpCompPtr->wAC_TBL_No])
		{
			vDecode_Generate_HuffTBL(lpJDVAR->hAC_HuffTBL[lpCompPtr->wAC_TBL_No]);
			wACFlag[lpCompPtr->wAC_TBL_No] = 0;
		}
		lpJDVAR->iPrev_DC_Val[wi] = 0;
	}
}



void CJPGDInit::vDecode_Generate_HuffTBL(HANDLE hHuffTBL)
{
	LPJPGD_HUFF_TBL lpHuffPtr;
	WORD wCode, wSizeNdx, wHuffCode[257], wHuffSize[257];
	register MYWORD iBits;
	register MYWORD iTemp;
	register int i;
	register int j;

	// ** Figure 1:Make table of Huffman code length for each symbol,note that this is code_length order.
	lpHuffPtr = (LPJPGD_HUFF_TBL)GlobalLock(hHuffTBL);
	iTemp = 0;
	for(i=1;i<=16;i++)
	{
		iBits = (MYWORD)lpHuffPtr->byBits[i];
		for(j=1; j<=iBits; j++)
			wHuffSize[iTemp++] = (WORD)i;
	}
	wHuffSize[iTemp] = 0;//将最后一位置0，表示结束.
	// ** Figure C.2 :generate the wCodes themselves
	// ** Note that this is in wCode_length order.
	wSizeNdx = wHuffSize[0];
	wCode = 0;
	iTemp = 0;
	while(wHuffSize[iTemp])
	{
		while(wHuffSize[iTemp] == wSizeNdx)
		{
			wHuffCode[iTemp++] = wCode;
			wCode++;
		}
		wCode <<= 1;
		wSizeNdx ++ ;
	}

	// ** Figure F.15: Generate decoding tables
	iTemp = 0;
	for(i=1; i<=16; i++)
	{
		iBits = (MYWORD)lpHuffPtr->byBits[i];
		if(iBits)
		{
			// ** byHuffVal[] index of 1st sym of code len i
			lpHuffPtr->iValPtr[i] = iTemp;

			// **Minium code of length i
			lpHuffPtr->wMinCode[i] = wHuffCode[iTemp];
			iTemp += iBits;
			// **Maxmum code of length i
			lpHuffPtr->lMaxCode[i] = (LONG)wHuffCode[iTemp-1];
		}
		else
			lpHuffPtr->lMaxCode[i] = -1L;
	}
	// ** Ensure huff_DECODE terminates
	lpHuffPtr->lMaxCode[17] = 0xFFFFFL;
	GlobalUnlock(hHuffTBL);
}


void CJPGDInit::vDecode_NonInterleaved_Init(LPJPGD_VAR lpJDVAR)
{
	LPJPG_COMP_INFO lpCompPtr;
	lpCompPtr =(LPJPG_COMP_INFO)&lpJDVAR->Component[0];
	lpCompPtr->wMCU_Width = 1;
	lpCompPtr->wMCU_Blocks = 1;
	lpCompPtr->wMCU_Depth = 1;
	
	// ** Compute physical dimensions of component
	lpCompPtr->wSubsampled_Width = m_utility.wRoundUp(lpCompPtr->wTrue_Comp_Width,DCT_SIZE);
	lpCompPtr->wSubsampled_Depth = m_utility.wRoundUp(lpCompPtr->wTrue_Comp_Depth,DCT_SIZE);
	// ** (>>3) means (/DCT_SIZE)
	lpJDVAR->wW_MCUs = lpCompPtr->wSubsampled_Width >> 3;
	lpJDVAR->wH_MCUs = lpCompPtr->wSubsampled_Depth >> 3;

	// ** Prepare array describing MCU composition
	lpJDVAR->wBlocks_in_MCU = 1;
	lpJDVAR->wBlock_Mapping_Comp[0] = 0;

}


void CJPGDInit::vDecode_Interleaved_Init(LPJPGD_VAR lpJDVAR)
{
	LPJPG_COMP_INFO lpCompPtr;
	WORD wBlocks, wi;
	// ** 可以把某方向取样因子看作是一个MCU中某方向上包含8象素单位的个数
	lpJDVAR->wW_MCUs = (lpJDVAR->wWidth + lpJDVAR->wMaxHor_SampleFactor * DCT_SIZE -1)/ //整幅图象水平方向上包含包含多少MCU；
						(lpJDVAR->wMaxHor_SampleFactor * DCT_SIZE);
	lpJDVAR->wH_MCUs = (lpJDVAR->wDepth + lpJDVAR->wMaxVer_SampleFactor * DCT_SIZE -1)/ //整幅图象垂直方向上包含包含多少MCU；
						(lpJDVAR->wMaxVer_SampleFactor * DCT_SIZE);

	lpJDVAR->wBlocks_in_MCU = 0;
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];

		// ** For interleaved scan,sampling factors
		// ** give # of blocks per component
		lpCompPtr->wMCU_Width = lpCompPtr->wHor_SampleFactor;
		lpCompPtr->wMCU_Depth = lpCompPtr->wVer_SampleFactor;
		lpCompPtr->wMCU_Blocks = lpCompPtr->wMCU_Width * lpCompPtr->wMCU_Depth;// 一个MCU中包含该元素的block(8*8个象素)的快数。

		// ** Compute physical dimensions of component (in whole image!)
		lpCompPtr->wSubsampled_Width = m_utility.wRoundUp(lpCompPtr->wTrue_Comp_Width,lpCompPtr->wMCU_Width * DCT_SIZE);
		lpCompPtr->wSubsampled_Depth = m_utility.wRoundUp(lpCompPtr->wTrue_Comp_Depth,lpCompPtr->wMCU_Depth * DCT_SIZE);

		// ** Prepare array describing MCU composition
		wBlocks = lpCompPtr->wMCU_Blocks;
		while(wBlocks--)
		{
			lpJDVAR->wBlock_Mapping_Comp[lpJDVAR->wBlocks_in_MCU++] = wi;
		}
	}
}
