// JPGDHuff.cpp: implementation of the CJPGDHuff class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDhuff.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码HUFF算法的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGDHuff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGDHuff::CJPGDHuff()
{

}

CJPGDHuff::~CJPGDHuff()
{

}

extern int iReturn;

int CJPGDHuff::iEntropy_Decode(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, LPJPG_BLOCKROW MCU_Data)
{
	HANDLE hDC_HuffTBL, hAC_HuffTBL;
	LPJPG_COMP_INFO lpCompPtr;
	LPJPGD_HUFF_TBL lpDC_HuffTBL;
	LPJPGD_HUFF_TBL lpAC_HuffTBL;
	WORD wCompNdx;
	BYTE byChar;
	MYWORD iDC, iAC, iAdd, iCode;
	register MYWORD iCnt;
	register int i;

	// ** Account for restart interval, process restart marker if needed
	if(lpJDVAR->wRestart_Interval)
	{
		if(lpJDVAR->wRestart_Cnt == 0)
			iRestart_Decode(m_SrcFile,lpJDVAR);
		lpJDVAR->wRestart_Cnt--;
	}
	for(i=0; i<(int)lpJDVAR->wBlocks_in_MCU; i++)
	{
		wCompNdx = lpJDVAR->wBlock_Mapping_Comp[i];
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wCompNdx];
		hDC_HuffTBL = lpJDVAR->hDC_HuffTBL[lpCompPtr->wDC_TBL_No];
		hAC_HuffTBL = lpJDVAR->hAC_HuffTBL[lpCompPtr->wAC_TBL_No];
		lpDC_HuffTBL = (LPJPGD_HUFF_TBL)GlobalLock(hDC_HuffTBL);
		lpAC_HuffTBL = (LPJPGD_HUFF_TBL)GlobalLock(hAC_HuffTBL);

		// ** Decode one block
		memset((LPSTR)MCU_Data[i],0,DCT_SQUARE*2);//sizeof(JPG_BLOCK)

		// ** Section F.2.2.1 :decode the DC coefficient defference
		iCode = iHuff_Decode(m_SrcFile,lpJDVAR,lpDC_HuffTBL,&iDC);
		if(iDC)
		{
			// ** Get bits of iDC number
			// ** 刚才读取了SSSS,接下来要读取VVVV,VVVV的编码长度等于iDC;
			while(iDC > lpJDVAR->iHuffBits)
			{
				byChar = *lpJDVAR->lpBgnBuff++;
				if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
				{
					iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
													lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
					lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
					lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
				}
				lpJDVAR->lHuffBuff <<= 8;
				lpJDVAR->lHuffBuff |= (DWORD)(byChar & (BYTE)0xFF);
				lpJDVAR->iHuffBits += 8;

				// ** If it's 0xFF ,check and discard stuffed zero byte
				if(byChar == (BYTE)0xFF)
				{
					// ** Zero by stuffing
					byChar = *lpJDVAR->lpBgnBuff++;
					if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
					{
						iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
														lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
						lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
						lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
					}
				}
			}
			lpJDVAR->iHuffBits -= iDC;
			// ** 下面iAC为VVVV的编码值,并非AC系数;
			iAC = (MYWORD)((lpJDVAR->lHuffBuff >> lpJDVAR->iHuffBits) & ((1L << iDC) - 1L));
			iDC = HUFF_EXTEND(iAC, iDC);
		}
		MCU_Data[i][0] = iDC;

		// ** Section F.2.2.2: decode the AC coefficients
		for(iCnt=1; iCnt<DCT_SQUARE; iCnt++)
		{
			iCode = iHuff_Decode(m_SrcFile,lpJDVAR,lpAC_HuffTBL,&iAC);
			iDC = iAC & 15;
			iAdd = iAC >> 4;
			if(iDC)
			{
				iCnt += iAdd;
				// ** Get bits of iDC number
				while(iDC > lpJDVAR->iHuffBits)
				{
					byChar = *lpJDVAR->lpBgnBuff++;
					if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
					{
						iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
														lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
						lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
						lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
					}
					lpJDVAR->lHuffBuff <<= 8;
					lpJDVAR->lHuffBuff |= (DWORD)(byChar & (BYTE)0xFF);
					lpJDVAR->iHuffBits += 8;

					// ** If it's 0xFF ,check and discard stuffed zero byte
					if(byChar == (BYTE)0xFF)
					{
						// ** Zero by stuffing
						byChar = *lpJDVAR->lpBgnBuff++;
						if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
						{
							iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
															lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
							lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
							lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
						}
					}
				}
				lpJDVAR->iHuffBits -= iDC;
				iAC = (MYWORD)((lpJDVAR->lHuffBuff >> lpJDVAR->iHuffBits) & ((1L << iDC) - 1L));
				MCU_Data[i][iCnt] = HUFF_EXTEND(iAC, iDC);
			}
			// **或者有连续16个0，或者该矩阵剩下的AC系数皆为0，若是后者则跳出；
			else 
			{
				if(iAdd != 15)
					break;
				iCnt += iAdd;// (iAdd + 1)错误，15/0本身已占用一个编码；
			}
		}

		// ** Convert DC difference to actual ,update last_dc_val 
		// ** eg. 0,B1=3,B2=7,B3=9,B4=6,B5=4;
		// ** 0,D1=3,D2=4,D3=2,D4=-3,D5=-2,Dm=Bn-Bm,n=m+1;
		MCU_Data[i][0] += lpJDVAR->iPrev_DC_Val[wCompNdx];
		lpJDVAR->iPrev_DC_Val[wCompNdx] = MCU_Data[i][0];
		GlobalUnlock(hAC_HuffTBL);
		GlobalUnlock(hDC_HuffTBL);
	}
	return 0;
}


int CJPGDHuff::iHuff_Decode(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, LPJPGD_HUFF_TBL lpHuffPtr, LPMYWORD lpiVal)
{
	WORD wVal, wi;
	BYTE byChar;
	LONG lVal, lBitVal;
	int iCode;

	// ** Figure F.16 extract next coded from input stream
	// ** Get 1 bit
	if(lpJDVAR->iHuffBits)
		lBitVal = ((lpJDVAR->lHuffBuff >> (--lpJDVAR->iHuffBits)) & 1L);
	else
	{
		while(1>lpJDVAR->iHuffBits)
		{
			byChar = *lpJDVAR->lpBgnBuff++;
			if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
			{
				iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
												lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
				lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
				lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
			}
			lpJDVAR->lHuffBuff <<= 8;
			lpJDVAR->lHuffBuff |= (LONG)(byChar & (BYTE)0xFF);
			lpJDVAR->iHuffBits += 8;

			// ** If it's 0xFF,  check and discard stuffed zero byte
			if(byChar == (BYTE)0xFF)
			{
				// ** Zero by stuffing
				byChar = *lpJDVAR->lpBgnBuff++;
				if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
				{
					iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
													lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
					lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
					lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
				}
			}
		}
		lpJDVAR->iHuffBits -= 1;
		lBitVal = ((lpJDVAR->lHuffBuff >> lpJDVAR->iHuffBits) & ((1L << 1) - 1L));
	}

	lVal = lBitVal;
	wi = 1;
	while(lVal > lpHuffPtr->lMaxCode[wi])
	{
		// ** Get 1 bit
		if(lpJDVAR->iHuffBits)
			lBitVal = ((lpJDVAR->lHuffBuff >> (--lpJDVAR->iHuffBits)) & 1L);
		else
		{
			while(1 > lpJDVAR->iHuffBits)
			{
				byChar = *lpJDVAR->lpBgnBuff++;
				if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
				{
					iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
													lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
					lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
					lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
				}
				lpJDVAR->lHuffBuff <<= 8;
				lpJDVAR->lHuffBuff |= (LONG)(byChar & (BYTE)0xFF);
				lpJDVAR->iHuffBits += 8;

				// ** If it's 0xFF,  check and discard stuffed zero byte
				if(byChar == (BYTE)0xFF)
				{
					// ** Zero by stuffing
					byChar = *lpJDVAR->lpBgnBuff++;
					if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
					{
						iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
														lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
						lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
						lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
					}
				}
			}
			lpJDVAR->iHuffBits -= 1;
			lBitVal = ((lpJDVAR->lHuffBuff >> lpJDVAR->iHuffBits) & ((1L << 1) - 1L));
		}
		lVal = (lVal << 1) + lBitVal;
		wi++;
	}
	wVal = (WORD)((LONG)lpHuffPtr->iValPtr[wi] + (lVal - (LONG)lpHuffPtr->wMinCode[wi]));
	*lpiVal = (MYWORD)(lpHuffPtr->byHuffVal[wVal] & (BYTE)0xFF);
	return 0;
}


int CJPGDHuff::iRestart_Decode(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR)
{
	WORD wi;
	BYTE byMarker;
	int iCode;

	// ** Throw away any partial unread byte
	lpJDVAR->iHuffBits = 0;

	// ** Scan for next JPEG marker
	do
	{
		// ** Skip any non-FF bytes
		do
		{
			byMarker = *lpJDVAR->lpBgnBuff++;
			if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
			{
				iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
												lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
				lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
				lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
			}
		} while (byMarker != (BYTE)0xFF);

		// ** Skip any duplicate FFs
		do
		{
			byMarker = *lpJDVAR->lpBgnBuff++;
			if(lpJDVAR->lpBgnBuff == lpJDVAR->lpEndBuff && !lpJDVAR->bEOF)
			{
				iCode = m_utility.iReadSrcData(m_SrcFile,&lpJDVAR->wMemLen,&lpJDVAR->dwDataLen,
												lpJDVAR->lpDataBuff,&lpJDVAR->bEOF);
				lpJDVAR->lpBgnBuff = lpJDVAR->lpDataBuff;
				lpJDVAR->lpEndBuff = lpJDVAR->lpBgnBuff + lpJDVAR->wMemLen;
			}
		} while (byMarker == (BYTE)0xFF);
	}while (byMarker == (BYTE)0x00);

	// ** Re-initialize DC predictions to 0;
	for(wi=0; wi<lpJDVAR->wComps_SOS; wi++)
		lpJDVAR->iPrev_DC_Val[wi] = 0;

	// ** Updata restart state
	lpJDVAR->wRestart_Cnt = lpJDVAR->wRestart_Interval;
	return 0;
}


		

int CJPGDHuff::HUFF_EXTEND(MYWORD iAC, MYWORD iDC)
{
	MYWORD iMaxVal, iMinVal;
	UNMYWORD iTemp = 0xFFFF;
	iMaxVal = (1 << iDC) - 1;
	iMinVal = (1 << (iDC -1));
	if ((iAC >= iMinVal) && (iAC <= iMaxVal))
		iDC = iAC ;
	else
	/*{
		iTemp  <<= iDC;
		iAC |= iTemp;
		iTemp = iAC;
		iTemp = ~iTemp ;
		iDC = iTemp ;
		iTemp = 0xFFFF;
		iDC &= iTemp;
		iDC = -iDC - 1;
	}*/
	{
		iAC = ~iAC;
		iTemp >>= (sizeof(UNMYWORD)*8 - iDC);
		iAC &= iTemp;
		iDC = -iAC ;
	
	}
	return iDC;
}
