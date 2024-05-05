// JPGCSAMP.cpp: implementation of the CJPGCSAMP class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCSAMP.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码采样的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGCSAMP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGCSAMP::CJPGCSAMP()
{

}

CJPGCSAMP::~CJPGCSAMP()
{

}
extern int iReturn;

int CJPGCSAMP::iSubSample(LPJPGC_VAR lpJCVAR, WORD wFullWidth, LPSTR *lpSrcBuff, LPHANDLE lphCompData)
{
	LPJPG_COMP_INFO lpCompPtr;
	LPSTR lpCompData[MAX_COMPS_IN_SCAN];
	WORD wDstRows, wDstCols, wHorExpand, wVerExpand;
	WORD wMaxHor, wMaxVer, wHor, wVer, wi;

	// ** 获取将要存放三个成分(采样后)的数据的内存指针；
	for(wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		lpCompData[wi] = (LPSTR)GlobalLock(lphCompData[wi]);

	for(wi =0; wi<lpJCVAR->wComps_SOS; wi++)
	{
		lpCompPtr = (LPJPG_COMP_INFO)&lpJCVAR->Component[wi];
		// ** (<<3) means (*DCT_SIZE);
		wDstRows = lpCompPtr->wVer_SampleFactor << 3;
		wDstCols = lpCompPtr->wSubsampled_Width;
		wMaxHor = lpJCVAR->wMaxHor_SampleFactor;
		wMaxVer = lpJCVAR->wMaxVer_SampleFactor;
		wHor = lpCompPtr->wHor_SampleFactor;
		wVer = lpCompPtr->wVer_SampleFactor;
		wHorExpand = wMaxHor / wHor;
		wVerExpand = wMaxVer / wVer;
		// ** 处理一个MCU行；
		if(wMaxHor == wHor && wMaxVer == wVer)
			vNullSubSample(wDstRows, wDstCols,wFullWidth,lpSrcBuff[wi],lpCompData[wi]);
		else
		{
			if((wMaxHor % wHor) == 0 && (wMaxVer & wVer ) == 0)
				vIntSubSample(wDstRows, wDstCols,wFullWidth,wHorExpand,wVerExpand,lpSrcBuff[wi],lpCompData[wi]);
			else
			{
				AfxMessageBox("图象采样比例错误,按不抽样处理!");
				vNullSubSample(wDstRows, wDstCols,wFullWidth,lpSrcBuff[wi],lpCompData[wi]);
			}
		}
	}

	for (wi=0; wi<lpJCVAR->wComps_SOS; wi++)
		GlobalUnlock(lphCompData[wi]);
	return 0;
}


void CJPGCSAMP::vNullSubSample(WORD wDstRows, WORD wDstCols, WORD wFullWidth, LPSTR lpSrc, LPSTR lpDst)
{
	WORD wi;
	for(wi=0; wi<wDstRows; wi++)
	{
		memcpy(lpDst,lpSrc,wDstCols);
		lpSrc += wFullWidth;
		lpDst += wDstCols;
	}
}


void CJPGCSAMP::vIntSubSample(WORD wDstRows, WORD wDstCols, WORD wFullWidth, WORD wHorExpand, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst)
{
	LPSTR lpSrcTmp, lpDstTmp;
	WORD wi, wj;
	WORD wk, wl;
	WORD wPixelNum, wPixelHalfNum;
	WORD wValue;
	WORD wHorOff;
	WORD wVerOff;

	wPixelNum = wHorExpand * wVerExpand;
	wPixelHalfNum = wPixelNum >> 1;
	wVerOff = wVerExpand * wFullWidth;
	for(wi=0; wi<wDstRows; wi++)
	{
		lpDstTmp = lpDst;
		wHorOff = 0;
		for(wj=0; wj<wDstCols; wj++)
		{
			lpSrcTmp = lpSrc + wHorOff;
			wValue = 0;
			// ** 假设wHorExPand和wVerExPand都为2，则下面的两个for循环相当于把源区象素矩阵相邻的4个象素
			// ** 的值相加，作为单独的一个象素值，然后会对该值求平均；最后的效果即是抽样。
			for(wk=0; wk<wVerExpand; wk++)
			{
				for(wl=0; wl<wHorExpand; wl++)//读取相邻的wHorExpand个字节。
					wValue += (WORD)(lpSrcTmp[wl] & (BYTE)0xFF);
				lpSrcTmp += wFullWidth;// 移到源区象素矩阵的下一行。
			}
			*lpDstTmp++ = (BYTE)((wValue + wPixelHalfNum) / wPixelNum);//相当于求一下平均值；
			wHorOff += wHorExpand;
		}
		lpSrc += wVerOff;
		lpDst += wDstCols;
	}
}
