
// JPGDMcu.cpp: implementation of the CJPGDMcu class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGDMCU.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���봦��MCU��ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
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
		// ** lpJCVAR->wW_MCUsΪһ��MCU���а�����MCU����!
		for(wi=0; wi<lpJDVAR->wW_MCUs; wi++) 
		{
			// ** ��lpJDVAR->lpDataBuff��ָ��ѹ�������н�ѹ��һ��MCU��Ԫ(����MAX_BLOCKS_IN_MCUGE��8*8����),
			// ** ����ŵ�MCU_Data��;��ʱ��û�з������ͷ�DCT�任;
			iReturn = m_jpgdhuff.iEntropy_Decode(m_SrcFile, lpJDVAR,MCU_Data);
			wBlockNo = 0;

			// ** һ��MCUӦ�����������ɷ��е�Block;(ֻ�����ȳɷ�ռ�ı��ش�)
			for(wj=0; wj<lpJDVAR->wComps_SOS; wj++)
			{
				lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wj];
				lpQuantPtr = lpJDVAR->QuantTBL[lpCompPtr->wQuant_TBL_No];

				// ** (<<3) means (*DCT_SIZE);
				wBlockRowOff = lpCompPtr->wSubsampled_Width << 3; //��8�У�
				wRowOff = 0;
				// ** (<<6) means (*DCT_SQUARE);
				// ** ע��:�ڱ���ʱ,�������DCT_SIZE.ԭ���Ǳ���ʱ,��Դ����ȡһ��8*8����ʱҲ��ȷ�ֳ�8�ж�ȡ;
				// ** ���ڽ���ʱ,��Ŀ����д8*8����ʱ,���ǲ����е�,һ������д��;���������н����������vReverseDCT
				// ** ����,�ú����Ὣ����ԭ�ɷ��е�����,����ʵ�����ص�����Ӧ;
				wColOff = (wi * lpCompPtr->wMCU_Width) << 6;
				for(wh=0; wh<lpCompPtr->wMCU_Depth; wh++)
				{
					lpiTemp = lpiCompData[wj] + wRowOff + wColOff;
					for(ww=0; ww<lpCompPtr->wMCU_Width; ww++)
					{
						// ** ��MCU_Data�еĵ�wBlockNo��Block(��8*8����)������,
						// ** ��������˳��ԭ(ѹ��ʱ��֮��������).ת�������ݷŵ�lpiTemp��,��lphCompData
						// ** ��ָ���ڴ��;��ʱ��û�з�DCT�任;
						vQuant_Zigzag(MCU_Data[wBlockNo],lpiTemp,lpQuantPtr);
						wBlockNo++;
						//** �Ƶ���MCU�ĸóɷֵ���һ��Block��λ��(�з�����)��ע��lpiTemp��
						//** ָ�����ε�,lpiTemp����DCT_SQUARE,ʵ����ָ���ƶ���������DCT_SQUARE;
						lpiTemp += DCT_SQUARE;// ����DCT_SIZE;
					}
					wRowOff += wBlockRowOff;//** �Ƶ���MCU�ĸóɷֵ���һ��Block��λ��(�з�����)��
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
						// ** ȷ��iTempֵ�����α��BYTE��(��ֻռ��һ���ֽ�,λ��0--255��);
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

