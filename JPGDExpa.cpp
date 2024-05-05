// JPGDExpa.cpp: implementation of the CJPGDExpa class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCExpa.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码反采样的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGDExpa.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGDExpa::CJPGDExpa()
{

}

CJPGDExpa::~CJPGDExpa()
{

}
extern int iReturn;

int CJPGDExpa::iExpand(LPJPGD_VAR lpJDVAR, WORD wFullWidth, LPSTR *lpRealData, LPSTR *lpDstBuff)
{
	LPJPG_COMP_INFO lpCompPtr;
	WORD wSrcRows, wSrcCols, wHorExpand, wVerExpand, wMaxHor, wMaxVer, wHor, wVer, wi;
	for(wi =0; wi<lpJDVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJDVAR->Component[wi];
		// ** (<<3) means (*DCT_SIZE);
		wSrcRows = lpCompPtr->wVer_SampleFactor << 3;
		wSrcCols = lpCompPtr->wSubsampled_Width;
		wMaxHor = lpJDVAR->wMaxHor_SampleFactor;
		wMaxVer = lpJDVAR->wMaxVer_SampleFactor;
		wHor = lpCompPtr->wHor_SampleFactor;
		wVer = lpCompPtr->wVer_SampleFactor;
		wHorExpand = wMaxHor / wHor;
		wVerExpand = wMaxVer / wVer;
		// ** 处理一个MCU行；
		if(wMaxHor == wHor && wMaxVer == wVer)
			vNullExpand(wSrcRows,wSrcCols,wFullWidth,lpRealData[wi],lpDstBuff[wi]);
		else
			if(wMaxHor == (wHor << 1) && (wMaxVer % wVer) == 0)
				vHor2Expand(wSrcRows,wSrcCols,wFullWidth,wVerExpand,lpRealData[wi],lpDstBuff[wi]);
			else
				if((wMaxHor % wHor) == 0 && (wMaxVer % wVer) == 0)
					vIntExpand(wSrcRows,wSrcCols,wFullWidth,wHorExpand,wVerExpand,lpRealData[wi],lpDstBuff[wi]);
	}
	return 0;
}


void CJPGDExpa::vNullExpand(WORD wSrcRows, WORD wSrcCols, WORD wFullWidth, LPSTR lpSrc, LPSTR lpDst)
{
	WORD wi;
	for(wi=0; wi<wSrcRows; wi++)
	{
		memcpy(lpDst,lpSrc,wSrcCols);
		lpSrc += wSrcCols;
		lpDst += wFullWidth;
	}
}


void CJPGDExpa::vHor2Expand(WORD wSrcRows, WORD wSrcCols, WORD wFullWidth, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst)
{
	LPSTR lpSrcTmp;
	LPSTR lpDstTmp;
	WORD wi, wj, wk;
	
	for(wi=0; wi<wSrcRows; wi++)
	{
		for(wj=0; wj<wVerExpand; wj++)
		{
			lpSrcTmp = lpSrc;
			lpDstTmp = lpDst;
			for(wk=0; wk<wSrcCols; wk += DCT_SIZE)
			{
				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;
				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;

				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;
				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;

				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;
				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;

				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;
				*lpDstTmp++ = *lpSrcTmp;
				*lpDstTmp++ = *lpSrcTmp++;
			}
			lpDst += wFullWidth;
		}
		lpSrc += wSrcCols;
	}
}


void CJPGDExpa::vIntExpand(WORD wSrcRows, WORD wSrcCols, WORD wFullWidth, WORD wHorExpand, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst)
{
	LPSTR lpSrcTmp;
	LPSTR lpDstTmp;
	WORD wi, wj, wk, wl;
	BYTE byChar;
	
	for(wi=0; wi<wSrcRows; wi++)
	{
		for(wj=0; wj<wVerExpand; wj++)
		{
			lpSrcTmp = lpSrc;
			lpDstTmp = lpDst;
			for(wk=0; wk<wSrcCols; wk++)
			{
				byChar = *lpSrcTmp++;
				for(wl=0; wl<wHorExpand; wl++)
					*lpDstTmp++ = byChar;
			}
			lpDst += wFullWidth;
		}
		lpSrc += wSrcCols;
	}
}

