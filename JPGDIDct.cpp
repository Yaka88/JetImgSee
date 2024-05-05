// JPGDIDct.cpp: implementation of the CJPGDIDct class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDidct.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码DCT变换的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JPGDIDct.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJPGDIDct::CJPGDIDct()
{

}

CJPGDIDct::~CJPGDIDct()
{

}

extern int iReturn;

// ** Macro to right shift bits (if x<0 then add sign bit) 
#define RIGHT_SHIFT(x,shift) ((lShiftTemp = (x) ) < 0 ? \
							((lShiftTemp >> (shift)) | ((~(0L)) << (32-(shift)))) : \
							(lShiftTemp >> (shift)))
#define CONST_BITS 13
#define PASS1_BITS 2
#define  ONE (1L)
#define CONST_SCALE (ONE << CONST_BITS)

// ** Convert a positive real constant to an integer scaled by CONST_SCALE
#define FIX(x) ((LONG) ((x) * CONST_SCALE + 0.5))
#define FIX_0_298631336 (2446L) // FIX(0.298631336)
#define FIX_0_390180644 (3196L) // FIX(0.390180644)
#define FIX_0_541196100 (4433L) // FIX(0.541196100)
#define FIX_0_765366865 (6270L) // FIX(0.765366865)
#define FIX_0_899976223 (7373L) // FIX(0.899976223)

#define FIX_1_175875602 (9633L) // FIX(0.175875602)
#define FIX_1_501321110 (12299L) // FIX(0.501321110)
#define FIX_1_847759065 (15137L) // FIX(0.847759065)
#define FIX_1_961570560 (16069L) // FIX(0.961570560)

#define FIX_2_053119869 (16819L) // FIX(0.053119869)
#define FIX_2_562915447 (20995L) // FIX(0.562915447)
#define FIX_3_072711026 (25172L) // FIX(0.765366865)

// ** Descale and correctly round an LONG value that is scaled by N
// ** bits. We assume RIGHT_SHIFT rounds towards minus infinity, so
// ** adding the fudge factor is correct for either sign of X;
#define DESCALE(x,n) RIGHT_SHIFT((x) + (ONE << ((n) - 1)),n)
#define MULTIPLY(var,const) (((int)(var)) * ((LONG)(const)))


void CJPGDIDct::vDCT_Inverse(LPJPG_BLOCK Block)
{
	LPMYWORD lpiPtr;
	int i;
	LONG lTmp0, lTmp1, lTmp2, lTmp3, lTmp10, lTmp11, lTmp12, lTmp13;
	LONG lTmpZ1, lTmpZ2,lTmpZ3,lTmpZ4,lTmpZ5;
	LONG lShiftTemp;
	// ** A 2-D IDCT can be done by 1-D IDCT on each row followed by 1-D IDCT on each column.
	// ** Direct algorithms are also available, but they are much more complex and seem not to
	// ** be any faster when reduced to code .Pass 1: process rows.
	lpiPtr = (LPMYWORD)Block;
	for(i=0; i<DCT_SIZE; i++)
	{
		// ** Even part: reverse the even part of the forward DCT. The rotator is sqrt(2) * (-c6).
		lTmpZ2 = (LONG)lpiPtr[2];
		lTmpZ3 = (LONG)lpiPtr[6];

		lTmpZ1 = MULTIPLY(lTmpZ2 + lTmpZ3, FIX_0_541196100);
		lTmp2 = MULTIPLY(lTmpZ3, -FIX_1_847759065) + lTmpZ1;
		lTmp3 = MULTIPLY(lTmpZ2, FIX_0_765366865) + lTmpZ1;

		lTmp0 = ((LONG)lpiPtr[0] + (LONG)lpiPtr[4]) << CONST_BITS;
		lTmp1 = ((LONG)lpiPtr[0] - (LONG)lpiPtr[4]) << CONST_BITS;

		lTmp10 = lTmp0 + lTmp3;
		lTmp13 = lTmp0 - lTmp3;
		lTmp11 = lTmp1 + lTmp2;
		lTmp12 = lTmp1 - lTmp2;

		// ** Odd part per figrue 8; the matrix is unitary and hence its transpose is its inverse.
		// ** i0..i3 are y7,y5,y3,y1 respecttively.
		lTmp0 = (LONG)lpiPtr[7];
		lTmp1 = (LONG)lpiPtr[5];
		lTmp2 = (LONG)lpiPtr[3];
		lTmp3 = (LONG)lpiPtr[1];

		lTmpZ1 = lTmp0 + lTmp3;
		lTmpZ2 = lTmp1 + lTmp2;
		lTmpZ3 = lTmp0 + lTmp2;
		lTmpZ4 = lTmp1 + lTmp3;
		lTmpZ5 = MULTIPLY(lTmpZ3 + lTmpZ4, FIX_1_175875602); // sqrt(2)*c3;

		lTmp0 = MULTIPLY(lTmp0 , FIX_0_298631336); // sqrt(2)*(-c1 + c3 + c5 - c7);
		lTmp1 = MULTIPLY(lTmp1 , FIX_2_053119869); // sqrt(2)*(c1 + c3 - c5 + c7);
		lTmp2 = MULTIPLY(lTmp2 , FIX_3_072711026); // sqrt(2)*(c1 + c3 + c5 - c7);
		lTmp3 = MULTIPLY(lTmp3 , FIX_1_501321110); // sqrt(2)*(c1 + c3 - c5 - c7);
		lTmpZ1 = MULTIPLY(lTmpZ1 , -FIX_0_899976223); // sqrt(2)*( c7-c3);
		lTmpZ2 = MULTIPLY(lTmpZ2 , -FIX_2_562915447); // sqrt(2)*(-c1 - c3);
		lTmpZ3 = MULTIPLY(lTmpZ3 , -FIX_1_961570560); // sqrt(2)*(- c3 - c5);
		lTmpZ4 = MULTIPLY(lTmpZ4 , -FIX_0_390180644); // sqrt(2)*( c5-c3);

		lTmpZ3 += lTmpZ5;
		lTmpZ4 += lTmpZ5;

		lTmp0 += (lTmpZ1 + lTmpZ3);
		lTmp1 += (lTmpZ2 + lTmpZ4);
		lTmp2 += (lTmpZ2 + lTmpZ3);
		lTmp3 += (lTmpZ1 + lTmpZ4);

		// ** Final output stage: inputs are lTmp10..iTmp13, lTmp0..lTmp3;

		lpiPtr[0] = (int)DESCALE(lTmp10 + lTmp3,CONST_BITS-PASS1_BITS);
		lpiPtr[7] = (int)DESCALE(lTmp10 - lTmp3,CONST_BITS-PASS1_BITS);
		lpiPtr[1] = (int)DESCALE(lTmp11 + lTmp2,CONST_BITS-PASS1_BITS);
		lpiPtr[6] = (int)DESCALE(lTmp11 - lTmp2,CONST_BITS-PASS1_BITS);
		lpiPtr[2] = (int)DESCALE(lTmp12 + lTmp1,CONST_BITS-PASS1_BITS);
		lpiPtr[5] = (int)DESCALE(lTmp12 - lTmp1,CONST_BITS-PASS1_BITS);
		lpiPtr[3] = (int)DESCALE(lTmp13 + lTmp0,CONST_BITS-PASS1_BITS);
		lpiPtr[4] = (int)DESCALE(lTmp13 - lTmp0,CONST_BITS-PASS1_BITS);

		lpiPtr += DCT_SIZE; // Advance lpiPtr to next row
	}
	// ** Pass 2: Process columns
	lpiPtr = (LPMYWORD)Block;
	for(i=0; i<DCT_SIZE; i++)
	{
		// ** Even part: reverse the even part of the forward DCT
		// ** The rotator is sqrt(2) * (-c6).
		lTmpZ2 = (LONG)lpiPtr[16];
		lTmpZ3 = (LONG)lpiPtr[48];

		lTmpZ1 = MULTIPLY(lTmpZ2 + lTmpZ3, FIX_0_541196100);
		lTmp2 = MULTIPLY(lTmpZ3, -FIX_1_847759065) + lTmpZ1;
		lTmp3 = MULTIPLY(lTmpZ2, FIX_0_765366865) + lTmpZ1;

		lTmp0 = ((LONG)lpiPtr[0] + (LONG)lpiPtr[32]) << CONST_BITS;
		lTmp1 = ((LONG)lpiPtr[0] - (LONG)lpiPtr[32]) << CONST_BITS;

		lTmp10 = lTmp0 + lTmp3;
		lTmp13 = lTmp0 - lTmp3;
		lTmp11 = lTmp1 + lTmp2;
		lTmp12 = lTmp1 - lTmp2;

		// ** Odd part per figrue 8; the matrix is unitary and hence its transpose is its inverse.
		// ** i0..i3 are y7,y5,y3,y1 respecttively.
		lTmp0 = (LONG)lpiPtr[56];
		lTmp1 = (LONG)lpiPtr[40];
		lTmp2 = (LONG)lpiPtr[24];
		lTmp3 = (LONG)lpiPtr[8];

		lTmpZ1 = lTmp0 + lTmp3;
		lTmpZ2 = lTmp1 + lTmp2;
		lTmpZ3 = lTmp0 + lTmp2;
		lTmpZ4 = lTmp1 + lTmp3;
		lTmpZ5 = MULTIPLY(lTmpZ3 + lTmpZ4, FIX_1_175875602); // sqrt(2)*c3;

		lTmp0 = MULTIPLY(lTmp0 , FIX_0_298631336); // sqrt(2)*(-c1 + c3 + c5 - c7);
		lTmp1 = MULTIPLY(lTmp1 , FIX_2_053119869); // sqrt(2)*(c1 + c3 - c5 + c7);
		lTmp2 = MULTIPLY(lTmp2 , FIX_3_072711026); // sqrt(2)*(c1 + c3 + c5 - c7);
		lTmp3 = MULTIPLY(lTmp3 , FIX_1_501321110); // sqrt(2)*(c1 + c3 - c5 - c7);
		lTmpZ1 = MULTIPLY(lTmpZ1 , -FIX_0_899976223); // sqrt(2)*( c7-c3);
		lTmpZ2 = MULTIPLY(lTmpZ2 , -FIX_2_562915447); // sqrt(2)*(-c1 - c3);
		lTmpZ3 = MULTIPLY(lTmpZ3 , -FIX_1_961570560); // sqrt(2)*(- c3 - c5);
		lTmpZ4 = MULTIPLY(lTmpZ4 , -FIX_0_390180644); // sqrt(2)*( c5-c3);

		lTmpZ3 += lTmpZ5;
		lTmpZ4 += lTmpZ5;

		lTmp0 += (lTmpZ1 + lTmpZ3);
		lTmp1 += (lTmpZ2 + lTmpZ4);
		lTmp2 += (lTmpZ2 + lTmpZ3);
		lTmp3 += (lTmpZ1 + lTmpZ4);

		// ** Final output stage: inputs are lTmp10..iTmp13, lTmp0..lTmp3;
		lpiPtr[ 0] = (int)DESCALE(lTmp10 + lTmp3,CONST_BITS+PASS1_BITS+3);
		lpiPtr[56] = (int)DESCALE(lTmp10 - lTmp3,CONST_BITS+PASS1_BITS+3);
		lpiPtr[ 8] = (int)DESCALE(lTmp11 + lTmp2,CONST_BITS+PASS1_BITS+3);
		lpiPtr[48] = (int)DESCALE(lTmp11 - lTmp2,CONST_BITS+PASS1_BITS+3);
		lpiPtr[16] = (int)DESCALE(lTmp12 + lTmp1,CONST_BITS+PASS1_BITS+3);
		lpiPtr[40] = (int)DESCALE(lTmp12 - lTmp1,CONST_BITS+PASS1_BITS+3);
		lpiPtr[24] = (int)DESCALE(lTmp13 + lTmp0,CONST_BITS+PASS1_BITS+3);
		lpiPtr[32] = (int)DESCALE(lTmp13 - lTmp0,CONST_BITS+PASS1_BITS+3);

		lpiPtr ++; // Advance lpiPtr to next row
	}
}






