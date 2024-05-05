
// JPGDMcu.cpp: implementation of the CJPGDMcu class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDMCU.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码处理MCU的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGDMcu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGDMcu::CJPGDMcu()
{

}

CJPGDMcu::~CJPGDMcu()
{

}

extern int iReturn;

int CJPGDMcu::iDisassembleMCU(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, LPHANDLE lphCompData)
{
	JPG_BLOCK MCU_Data[MAX_BLOCKS_IN_MCU];
	LPJPG_COMP_INFO lpCompPtr;
	LPJPG_QUANT_TBL lpQuantPtr;
	LPMYWORD lpiCompData[MAX_COMPS_IN_SCAN];
	LPMYWORD lpiTemp;
	WORD wRowOff;
	WORD wColOff;
	WORD wBlockRowOff;
	WORD wBlockNo;
	WORD wi, wj, wh, ww;
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
		lpiCompData[wi] = (LPMYWORD)GlobalLock(lphCompData[wi]);
	if(lpJDVAR->wComps_SOS == 1)
	{
		// ** Component and one block per MCU;
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[0];
		lpQuantPtr = lpJDVAR->QuantTBL[lpCompPtr->wQuant_TBL_No];
		lpiTemp = lpiCompData[0];
		for(wi=0; wi<lpJDVAR->wW_MCUs;wi++)
		{
			// ** All components have the same wW_MCUS
			iReturn = m_jpgdhuff.iEntropy_Decode(m_SrcFile, lpJDVAR,MCU_Data);
			vQuant_Zigzag(MCU_Data[0],lpiTemp,lpQuantPtr);
			lpiTemp += DCT_SQUARE;
		}
	}
	else
	{
		// ** Interleaved_MCU
		// ** lpJCVAR->wW_MCUs为一个MCU行中包含的MCU个数!
		for(wi=0; wi<lpJDVAR->wW_MCUs; wi++) 
		{
			// ** 从lpJDVAR->lpDataBuff所指的压缩数据中解压出一个MCU单元(包括MAX_BLOCKS_IN_MCUGE个8*8矩阵),
			// ** 结果放到MCU_Data中;此时还没有反量化和反DCT变换;
			iReturn = m_jpgdhuff.iEntropy_Decode(m_SrcFile, lpJDVAR,MCU_Data);
			wBlockNo = 0;

			// ** 一个MCU应包含有三个成分中的Block;(只是亮度成分占的比重大)
			for(wj=0; wj<lpJDVAR->wComps_SOS; wj++)
			{
				lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wj];
				lpQuantPtr = lpJDVAR->QuantTBL[lpCompPtr->wQuant_TBL_No];

				// ** (<<3) means (*DCT_SIZE);
				wBlockRowOff = lpCompPtr->wSubsampled_Width << 3; //即8行；
				wRowOff = 0;
				// ** (<<6) means (*DCT_SQUARE);
				// ** 注意:在编码时,这里乘以DCT_SIZE.原因是编码时,从源区读取一个8*8矩阵时也的确分成8行读取;
				// ** 而在解码时,向目的区写8*8矩阵时,则是不分行的,一次连续写入;在主程序中接下来会调用vReverseDCT
				// ** 函数,该函数会将矩阵还原成分行的特征,即与实际象素点矩阵对应;
				wColOff = (wi * lpCompPtr->wMCU_Width) << 6;
				for(wh=0; wh<lpCompPtr->wMCU_Depth; wh++)
				{
					lpiTemp = lpiCompData[wj] + wRowOff + wColOff;
					for(ww=0; ww<lpCompPtr->wMCU_Width; ww++)
					{
						// ** 将MCU_Data中的第wBlockNo个Block(即8*8矩阵)反量化,
						// ** 并将排列顺序还原(压缩时按之字形排列).转换后数据放到lpiTemp中,即lphCompData
						// ** 所指的内存块;此时还没有反DCT变换;
						vQuant_Zigzag(MCU_Data[wBlockNo],lpiTemp,lpQuantPtr);
						wBlockNo++;
						//** 移到该MCU的该成分的下一个Block块位置(行方向上)；注意lpiTemp是
						//** 指向整形的,lpiTemp加上DCT_SQUARE,实际上指针移动了两倍的DCT_SQUARE;
						lpiTemp += DCT_SQUARE;// 不是DCT_SIZE;
					}
					wRowOff += wBlockRowOff;//** 移到该MCU的该成分的下一个Block块位置(列方向上)；
				}
			}
		}
	}
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
		GlobalUnlock(lphCompData[wi]);
	return 0;
}


void CJPGDMcu::vQuant_Zigzag(LPJPG_BLOCK SrcBlock, LPJPG_BLOCK DstBlock, LPJPG_QUANT_TBL lpQuantPtr)
{
	WORD wZigzag[DCT_SQUARE] = {
		0,  1,  8,  16, 9,  2,  3,  10, 
		17, 24, 32, 25, 18, 11, 4,  5,
		12, 19, 26, 33, 40, 48, 41, 34,
		27, 20, 13, 6,  7,  14, 21, 28,
		35, 42, 49, 56, 57, 50, 43, 36,
		29, 22, 15, 23, 30, 37, 44, 51,
		58, 59, 52, 45, 38, 31, 39, 46,
		53, 60, 61, 54, 47, 55, 62, 63
	};
	register int i;
	for(i=0; i<DCT_SQUARE; i++)
	{
		DstBlock[wZigzag[i]] = SrcBlock[i] * lpQuantPtr[i];
	}
}



void CJPGDMcu::vReverseDCT(LPJPGD_VAR lpJDVAR, LPHANDLE lphCompData, LPSTR *lpRealData, WORD wStartBlockRow)
{
	JPG_BLOCK JPG_Block;
	LPJPG_BLOCK lpBlock;
	LPJPG_COMP_INFO lpCompPtr;
	LPMYWORD lpiCompData[MAX_COMPS_IN_SCAN];
	LPMYWORD lpiSrc;
	LPMYWORD lpiSrcTmp;
	LPSTR lpDst, lpDstRowPtr, lpDstTmp;
	WORD wBlocks_PerRow;
	WORD wBlock_Rows, wBlockRowOff;
	WORD wDstColOff;
	WORD wi, ww, wh;
	int iRow;
	register MYWORD iTemp;
	register MYWORD iCol;

	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
		lpiCompData[wi] = (LPMYWORD)GlobalLock(lphCompData[wi]);
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];

		// ** (>>3) means ( /DCT_SIZE);
		wBlocks_PerRow = lpCompPtr->wSubsampled_Width >> 3;
		wBlock_Rows = lpCompPtr->wMCU_Depth;

		// ** (<<3) means (*DCT_SIZE);
		wBlockRowOff = lpCompPtr->wSubsampled_Width << 3;
		lpiSrc = lpiCompData[wi];  
		lpDst = lpRealData[wi] + wStartBlockRow * wBlockRowOff;
		for(wh=0; wh<wBlock_Rows; wh++)
		{
			lpiSrcTmp = lpiSrc; 
			lpDstRowPtr = lpDst;
			wDstColOff = 0;
			for(ww=0; ww<wBlocks_PerRow; ww++)
			{
				memcpy((LPSTR)JPG_Block,(LPSTR)lpiSrcTmp,DCT_SQUARE*2);//sizeof(JPG_BLOCK)
				m_jpgdidct.vDCT_Inverse(JPG_Block);
				
				lpBlock = JPG_Block;

				// ** output the data into the sample array.Note change from signed to unsigned
				// ** representation: DCT calculation works with values + - 128,but sample array
				// ** always hold 0..255.Have to do explicit range-limiting because of quantization
				// ** errors and so forth in the DCT/IDCT phase.

				lpDstTmp = lpDstRowPtr + wDstColOff;
				for(iRow=0; iRow<DCT_SIZE; iRow++)
				{
					for(iCol=0; iCol<DCT_SIZE; iCol++)
					{
						// ** 0<=lpDstTmp[iCol]<=255;
						iTemp = (*lpBlock++) + 128;
						// ** 确保iTemp值由整形变成BYTE型(即只占用一个字节,位于0--255间);
						lpDstTmp[iCol] = (BYTE)MAX(MIN(iTemp,255),0);
					}
					lpDstTmp += lpCompPtr->wSubsampled_Width;
				}
				wDstColOff += DCT_SIZE;
				lpiSrcTmp += DCT_SQUARE; 
			}
			lpiSrc += wBlockRowOff;
			lpDst += wBlockRowOff;
		}
	}
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
		GlobalUnlock(lphCompData[wi]);
}

