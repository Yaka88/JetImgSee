// JPGCHuff.cpp: implementation of the CJPGCHuff class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCFhuff.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码HUFF算法的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGCHuff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGCHuff::CJPGCHuff()
{

}

CJPGCHuff::~CJPGCHuff()
{
}

extern int iReturn;

int CJPGCHuff::iEntropy_Encode(CFile *m_DstFile, LPJPGC_VAR lpJCVAR, LPJPG_BLOCKROW MCU_Data)
{
	HANDLE hDC_HuffTBL, hAC_HuffTBL;
	LPJPG_COMP_INFO lpCompPtr;
	LPJPGC_HUFF_TBL lpDC_HuffTBL;
	LPJPGC_HUFF_TBL lpAC_HuffTBL;
	WORD wCompNdx;
	MYWORD iDC, iNdx, iBits, iTemp1, iTemp2;
	int iZeroLen ;
	register int i, j ;

	// ** Account for restart interval, process restart marker if needed
	if(lpJCVAR->wRestart_Interval)
	{
		if(lpJCVAR->wRestart_Cnt == 0)
			iRestart_Encode(m_DstFile,lpJCVAR);
		lpJCVAR->wRestart_Cnt--;
	}
	for(i=0; i<(int)lpJCVAR->wBlocks_in_MCU; i++)
	{
		wCompNdx = lpJCVAR->wBlock_Mapping_Comp[i]; //wCompNdx实际上是成分的编号；
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wCompNdx];
		hDC_HuffTBL = lpJCVAR->hDC_HuffTBL[lpCompPtr->wDC_TBL_No];
		hAC_HuffTBL = lpJCVAR->hAC_HuffTBL[lpCompPtr->wAC_TBL_No];
		lpDC_HuffTBL = (LPJPGC_HUFF_TBL)GlobalLock(hDC_HuffTBL);
		lpAC_HuffTBL = (LPJPGC_HUFF_TBL)GlobalLock(hAC_HuffTBL);

		// ** Convert DC value to difference ,update Prev_DC_Val
		iDC = MCU_Data[i][0];
		MCU_Data[i][0] -= lpJCVAR->iPrev_DC_Val[wCompNdx];
		lpJCVAR->iPrev_DC_Val[wCompNdx] = iDC;

		// ** Encode one block
		// ** Encode the DC coefficient difference per section F1.2.2
		iTemp1 = MCU_Data[i][0];
		iTemp2 = iTemp1;

		if(iTemp1 < 0)
		{
			// ** iTemp1 is abs value of input
			iTemp1 = -iTemp1;
			// ** For a negative input,want iTemp2=bitwise complement of abs(input).
			// ** This code assumes we are on a two's complement machine.
			iTemp2--;
		}
		// ** Find the number of bits needed for the magnitude of the coeffecient
		iBits = 0;
		while(iTemp1)
		{
			iBits++;
			iTemp1 >>= 1;
		}
		// ** Emit the Huffman--coded symbol for the number of bits
		iReturn = iBits_Encode(m_DstFile, lpJCVAR, lpDC_HuffTBL->wHuffCode[iBits], lpDC_HuffTBL->wHuffSize[iBits]);
		// ** Emit that number of bits of the value, if positive
		// ** or the complement of its magnitude, if negative.
		if(iBits)
		{
			// ** emit_bits rejects calls with size 0
			iReturn = iBits_Encode(m_DstFile, lpJCVAR, (WORD)iTemp2, (WORD)iBits);
		}

		// ** Encode the AC coefficients per section F.1.2.2
		iZeroLen = 0;
		for(j=1; j<DCT_SQUARE; j++)
		{
			iTemp1 = MCU_Data[i][j];
			if(iTemp1 == 0)
				iZeroLen ++;
			else
			{
				// ** if run length>15,must emit special run_length_16 codes(0xF0)
				while(iZeroLen > 15)
				{
					iReturn = iBits_Encode(m_DstFile, lpJCVAR, lpAC_HuffTBL->wHuffCode[0xF0], lpAC_HuffTBL->wHuffSize[0xF0]);
					iZeroLen -= 16;
				}
				iTemp2 = iTemp1;
				if(iTemp1 < 0)
				{
					// ** iTemp1 is abs value of input
					iTemp1 = -iTemp1;
					// ** For a negative input,want iTemp2=bitwise complement of abs(input).
					// ** This code assumes we are on a two's complement machine.
					iTemp2--;
				}

				// ** Find the number of bits needed for the magnitude of the coeffecient
				// ** there must be at least one 1 bit.
				iBits = 1; //也可以从0开始，但下面的循环条件要改。
				while(iTemp1 >>= 1)
					iBits++;
				// ** Emit Huffman symbol for run length / number of bits;
				iNdx = (iZeroLen << 4) + iBits; //**即（RRRR/SSSS）
				iReturn = iBits_Encode(m_DstFile, lpJCVAR, lpAC_HuffTBL->wHuffCode[iNdx], lpAC_HuffTBL->wHuffSize[iNdx]);
				// ** Emit that number of bits of the value ,if positive; or the complement of its magnitude, if negative;
				iReturn = iBits_Encode(m_DstFile, lpJCVAR, (WORD)iTemp2, (WORD)iBits);
				iZeroLen = 0;
			}
		}

		// ** If the last coef(s) were zero, emit an end-of-block code.
		if(iZeroLen > 0)
			iReturn = iBits_Encode(m_DstFile, lpJCVAR, lpAC_HuffTBL->wHuffCode[0], lpAC_HuffTBL->wHuffSize[0]);
		
		GlobalUnlock(hAC_HuffTBL);
		GlobalUnlock(hDC_HuffTBL);
	}
	return 0;
}


int CJPGCHuff::iBits_Encode(CFile *m_DstFile, LPJPGC_VAR lpJCVAR, WORD wCode, WORD wSize)
{
	// ** This routine is heavily used ,so it's worth coding tightly
	register LONG lHuffBuff;
	register int iHuffBits;
	DWORD dwBuffCnt;
	BYTE byChar;

	lHuffBuff = (LONG)wCode;
	iHuffBits = lpJCVAR->iHuffBits;

	// ** Mask off any excess bits in code 
	lHuffBuff &= ((1L << wSize) - 1L); //** 非常巧妙，取long型的最低wSize位；

	// ** New number of bits in buffer;
	iHuffBits += (int)wSize; //经过一次循环后,iHuffBits的值不一定是零;

	// ** Align incoming bits
	lHuffBuff <<= (24 - iHuffBits);

	// ** and merge with old buffer contents;
	lHuffBuff |= lpJCVAR->lHuffBuff; //只是连接，不会重合；

	while(iHuffBits >= 8)
	{
		byChar = (BYTE)(lHuffBuff >> 16);
		dwBuffCnt = lpJCVAR->dwBuffCnt;
		if(dwBuffCnt == MAX_BUFF_SIZE)
		{
			m_DstFile->Write(lpJCVAR->lpDataBuff,dwBuffCnt);
			dwBuffCnt = 0;
		}
		lpJCVAR->lpDataBuff[dwBuffCnt++] = byChar;

		if(byChar == 0xFF) 
		{
			// ** Need to stuff a zero byte ? 
			if(dwBuffCnt == MAX_BUFF_SIZE)
			{
				m_DstFile->Write(lpJCVAR->lpDataBuff,dwBuffCnt);
				dwBuffCnt = 0;
			}
			lpJCVAR->lpDataBuff[dwBuffCnt++] = 0;
		}
		lpJCVAR->dwBuffCnt = dwBuffCnt;
		lHuffBuff <<= 8; //将刚才写入的一个字节去除(lHuffBuff最高字节不用)
		iHuffBits -= 8;
	}

	lpJCVAR->lHuffBuff = lHuffBuff;
	lpJCVAR->iHuffBits = iHuffBits;

	return 0;
}


int CJPGCHuff::iRestart_Encode(CFile *m_DstFile, LPJPGC_VAR lpJCVAR)
{
	WORD wi;
	BYTE byChar;
	DWORD dwBuffCnt;

	// ** Flush bits
	iReturn = iBits_Encode(m_DstFile, lpJCVAR, (WORD)0x7F, (WORD)7);
	lpJCVAR->lHuffBuff = 0;
	lpJCVAR->iHuffBits = 0;
	dwBuffCnt = lpJCVAR->dwBuffCnt;
	if(dwBuffCnt == MAX_BUFF_SIZE)
	{
		m_DstFile->Write(lpJCVAR->lpDataBuff, dwBuffCnt);
		dwBuffCnt = 0;
	}
	lpJCVAR->lpDataBuff[dwBuffCnt++] = (BYTE)0xFF;
	if(dwBuffCnt == MAX_BUFF_SIZE)
	{
		m_DstFile->Write(lpJCVAR->lpDataBuff, dwBuffCnt);
		dwBuffCnt = 0;
	}
	byChar = (BYTE)((WORD)(M_RST0 & (BYTE)0xFF) + lpJCVAR->wRestart_Num);
	lpJCVAR->lpDataBuff[dwBuffCnt++] = byChar;
	lpJCVAR->dwBuffCnt = dwBuffCnt;

	// ** Re-initialize DC predictions to 0
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		lpJCVAR->iPrev_DC_Val[wi] = 0;

	// ** Update restart state
	lpJCVAR->wRestart_Cnt = lpJCVAR->wRestart_Interval;
	lpJCVAR->wRestart_Num ++;
	lpJCVAR->wRestart_Num &= 7;

	return 0;
}
