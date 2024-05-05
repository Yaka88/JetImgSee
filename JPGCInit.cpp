// JPGInit.cpp: implementation of the CJPGInit class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCinit.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码初始化的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGCInit.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGInit::CJPGInit()
{
	YUV_Sample = YUV_411;//或422，444不支持！
    //gwQuality = 12;//400;//100;//50;//30;//24;//最初为12(不能为0);
}

CJPGInit::~CJPGInit()
{

}

extern int iReturn;

int CJPGInit::iEncode_Init(LPJPGC_VAR lpJCVAR)
{
	DWORD dwTemp;
	WORD wi;

	lpJCVAR->hEncodeBuff = GlobalAlloc(GHND, (DWORD)MAX_BUFF_SIZE);
	lpJCVAR->lpDataBuff = (LPSTR)GlobalLock(lpJCVAR->hEncodeBuff);
	dwTemp = (DWORD)sizeof(JPGC_HUFF_TBL);
	for(wi=0; wi<NUM_HUFF_TABLES; wi++)
	{
		lpJCVAR->hDC_HuffTBL [wi] = GlobalAlloc(GHND,dwTemp);
		lpJCVAR->hAC_HuffTBL [wi] = GlobalAlloc(GHND,dwTemp);
	}
	/* Initial default values */
	lpJCVAR->wXDensity = 1;
	lpJCVAR->wYDensity = 1;
	lpJCVAR->byDensityUnit = (BYTE)0x00;
	lpJCVAR->wMaxHor_SampleFactor = 1;
	lpJCVAR->wMaxVer_SampleFactor = 1;

	lpJCVAR->wRestart_Interval = 0;
	lpJCVAR->wRestart_Num = 0;
	lpJCVAR	->wRestart_Cnt = 0;
	lpJCVAR->dwBuffCnt = 0;
	lpJCVAR->lHuffBuff = 0;
	lpJCVAR->iHuffBits = 0;

	vEncode_Component_Init(lpJCVAR); //初始化三个成分的一些数据结构。
	vEncode_QuantTBL_Init(lpJCVAR);// 创建两个量化表。
	vEncode_HuffTBL_Init(lpJCVAR); // 创建四个HUFF表。

	if(lpJCVAR->wComps_SOS == 1)
		vEncode_NonInterleaved_Init(lpJCVAR);
	else
		vEncode_Interleaved_Init(lpJCVAR);
	return 0;
		
}

void CJPGInit::vEncode_Component_Init(LPJPGC_VAR lpJCVAR)
{
	LPJPG_COMP_INFO lpCompPtr;
	WORD wHorFactor, wVerFactor, wi;

	if(gbGray)
	{
		lpJCVAR->wComps_SOF = 1;
		lpJCVAR->ColorSystem = CS_GRAY;
		lpJCVAR->Component [0].wComponent_Index = 0;
		lpJCVAR->Component [0].wComponent_ID = 1;
		// ** Set single component to 1x1 subsampling 
		lpJCVAR->Component[0].wHor_SampleFactor =1;
		lpJCVAR->Component[0].wVer_SampleFactor = 1;
		lpJCVAR->Component[0].wQuant_TBL_No = 0;
		lpJCVAR->Component[0].wDC_TBL_No = 0;
		lpJCVAR->Component[0].wAC_TBL_No = 0;
	}
	else
	{
		lpJCVAR->wComps_SOF = 3;//三个元素；
		lpJCVAR->ColorSystem = CS_YCbCr;
		wVerFactor = (YUV_Sample == YUV_411) ? 2 : 1;//垂直取样因子变化，但水平取样因子固定为2（都是针对亮度成分而言）。

		// ** JFIF specifies IDs 1,2,3 
		lpJCVAR->Component[0].wComponent_Index = 0;
		lpJCVAR->Component [0].wComponent_ID = 1;
		lpJCVAR->Component[0].wHor_SampleFactor = 2;
		lpJCVAR->Component[0].wVer_SampleFactor = wVerFactor;

		// ** Use tables 0 for chrominance 
		lpJCVAR->Component[0].wQuant_TBL_No = 0;
		lpJCVAR->Component[0].wDC_TBL_No = 0;
		lpJCVAR->Component[0].wAC_TBL_No = 0;

		// ** JFIF specifies IDs 1,2,3 
		lpJCVAR->Component[1].wComponent_Index = 1;
		lpJCVAR->Component [1].wComponent_ID = 2;
		lpJCVAR->Component[1].wHor_SampleFactor = 1;
		lpJCVAR->Component[1].wVer_SampleFactor = 1;

		// ** Use tables 1 for chrominance 
		lpJCVAR->Component[1].wQuant_TBL_No = 1;
		lpJCVAR->Component[1].wDC_TBL_No = 1;
		lpJCVAR->Component[1].wAC_TBL_No = 1;
		
		// ** JFIF specifies IDs 1,2,3 /
		lpJCVAR->Component[2].wComponent_Index = 2;
		lpJCVAR->Component [2].wComponent_ID = 3;
		lpJCVAR->Component[2].wHor_SampleFactor = 1;
		lpJCVAR->Component[2].wVer_SampleFactor = 1;

		// ** Use tables 2 for chrominance 
		// ** 两个色度成分公用一个Quant_Table(量化表)和AC_Table，DC_Table(HUFF表?).
		lpJCVAR->Component[2].wQuant_TBL_No = 1;
		lpJCVAR->Component[2].wDC_TBL_No = 1;
		lpJCVAR->Component[2].wAC_TBL_No = 1;
	}
	lpJCVAR->wComps_SOS = lpJCVAR->wComps_SOF ;

	// ** Computer maximum sampling factors  
	for(wi=0; wi<lpJCVAR->wComps_SOF; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];
		lpJCVAR->wMaxHor_SampleFactor = MAX(lpJCVAR->wMaxHor_SampleFactor,lpCompPtr->wHor_SampleFactor);
		lpJCVAR->wMaxVer_SampleFactor = MAX(lpJCVAR->wMaxVer_SampleFactor,lpCompPtr->wVer_SampleFactor);
	}

	//  ** Computer logical subsampled dimensions of components 
	for(wi=0; wi<lpJCVAR->wComps_SOF; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];
		wHorFactor = lpCompPtr->wHor_SampleFactor;
		wVerFactor = lpCompPtr->wVer_SampleFactor;
		lpCompPtr->wTrue_Comp_Width = (lpJCVAR->wWidth * wHorFactor + lpJCVAR->wMaxHor_SampleFactor - 1) /
									  lpJCVAR->wMaxHor_SampleFactor;
		lpCompPtr->wTrue_Comp_Depth = (lpJCVAR->wDepth * wVerFactor + lpJCVAR->wMaxVer_SampleFactor - 1) /
									  lpJCVAR->wMaxVer_SampleFactor;
	}
}


void CJPGInit::vEncode_QuantTBL_Init(LPJPGC_VAR lpJCVAR)
{
	MYWORD iQuality, i;

	// ** 下面两个表已经按Zigzag(之字型)顺序排列；
	MYWORD iLuminanceTBL[DCT_SQUARE] = {
		16,  11,  12,  14,  12, 10, 16,   14,
		13,  14,  18,  17,  16, 19,  24,  20,
		26,  24,  22,  22,  24, 49,  35,  37,
		29,  40,  58,  51,  61, 60,  57,  51,
		56,  55,  64,  72,  92, 78,  64,  48,
		87,  69,  55,  56,  80, 109, 81,  87,
		95,	 98,  103, 104, 103, 62, 77,  113,
		121, 112, 100, 120, 92, 101, 103, 99
	};

	MYWORD iChrominanceTBL[DCT_SQUARE] = {
		17, 18, 18, 24, 21, 24, 47, 26,
		26, 47, 99, 66, 56, 66, 99, 99,
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
	};
	// ** Convert a user-specified quality rating to a percentage 
	// ** scaling factor for an underlying quantization table , using
	// ** our recommended scaling curve. The quality factor should be
	// ** 1(very good) to 100(terrible).
	iQuality = (MYWORD)(pow((double)gwQuality, (double)0.6) + (double)0.5);
	// ** Set up tow quantization tables using the specified quality scaling.
	for(i=0; i<DCT_SQUARE; i++)
	{
		lpJCVAR->QuantTBL[0][i] = (iLuminanceTBL[i] * iQuality) >> 3;
		lpJCVAR->QuantTBL[1][i] = (iChrominanceTBL[i] * iQuality) >> 3;
	}
}


void CJPGInit::vEncode_HuffTBL_Init(LPJPGC_VAR lpJCVAR)
{
	LPJPGC_HUFF_TBL lpHuffPtr;
	LPJPG_COMP_INFO lpCompPtr;
	WORD wDCFlag[4];
	WORD wACFlag[4];
	WORD wi;
	BYTE byDC_Luminance_Bits[17] = {//0 - base
		0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0 };
	BYTE byDC_Luminance_Val[12] = 
		{ 0,1,2,3,4,5,6,7,8,9,10,11 };
	BYTE byDC_Chrominance_Bits[17] = { //0 - base
		0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 };
	BYTE byDC_Chrominance_Val[12] = 
		{ 0,1,2,3,4,5,6,7,8,9,10,11 };
	BYTE byAC_Luminance_Bits[17] = {//0 - base
		0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7D };//加起来共162个
	BYTE byAC_Luminance_Val[256] = {
		0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,	//相当于0/1，0/2，0/3，0/0 0/4....,他们对应的HUFF码则按顺序排列，即码的长度从少到多，同一长度内，按从小到大顺序排列
		0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,	//相当于2/1，3/1，4/1，0/6 1/3....,具体可参看相应资料。
		0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08,
		0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,
		0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,
		0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,
		0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,
		0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
		0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
		0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
		0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
		0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
		0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,
		0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
		0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,
		0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,
		0xC6,0xC7,0xD8,0xC9,0xCA,0xD2,0xD3,0xD4,
		0xD5,0xD6,0xC7,0xD8,0xD9,0xDA,0xE1,0xE2,
		0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,
		0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };//共162个


	BYTE byAC_Chrominance_Bits[17] = {//0 - base
	    0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77 };//加起来共162个
	BYTE byAC_Chrominance_Val[256] = {
		0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,
		0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,
		0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
		0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,
		0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,
		0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26,
		0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,
		0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,
		0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,
		0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,
		0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,
		0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,
		0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,
		0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,
		0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,
		0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,
		0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,
		0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,
		0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,
		0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };//共162个

	// ** Set up the standard Huffman tables(cf.JPEG standard section k.3)
	// ** IMPORTANT: these are only valid for 8-bit data precision!
	lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(lpJCVAR->hDC_HuffTBL[0]);
	memcpy(lpHuffPtr->byBits, byDC_Luminance_Bits, sizeof(byDC_Luminance_Bits));
	memcpy(lpHuffPtr->byHuffVal, byDC_Luminance_Val, sizeof(byDC_Luminance_Val));
	GlobalUnlock(lpJCVAR->hDC_HuffTBL[0]);

	lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(lpJCVAR->hDC_HuffTBL[1]);
	memcpy(lpHuffPtr->byBits, byDC_Chrominance_Bits, sizeof(byDC_Chrominance_Bits));
	memcpy(lpHuffPtr->byHuffVal, byDC_Chrominance_Val, sizeof(byDC_Chrominance_Val));
	GlobalUnlock(lpJCVAR->hDC_HuffTBL[1]);

	lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(lpJCVAR->hAC_HuffTBL[0]);
	memcpy(lpHuffPtr->byBits, byAC_Luminance_Bits, sizeof(byAC_Luminance_Bits));
	memcpy(lpHuffPtr->byHuffVal, byAC_Luminance_Val, sizeof(byAC_Luminance_Val));
	GlobalUnlock(lpJCVAR->hAC_HuffTBL[0]);

	lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(lpJCVAR->hAC_HuffTBL[1]);
	memcpy(lpHuffPtr->byBits, byAC_Luminance_Bits, sizeof(byAC_Luminance_Bits));
	memcpy(lpHuffPtr->byHuffVal, byAC_Luminance_Val, sizeof(byAC_Luminance_Val));
	GlobalUnlock(lpJCVAR->hAC_HuffTBL[1]);

	lpJCVAR->wRestart_Cnt = lpJCVAR->wRestart_Interval;
	for(wi=0; wi<4;wi++)
	{
		wDCFlag[wi] = 1;
		wACFlag[wi] = 1;
	}
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)// 填充四个HUFF表的每个码的具体数值和长度
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];
		// **  computer derived values for Huffman tables
		if(wDCFlag[lpCompPtr->wDC_TBL_No])// 针对亮度和色度的DC成份的HUFF表。
		{
			vEncode_Generate_HuffTBL(lpJCVAR->hDC_HuffTBL[lpCompPtr->wDC_TBL_No]);
			wDCFlag[lpCompPtr->wDC_TBL_No] = 0;
		}
		if(wACFlag[lpCompPtr->wAC_TBL_No])// 针对亮度和色度的AC成份的HUFF表。
		{
			vEncode_Generate_HuffTBL(lpJCVAR->hAC_HuffTBL[lpCompPtr->wAC_TBL_No]);
			wACFlag[lpCompPtr->wAC_TBL_No] = 0;
		}
		// ** Initialize DC predictions to zero
		lpJCVAR->iPrev_DC_Val[wi] = 0;
	}
}


void CJPGInit::vEncode_Generate_HuffTBL(HANDLE hHuffTBL)
{
	LPJPGC_HUFF_TBL lpHuffPtr;
	WORD wCode, wSizeNdx;
	WORD wHuffCode[257];
	WORD wHuffSize[257];
	MYWORD iBits;
	MYWORD iTemp;
	MYWORD iLast;
	register int i, j;

	// ** Figure C.1: make table of Huffman code length for
	// ** each symbol, note that this is in code_length order
	lpHuffPtr = (LPJPGC_HUFF_TBL)GlobalLock(hHuffTBL);
	iTemp = 0;
	for(i=1; i<=16; i++)
	{
		iBits = (int)lpHuffPtr->byBits[i];
		for(j=1; j<=iBits; j++)
			wHuffSize[iTemp++] = (WORD)i;
	}
	wHuffSize[iTemp] = 0;//将最后一位置0，表示结束。
	iLast = iTemp;

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
	// ** Figure C.3: generat encoding tables.These are code and size indexed by symbol
	// ** value. Set any codeless symbols to have code length 0; this allows emit_bits to 
	// ** detect any attempt to emit such symbols.
	for(i=0; i<iLast; i++)
	{	// ** 一定要用iTemp变量过度，即lpHuffPtr->wHuffCode[iTemp]不能
		// **用lpHuffPtr->wHuffCode[i]代替，原因可参照byAC_Chrominance_Val[256]或byDC_Chrominance_Val[256]的排列顺序。
		iTemp = (MYWORD)(lpHuffPtr->byHuffVal[i] & (BYTE)0xFF);
		lpHuffPtr->wHuffCode[iTemp] = wHuffCode[i];
		lpHuffPtr->wHuffSize[iTemp] = wHuffSize[i];
	}
	GlobalUnlock(hHuffTBL);
}


void CJPGInit::vEncode_NonInterleaved_Init(LPJPGC_VAR lpJCVAR)
{
	LPJPG_COMP_INFO lpCompPtr;
	lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[0];
	// ** For noninterleaved scan, always one block per MCU
	lpCompPtr->wMCU_Width = 1;
	lpCompPtr->wMCU_Depth = 1;
	lpCompPtr->wMCU_Blocks = 1;

	// ** Compute physical dimensions of component
	lpCompPtr->wSubsampled_Width = m_utility.wRoundUp(lpCompPtr->wTrue_Comp_Width,DCT_SIZE);
	lpCompPtr->wSubsampled_Depth = m_utility.wRoundUp(lpCompPtr->wTrue_Comp_Depth,DCT_SIZE);
	// ** (>>3) means (/DCT_SIZE)
	lpJCVAR->wW_MCUs = lpCompPtr->wSubsampled_Width >> 3;
	lpJCVAR->wH_MCUs = lpCompPtr->wSubsampled_Depth >> 3;

	// ** Prepare array describing MCU composition
	lpJCVAR->wBlocks_in_MCU = 1;
	lpJCVAR->wBlock_Mapping_Comp[0] = 0;
}

	
void CJPGInit::vEncode_Interleaved_Init(LPJPGC_VAR lpJCVAR)
{
	LPJPG_COMP_INFO lpCompPtr;
	WORD wBlocks, wi;
	// ** 可以把某方向取样因子看作是一个MCU中某方向上包含8象素单位的个数
	lpJCVAR->wW_MCUs = (lpJCVAR->wWidth + lpJCVAR->wMaxHor_SampleFactor * DCT_SIZE -1)/ //整幅图象水平方向上包含包含多少MCU；
						(lpJCVAR->wMaxHor_SampleFactor * DCT_SIZE);
	lpJCVAR->wH_MCUs = (lpJCVAR->wDepth + lpJCVAR->wMaxVer_SampleFactor * DCT_SIZE -1)/ //整幅图象垂直方向上包含包含多少MCU；
						(lpJCVAR->wMaxVer_SampleFactor * DCT_SIZE);
	lpJCVAR->wBlocks_in_MCU = 0;

	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];

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
			lpJCVAR->wBlock_Mapping_Comp[lpJCVAR->wBlocks_in_MCU++] = wi;
		}
	}
}


