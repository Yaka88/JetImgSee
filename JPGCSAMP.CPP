// JPGCSAMP.cpp: implementation of the CJPGCSAMP class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGCSAMP.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���������ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
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

	// ** ��ȡ��Ҫ��������ɷ�(������)�����ݵ��ڴ�ָ�룻
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
		// ** ����һ��MCU�У�
		if(wMaxHor == wHor && wMaxVer == wVer)
			vNullSubSample(wDstRows, wDstCols,wFullWidth,lpSrcBuff[wi],lpCompData[wi]);
		else
		{
			if((wMaxHor % wHor) == 0 && (wMaxVer & wVer ) == 0)
				vIntSubSample(wDstRows, wDstCols,wFullWidth,wHorExpand,wVerExpand,lpSrcBuff[wi],lpCompData[wi]);
			else
			{
				AfxMessageBox("ͼ�������������,������������!");
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
			// ** ����wHorExPand��wVerExPand��Ϊ2�������������forѭ���൱�ڰ�Դ�����ؾ������ڵ�4������
			// ** ��ֵ��ӣ���Ϊ������һ������ֵ��Ȼ���Ը�ֵ��ƽ��������Ч�����ǳ�����
			for(wk=0; wk<wVerExpand; wk++)
			{
				for(wl=0; wl<wHorExpand; wl++)//��ȡ���ڵ�wHorExpand���ֽڡ�
					wValue += (WORD)(lpSrcTmp[wl] & (BYTE)0xFF);
				lpSrcTmp += wFullWidth;// �Ƶ�Դ�����ؾ������һ�С�
			}
			*lpDstTmp++ = (BYTE)((wValue + wPixelHalfNum) / wPixelNum);//�൱����һ��ƽ��ֵ��
			wHorOff += wHorExpand;
		}
		lpSrc += wVerOff;
		lpDst += wDstCols;
	}
}