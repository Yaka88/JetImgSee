// JPGCMcu.cpp: implementation of the CJPGCMcu class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCMCU.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码处理MCU的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGCMcu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGCMcu::CJPGCMcu()
{

}

CJPGCMcu::~CJPGCMcu()
{

}
extern int iReturn;

int CJPGCMcu::iExtractMCU(CFile *m_DstFile, LPJPGC_VAR lpJCVAR, LPHANDLE lphCompData, WORD wMCU_NumRows)
{

	JPG_BLOCK MCU_Data[MAX_BLOCKS_IN_MCU];
	LPJPG_COMP_INFO lpCompPtr;
	LPJPG_QUANT_TBL lpQuantPtr;
	LPSTR lpCompData[MAX_COMPS_IN_SCAN];
	LPSTR lpTemp;
	WORD wRowOff;
	WORD wColOff;
	WORD wBlockRowOff;
	WORD wBlockNo;
	WORD wi, wj, wk, wh, ww;
	WORD wMyTemp ;
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		lpCompData[wi] = (LPSTR)GlobalLock(lphCompData[wi]);
	for(wi=0; wi<wMCU_NumRows; wi++) // ** 对于彩色图象(YCbCr)wMCU_NumRows为零。
	{
		// ** lpJCVAR->wW_MCUs为一个MCU行中包含的MCU个数!
		for(wj=0; wj<lpJCVAR->wW_MCUs; wj++) 
		{
			// ** Extract data from the image array,DCT it, and quantize it
			wBlockNo = 0;

			// ** 一个MCU应包含有三个成分中的Block;(只是亮度成分占的比重大)
			for(wk=0; wk<lpJCVAR->wComps_SOS; wk++)
			{
				lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wk];
				lpQuantPtr = lpJCVAR->QuantTBL[lpCompPtr->wQuant_TBL_No];

				// ** (<<3) means (*DCT_SIZE);
				wBlockRowOff = lpCompPtr->wSubsampled_Width << 3; //即8行；
				wRowOff = 0;
				// ** (<<3) means (*DCT_SIZE);
				wColOff = (wj * lpCompPtr->wMCU_Width) << 3;
				for(wh=0; wh<lpCompPtr->wMCU_Depth; wh++)
				{
					lpTemp = lpCompData[wk] + wRowOff + wColOff;
					wMyTemp = wRowOff + wColOff;
					for(ww=0; ww<lpCompPtr->wMCU_Width; ww++)
					{
						vExtractBlock(lpTemp,MCU_Data[wBlockNo],lpQuantPtr,lpCompPtr->wSubsampled_Width);
						wBlockNo++;
						lpTemp += DCT_SIZE; //** 移到该MCU的该成分的下一个Block块位置(行方向上)；　
					}
					wRowOff += wBlockRowOff;//** 移到该MCU的该成分的下一个Block块位置(列方向上)；
				}
			}
			// ** 将该MCU行的当前产生的这个MCU进行编码并保存；
			iReturn = m_jpgchuff.iEntropy_Encode(m_DstFile, lpJCVAR, MCU_Data);
		}
	}
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		GlobalUnlock(lphCompData[wi]);
	return 0;
}

void CJPGCMcu::vExtractBlock(LPSTR lpSrc, LPJPG_BLOCK DstBlock, LPJPG_QUANT_TBL lpQuantPtr, WORD wWidthBytes)
{
	// **　若在一个8*8的矩阵中按下面0-1-8-16-9-2-3-10-17-24---的顺序将各元素连线起来，将是"之"字型；
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
	JPG_BLOCK JPG_Block;
	LPJPG_BLOCK lpBlock;
	register MYWORD iValue;
	register int i;

	lpBlock = JPG_Block;
	for(i=0; i<DCT_SIZE; i++)
	{
		*lpBlock++ = (int)(lpSrc[0] & (BYTE)0xFF) - 128;
		*lpBlock++ = (int)(lpSrc[1] & (BYTE)0xFF) - 128;
		*lpBlock++ = (int)(lpSrc[2] & (BYTE)0xFF) - 128;
		*lpBlock++ = (int)(lpSrc[3] & (BYTE)0xFF) - 128;
		*lpBlock++ = (int)(lpSrc[4] & (BYTE)0xFF) - 128;
		*lpBlock++ = (int)(lpSrc[5] & (BYTE)0xFF) - 128;
		*lpBlock++ = (int)(lpSrc[6] & (BYTE)0xFF) - 128;
		*lpBlock++ = (int)(lpSrc[7] & (BYTE)0xFF) - 128;

		lpSrc += wWidthBytes;// 移到该MCU行块的下一行；也即该Block的下一行；
	}

	// FDCT 变换；
	m_jpgcfdct.vDCT_Forward(JPG_Block);


	for(i=0; i<DCT_SQUARE; i++)
	{
		iValue = JPG_Block[wZigzag[i]];
		iValue = (iValue > 0) ? (iValue + (lpQuantPtr[i] >> 1)) : (iValue - (lpQuantPtr[i] >> 1));
		DstBlock[i] = iValue / lpQuantPtr[i];
	}
}


