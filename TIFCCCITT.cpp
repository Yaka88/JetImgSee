// TIF_CCCITT.cpp: implementation of the CTIF_CCCITT class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: TIFFCCCITT.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TIFF的CCITT编码的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "TIFCCCITT.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef struct HUFFRUN
{
	WORD wCodeWord; //每个白色点行程的码字，这里用十六进制表示，标准是一串长度不同的而进制串；
	WORD wCodeLen; //上述每个码字的实际二进制串的长度；
} HUFFRUN;

HUFFRUN WhiteRun[104] ={ /* White Run Code Words */
	{0x0035, 8},/*	0 */	{0x0007, 6},/*	1 */
	{0x0007, 4},/*	2 */	{0x0008, 4},/*	3 */
	{0x000B, 4},/*	4 */	{0x000C, 4},/*	5 */
	{0x000E, 4},/*	6 */	{0x000F, 4},/*	6 */
	{0x0013, 5},/*	8 */	{0x0014, 5},/*	9 */
	{0x0007, 5},/*	10 */	{0x0008, 5},/*	11 */
	{0x0008, 6},/*	12 */	{0x0003, 6},/*	13 */
	{0x0034, 6},/*	14 */	{0x0035, 6},/*	15 */
	{0x002A, 6},/*	16 */	{0x002B, 6},/*	17 */
	{0x0027, 7},/*	18 */	{0x000C, 7},/*	19 */
	{0x0008, 7},/*	20 */	{0x0017, 7},/*	21 */
	{0x0003, 7},/*	22 */	{0x0004, 7},/*	23 */
	{0x0028, 7},/*	24 */	{0x002B, 7},/*	25 */
	{0x0013, 7},/*	26 */	{0x0024, 7},/*	27 */
	{0x0018, 7},/*	28 */	{0x0002, 8},/*	29 */
	{0x0003, 8},/*	30 */	{0x001A, 8},/*	31 */
	{0x001B, 8},/*	32 */	{0x0012, 8},/*	33 */
	{0x0013, 8},/*	34 */	{0x0014, 8},/*	35 */
	{0x0015, 8},/*	36 */	{0x0016, 8},/*	37 */
	{0x0017, 8},/*	38 */	{0x0028, 8},/*	39 */

	{0x0029, 8},/*	40 */	{0x002A, 8},/*	41 */
	{0x002B, 8},/*	42 */	{0x002C, 8},/*	43 */
	{0x002D, 8},/*	44 */	{0x0004, 8},/*	45 */
	{0x0005, 8},/*	46 */	{0x000A, 8},/*	47 */
	{0x000B, 8},/*	48 */	{0x0052, 8},/*	49 */
	{0x0053, 8},/*	50 */	{0x0054, 8},/*	51 */
	{0x0055, 8},/*	52 */	{0x0024, 8},/*	53 */
	{0x0025, 8},/*	54 */	{0x0058, 8},/*	55 */
	{0x0059, 8},/*	56 */	{0x005A, 8},/*	57 */
	{0x005B, 8},/*	58 */	{0x004A, 8},/*	59 */
	{0x004B, 8},/*	60 */	{0x0032, 8},/*	61 */
	{0x0033, 8},/*	62 */	{0x0034, 8},/*	63 */
	{0x001B, 5},/*	64 */	{0x0012, 5},/*	128 */
	//************************************************ 以上为Terminate Code;
	{0x0017, 6},/*	192 */	{0x0037, 7},/*	256 */
	{0x0036, 8},/*	320 */	{0x0037, 8},/*	384 */
	{0x0064, 8},/*	448 */	{0x0065, 8},/*	512 */
	{0x0068, 8},/*	576 */	{0x0067, 8},/*	640 */
	{0x00CC, 9},/*	704 */	{0x00CD, 9},/*	768 */
	{0x00D2, 9},/*	832 */	{0x00D3, 9},/*	896 */
	{0x00D4, 9},/*	960 */	{0x00D5, 9},/*	1024 */
	{0x00D6, 9},/*	1088 */	{0x00D7, 9},/*	1152 */
	{0x00D8, 9},/*	1216 */	{0x00D9, 9},/*	1280 */
	{0x00DA, 9},/*	1344 */	{0x00DB, 9},/*	1 408 */
	{0x0098, 9},/*	1472 */	{0x0099, 9},/*	1536 */
	{0x009A, 9},/*	1600 */	{0x0018, 6},/*	1644 */
	// ********************************************** 以上为Make_Up Code;
	{0x009B, 9},/*	1728 */	{0x0008, 11},/*	1792 */
	{0x000C, 11},/*	1856 */	{0x000D, 11},/*	1920 */
	{0x0012, 12},/*	1984 */	{0x0013, 12},/*	2048 */
	{0x0014, 12},/*	2112 */	{0x0015, 12},/*	2176 */
	{0x0016, 12},/*	2240 */	{0x0017, 12},/*	2304 */
	{0x001C, 12},/*	2368 */	{0x001D, 12},/*	2432 */
	{0x001E, 12},/*	2496 */	{0x001F, 12},/*	2560 */
	// ********************************************* 以上为 Additonal Make_Up Code;
				};

HUFFRUN BlackRun[104] = {/*Black Run Code Words*/
	{0x0037, 10},/*	0 */	{0x0002, 3},/*	1 */
	{0x0003, 2},/*	2 */	{0x0002, 2},/*	3 */
	{0x0003, 3},/*	4 */	{0x0003, 4},/*	5 */
	{0x0002, 4},/*	6 */	{0x0003, 5},/*	6 */
	{0x0005, 6},/*	8 */	{0x0004, 6},/*	9 */
	{0x0004, 7},/*	10 */	{0x0005, 7},/*	11 */
	{0x0007, 7},/*	12 */	{0x0004, 8},/*	13 */
	{0x0007, 8},/*	14 */	{0x0018, 9},/*	15 */
	{0x0017, 10},/*	16 */	{0x0018, 10},/*	17 */
	{0x0008, 10},/*	18 */	{0x0067, 11},/*	19 */
	{0x0068, 11},/*	20 */	{0x006C, 11},/*	21 */
	{0x0037, 11},/*	22 */	{0x0028, 11},/*	23 */
	{0x0017, 11},/*	24 */	{0x0018, 11},/*	25 */
	{0x00CA, 12},/*	26 */	{0x00CB, 12},/*	27 */
	{0x00CC, 12},/*	28 */	{0x00CD, 12},/*	29 */
	{0x0068, 12},/*	30 */	{0x0069, 12},/*	31 */
	{0x006A, 12},/*	32 */	{0x006B, 12},/*	33 */
	{0x00D2, 12},/*	34 */	{0x00D3, 12},/*	35 */
	{0x00D4, 12},/*	36 */	{0x00D5, 12},/*	37 */
	{0x00D6, 12},/*	38 */	{0x00D7, 12},/*	39 */

	{0x006C, 12},/*	40 */	{0x006D, 12},/*	41 */
	{0x00DA, 12},/*	42 */	{0x00DB, 12},/*	43 */
	{0x0054, 12},/*	44 */	{0x0055, 12},/*	45 */
	{0x0056, 12},/*	46 */	{0x0057, 12},/*	47 */
	{0x0064, 12},/*	48 */	{0x0065, 12},/*	49 */
	{0x0052, 12},/*	50 */	{0x0053, 12},/*	51 */
	{0x0024, 12},/*	52 */	{0x0037, 12},/*	53 */
	{0x0038, 12},/*	54 */	{0x0027, 12},/*	55 */
	{0x0028, 12},/*	56 */	{0x0058, 12},/*	57 */
	{0x0059, 12},/*	58 */	{0x002B, 12},/*	59 */
	{0x002C, 12},/*	60 */	{0x005A, 12},/*	61 */
	{0x0066, 12},/*	62 */	{0x0067, 12},/*	63 */
	{0x000F, 10},/*	64 */	{0x00C8, 12},/*	128 */
	//************************************************ 以上为Terminate Code;
	{0x00C9, 12},/*	192 */	{0x005B, 12},/*	256 */
	{0x0033, 12},/*	320 */	{0x0034, 12},/*	384 */
	{0x0035, 12},/*	448 */	{0x006C, 13},/*	512 */
	{0x006D, 13},/*	576 */	{0x004A, 13},/*	640 */
	{0x004B, 13},/*	704 */	{0x004C, 13},/*	768 */
	{0x004D, 13},/*	832 */	{0x0072, 13},/*	896 */
	{0x0073, 13},/*	960 */	{0x0074, 13},/*	1024 */
	{0x0075, 13},/*	1088 */	{0x0076, 13},/*	1152 */
	{0x0077, 13},/*	1216 */	{0x0052, 13},/*	1280 */
	{0x0053, 13},/*	1344 */	{0x0054, 13},/*	1 408 */
	{0x0055, 13},/*	1472 */	{0x005A, 13},/*	1536 */
	{0x005B, 13},/*	1600 */	{0x0064, 13},/*	1644 */
	// ********************************************** 以上为Make_Up Code;
	{0x0065, 13},/*	1728 */	{0x0008, 11},/*	1792 */
	{0x000C, 11},/*	1856 */	{0x000D, 11},/*	1920 */
	{0x0012, 12},/*	1984 */	{0x0013, 12},/*	2048 */
	{0x0014, 12},/*	2112 */	{0x0015, 12},/*	2176 */
	{0x0016, 12},/*	2240 */	{0x0017, 12},/*	2304 */
	{0x001C, 12},/*	2368 */	{0x001D, 12},/*	2432 */
	{0x001E, 12},/*	2496 */	{0x001F, 12},/*	2560 */
	// ********************************************* 以上为 Additonal Make_Up Code;
				};

int iWhiteRun[216] = {/*White Run Code Words*/
	2,		4,		6,		8,		10,		12,		14,		16,		18,
	20,		22,		24,		26,		28,		30,		32,		34,		36,		
	38,		40,		42,		-2,		-3,		44,		46,		-4,		-5,		
	48,		-6,		-7,		50,		52,		54,		56,		58,		60,			
	62,		-10,	-11,	64,		66,		68,		70,		72,		-128,		
	-8,		-9,		74,		76,		-64,	78,		80,		82,		-13,				
	84,		86,		88,		-1,		-12,	90,		92,		94,		96,							
	98,		100,	102,	104,	106,	108,	-192,	-1664,	110,							
	112,	114,	-16,	-17,	-14,	-15,	116,	118,	120,						
	-22,	-23,	122,	-20,	124,	126,	128,	-19,	130,		
	132,	-26,	134,	136,	138,	-21,	-28,	140,	142,	
	144,	-27,	146,	148,	-18,	-24,	150,	152,	-25,	
	154,	156,	158,	160,	162,	164,	166,	-256,	168,
	170,	-29,	-30,	-45,	-46,	-47,	-48,	-33,	-34,
	-35,	-36,	-37,	-38,	-31,	-32,	-53,	-54,	-39,	
	-40,	-41,	-42,	-43,	-44,	-61,	-62,	-63,	-0,

	-320,	-384,	-59,	-60,	172,	174,	-49,	-50,	-51,
	-52,	-55,	-56,	-57,	-58,	-448,	-512,	176,	-640,
	-576,	178,	180,	182,	184,	186,	188,	32767,	190,
	192,	-1472,	-1536,	-1600,	-1728,	-704,	-768,	-832,	-896,
	-960,	-1024,	-1088,	-1152,	-1216,	-1280,	-1344,	-1408,	194,	
	32767,	196,	198,	200,	202,	204,	32767,	-1792,	206,
	208,	210,	-1856,	-1920,	212,	214,	32767,	-32766,	-1984,
	-2048,	-2112,	-2176,	-2240,	-2304,	-2368,	-2432,	-2496,	-2560
};


int iBlackRun[216] = {/*Black Run Code Words*/
	2,		4,		6,		8,		-3,		-2,		10,		12,		-1,//9
	-4,		14,		16,		-6,		-5,		18,		20,		22,		-7,//18
	24,		26,		28,		30,		-9,		-8,		32,		34,		36,//27	
	38,		-10,	-11,	40,		-12,	42,		44,		46,		48,//36
	-13,	50,		52,		-14,	54,		56,		58,		32767,	60,	//45
	62,		64,		66,		68,		70,		72,		74,		76,		78,	//54
	-15,	80,		82,		84,		86,		32767,	88,		90,		92,//63
	94,		-18,	96,		98,		100,	102,	104,	106,	-64,//72
	108,	110,	112,	-16,	-17,	114,	116,	118,	120,	//81
	122,	124,	126,	128,	-0,		130,	32767,	-1792,	132,//90
	134,	136,	-1856,	-1920,	138,	140,	142,	144,	146,//99
	148,	150,	-24,	-25,	152,	154,	156,	158,	160,//108
	-23,	162,	164,	166,	168,	170,	172,	174,	176,//117
	178,	180,	-22,	182,	184,	186,	-19,	-20,	188,//126
	190,	192,	-21,	194,	32767,	-32766,	-1984,	-2048,	-2112,//135
	-2176,	-2240,	-2304,	-2368,	-2432,	-2496,	-2560,	-52,	196,//144
	198,	-55,	-56,	200,	202,	-59,	-60,	204,	206,//153
	-320,	-384,	-448,	208,	-53,	-54,	210,	212,	214,//162
	-50,	-51,	-44,	-45,	-46,	-47,	-57,	-58,	-61,//171
	-256,	-48,	-49,	-62,	-63,	-30,	-31,	-32,	-33,//180
	-40,	-41,	-128,	-192,	-26,	-27,	-28,	-29,	-34,//189
	-35,	-36,	-37,	-38,	-39,	-42,	-43,	-640,	-704,//198
	-768,	-832,	-1280,	-1344,	-1408,	-1472,	-1536,	-1600,	-1664,//207
	-1728,	-512,	-576,	-896,	-960,	-1024,	-1088,	-1152,	-1216//216
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTIF_CCCITT::CTIF_CCCITT()
{
}

CTIF_CCCITT::~CTIF_CCCITT()
{

}
extern int iReturn ;
// 功能描述	: TIFF的CCITT HUFF编码
// 参数		: LPTIFC_VAR lpTIFCVar,tiff结构LPSTR lpImage,
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CTIF_CCCITT::iEncodeLineTiff_Huffman(LPTIFC_VAR lpTIFCVar, LPSTR lpImage)
{
	DWORD dwBitCnt, dwWidthCnt, dwTempCnt, dwTempCode;
	WORD  wCodeLen, wTableNdx, wLeftBits, wCodeWord;
	BOOL bWhiteRun;
	BYTE byMask;

	dwWidthCnt = 0UL;
	dwTempCode = 0UL;
	wLeftBits = 0;
	bWhiteRun = TRUE;//总是以白色点开始。
	byMask = 0x80;

	while(dwWidthCnt < lpTIFCVar->dwWidth)
	{
		dwBitCnt = 0UL;
		if(bWhiteRun)
		{
			while((*lpImage & byMask) && (dwWidthCnt < lpTIFCVar->dwWidth))
			{
				dwWidthCnt ++;// 仅针对黑白图象，一个位即代表一个象素点，故dwWidthCnt应加1。
				dwBitCnt ++;// 有多少个连续白点。
				byMask >>= 1;
				if(byMask == 0x00)
				{
					byMask = 0x80;
					lpImage++;
				}
			}
		}
		else
		{
			while(!(*lpImage & byMask) && (dwWidthCnt < lpTIFCVar->dwWidth))
			{
				dwWidthCnt ++;// 仅针对黑白图象，一个位即代表一个象素点，故dwWidthCnt应加1。
				dwBitCnt ++;// 有多少个连续黑点。
				byMask >>= 1;
				if(byMask == 0x00)
				{
					byMask = 0x80;
					lpImage++;
				}
			}
		}
		while(dwBitCnt>2560UL)//最大连续长度
		{
			// ** Write Code Word
			wCodeLen = (bWhiteRun ? WhiteRun[103].wCodeLen : BlackRun[103].wCodeLen);
			wCodeWord = (bWhiteRun ? WhiteRun[103].wCodeWord : BlackRun[103].wCodeWord);
			dwTempCode <<= wCodeLen;//先腾出位置
			dwTempCode |= (DWORD)wCodeWord;//暂时赋值
			wLeftBits += wCodeLen;
			while(wLeftBits >= 8)//此时已凑齐一个字节，可以保存
			{
				// ** Put this char
				*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
				lpTIFCVar->wByteCount++;
				wLeftBits -= 8;
			}
			dwBitCnt -= 2560UL;//还剩下一些零头，留到下一个循环处理
		}
		while(dwBitCnt > 63UL)//位于Make_Up Code区中（该区以64为间隔）
		{
			dwTempCnt = dwBitCnt >> 6;//先看是64的几倍，该值加上63后，作为表的索引
			wTableNdx = (WORD)dwTempCnt + 63;
			// ** Write Code Word
			wCodeLen = (bWhiteRun ? WhiteRun[wTableNdx].wCodeLen : BlackRun[wTableNdx].wCodeLen);
			wCodeWord = (bWhiteRun ? WhiteRun[wTableNdx].wCodeWord : BlackRun[wTableNdx].wCodeWord);
			dwTempCode <<= wCodeLen;
			dwTempCode |= (DWORD)wCodeWord;
			wLeftBits += wCodeLen;
			while(wLeftBits >= 8)//此时已凑齐一个字节，可以保存
			{
				*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
				lpTIFCVar->wByteCount++;
				wLeftBits -= 8;
			}
			dwBitCnt -= (dwTempCnt << 6);//还剩下一些零头，留到下一个循环处理
		}
		// ** Write Code Word (dwBitCnt < 63UL);
		wCodeLen = (bWhiteRun ? WhiteRun[dwBitCnt].wCodeLen : BlackRun[dwBitCnt].wCodeLen);
		wCodeWord = (bWhiteRun ? WhiteRun[dwBitCnt].wCodeWord : BlackRun[dwBitCnt].wCodeWord);
		dwTempCode <<= wCodeLen;
		dwTempCode |= (DWORD)wCodeWord;
		wLeftBits += wCodeLen;
		while(wLeftBits>=8)
		{
			*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
			lpTIFCVar->wByteCount++;
			wLeftBits -= 8;
		}
		bWhiteRun = (bWhiteRun ? FALSE : TRUE);
	}

	if(wLeftBits)//此时可能没有8位
	{
		*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode << (8 - wLeftBits));//结尾可能补零；
		lpTIFCVar->wByteCount++;
	}
	return SUCCESS;
}
// 功能描述	: TIFF的CCITT 编码
// 参数		: LPTIFC_VAR lpTIFCVar,tiff结构LPSTR lpImage,图像句柄DWORD dwStripNums,带数DWORD dwLineNums行数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CTIF_CCCITT::iEncodeGroup3(LPTIFC_VAR lpTIFCVar,LPSTR lpImage,DWORD dwStripNums,DWORD dwLineNums)
{	
	
	if(lpTIFCVar->wEncodeMethod == CN_CCITT1D)
		return(iEncodeLineTiff_Huffman(lpTIFCVar, lpImage));

	BOOL bAddTag = FALSE;//若为FALSE则Group3仅采用一维编码；
	bEolInBound = FALSE;//必须和标签T4Option挂上钩;
	wEol = 0x01; wEolLength = 12;
	if(lpTIFCVar->dwGroupOption & (DWORD)0x01){
		bAddTag = TRUE;
	wEol = 0x03; wEolLength = 13;
	}
	if(lpTIFCVar->dwGroupOption & (DWORD)0x04)
		bEolInBound = TRUE;
	bAddHeader = FALSE;
	bAddRTC = FALSE;

	if((dwLineNums == 0))// && (dwStripNums == 0)
		bAddHeader = TRUE;
	if((dwStripNums == (lpTIFCVar->dwStripLen-1)) && (dwLineNums == (lpTIFCVar->dwRowsPerStrip-1)))
		bAddRTC = TRUE;
	if(lpTIFCVar->wEncodeMethod == CN_Group4Fax)
		return(iEncodeGroup3TwoDline(lpTIFCVar,lpImage,dwLineNums,lpTIFCVar->iLastLineOffset, lpTIFCVar->dwLastNumChange));
	else
	{
		if(bAddTag){
			if(dwLineNums % PAREMETERK)
				return(iEncodeGroup3TwoDline(lpTIFCVar,lpImage,dwLineNums,lpTIFCVar->iLastLineOffset, lpTIFCVar->dwLastNumChange));
			else
				return(iEncodeGroup3OneDline(lpTIFCVar,lpImage,dwLineNums, lpTIFCVar->iLastLineOffset, lpTIFCVar->dwLastNumChange));
		}
		else
			return(iEncodeGroup3OneDline(lpTIFCVar,lpImage,dwLineNums, lpTIFCVar->iLastLineOffset, lpTIFCVar->dwLastNumChange));
	}
}
// 功能描述	: TIFF的CCITT 1D编码
// 参数		: LPTIFC_VAR lpTIFCVar,tiff结构LPSTR lpImage,图像句柄DWORD dwStripNums,int* LastLineOffset,DWORD& dwLastNumChange
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CTIF_CCCITT::iEncodeGroup3OneDline(LPTIFC_VAR lpTIFCVar, LPSTR lpImage,DWORD dwNumInStrip, 
										 int* LastLineOffset,DWORD& dwLastNumChange)
{
    DWORD dwBitCnt, dwWidthCnt, dwTempCnt, dwTempCode;
	WORD  wCodeLen, wTableNdx, wLeftBits, wCodeWord;
	BOOL bWhiteRun = TRUE;//总是以白色点开始。;
	BYTE byMask = 0x80;
	dwLastNumChange = 0;//一行黑白变换数；

	dwWidthCnt = 0UL;
	dwTempCode = 0UL;
	wLeftBits = 0;
	if(!bEolInBound){
		wLeftBits = lpTIFCVar->wLeftBits;
		dwTempCode = lpTIFCVar->dwTempCode;
	}
	
	if(bAddHeader){//0000000000011 or 000000000001 should add to header;
		wLeftBits = wEolLength;
		dwTempCode = (DWORD)wEol;
	}
	while(dwWidthCnt < lpTIFCVar->dwWidth)
	{
		dwBitCnt = 0UL;
		if(bWhiteRun)
		{
			while((*lpImage & byMask) && (dwWidthCnt < lpTIFCVar->dwWidth))
			{
				dwWidthCnt ++;// 仅针对黑白图象，一个位即代表一个象素点，故dwWidthCnt应加1。
				dwBitCnt ++;// 有多少个连续白点。
				byMask >>= 1;
				if(byMask == 0x00)
				{
					byMask = 0x80;
					lpImage++;
				}
			}
			LastLineOffset[dwLastNumChange++] = dwWidthCnt;
		}
		else
		{
			while(!(*lpImage & byMask) && (dwWidthCnt < lpTIFCVar->dwWidth))
			{
				dwWidthCnt ++;// 仅针对黑白图象，一个位即代表一个象素点，故dwWidthCnt应加1。
				dwBitCnt ++;// 有多少个连续黑点。
				byMask >>= 1;
				if(byMask == 0x00)
				{
					byMask = 0x80;
					lpImage++;
				}
			}
			LastLineOffset[dwLastNumChange++] = dwWidthCnt;
		}
		while(dwBitCnt>2560UL)//最大连续长度
		{
			// ** Write Code Word
			wCodeLen = (bWhiteRun ? WhiteRun[103].wCodeLen : BlackRun[103].wCodeLen);
			wCodeWord = (bWhiteRun ? WhiteRun[103].wCodeWord : BlackRun[103].wCodeWord);
			dwTempCode <<= wCodeLen;//先腾出位置
			dwTempCode |= (DWORD)wCodeWord;//暂时赋值
			wLeftBits += wCodeLen;
			while(wLeftBits >= 8)//此时已凑齐一个字节，可以保存
			{
				// ** Put this char
				*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
				lpTIFCVar->wByteCount++;
				wLeftBits -= 8;
			}
			dwBitCnt -= 2560UL;//还剩下一些零头，留到下一个循环处理
		}
		while(dwBitCnt > 63UL)//位于Make_Up Code区中（该区以64为间隔）
		{
			dwTempCnt = dwBitCnt >> 6;//先看是64的几倍，该值加上63后，作为表的索引
			wTableNdx = (WORD)dwTempCnt + 63;
			// ** Write Code Word
			wCodeLen = (bWhiteRun ? WhiteRun[wTableNdx].wCodeLen : BlackRun[wTableNdx].wCodeLen);
			wCodeWord = (bWhiteRun ? WhiteRun[wTableNdx].wCodeWord : BlackRun[wTableNdx].wCodeWord);
			dwTempCode <<= wCodeLen;
			dwTempCode |= (DWORD)wCodeWord;
			wLeftBits += wCodeLen;
			while(wLeftBits >= 8)//此时已凑齐一个字节，可以保存
			{
				// ** Put this char
				*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
				lpTIFCVar->wByteCount++;
				wLeftBits -= 8;
			}
			dwBitCnt -= (dwTempCnt << 6);//还剩下一些零头，留到下一个循环处理
		}
		// ** Write Code Word (dwBitCnt < 63UL);
		wCodeLen = (bWhiteRun ? WhiteRun[dwBitCnt].wCodeLen : BlackRun[dwBitCnt].wCodeLen);
		wCodeWord = (bWhiteRun ? WhiteRun[dwBitCnt].wCodeWord : BlackRun[dwBitCnt].wCodeWord);
		dwTempCode <<= wCodeLen;
		dwTempCode |= (DWORD)wCodeWord;
		wLeftBits += wCodeLen;
		while(wLeftBits>=8)
		{
			*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
			lpTIFCVar->wByteCount++;
			wLeftBits -= 8;
		}
		bWhiteRun = (bWhiteRun ? FALSE : TRUE);
	}
	LastLineOffset[dwLastNumChange] = LastLineOffset[dwLastNumChange-1];//虚拟点；
	LastLineOffset[dwLastNumChange+1] = LastLineOffset[dwLastNumChange-1];//虚拟点；

	// ** 加上填充位和EOL；
	if(bEolInBound)
		vEnGroup3InEnd1(lpTIFCVar,dwTempCode,wLeftBits,dwNumInStrip);
	else
		vEnGroup3InEnd2(lpTIFCVar,dwTempCode,wLeftBits,dwNumInStrip);
	return SUCCESS;
}
// 功能描述	: TIFF的CCITT 2D编码
// 参数		: LPTIFC_VAR lpTIFCVar,tiff结构LPSTR lpImage,图像句柄DWORD dwNumInStrip,int* LastLineOffset,DWORD& dwLastNumChange
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CTIF_CCCITT::iEncodeGroup3TwoDline(LPTIFC_VAR lpTIFCVar, LPSTR lpImage,DWORD dwNumInStrip, 
										 int* iLastLineOffset,DWORD& dwLastNumChange)
{
	#define MODETABSIZE 11
	WORD cTifModeTab[MODETABSIZE] = {0x0001,0x0001,0x0001,0x0003,0x0003,0x0003,0x0002,0x0002,0x0002,0x0001,0x0001};
	WORD cTifModeTabLen[MODETABSIZE] = {4,3,1,3,6,7,3,6,7,7,9};

	DWORD dwBitCnt, dwWidthCnt;
	WORD  wCodeLen,  wCodeWord;
	BOOL bWhiteRun = TRUE;//总是以白色点开始。
	BYTE byMask = 0x80;
	DWORD dwCurNumChange = 0;//一行黑白变换数；
	BOOL bIsGroup3=TRUE;
	dwWidthCnt = 0UL;
	DWORD dwTempCode = 0UL;
	WORD wLeftBits = 0;
	if(!bEolInBound){
		wLeftBits = lpTIFCVar->wLeftBits;
		dwTempCode = lpTIFCVar->dwTempCode;
	}
	int iDistanceA1B1;
	int ia0,ia1, ia2, ib1, ib2;
	int iCurLineOffset[MAXCHANGE];
	unsigned int ii;
	for(ii=0;ii<MAXCHANGE;ii++)
		iCurLineOffset[ii]=0;

	while(dwWidthCnt < lpTIFCVar->dwWidth)
	{
		dwBitCnt = 0UL;
		if(bWhiteRun)
		{
			while((*lpImage & byMask) && (dwWidthCnt < lpTIFCVar->dwWidth))
			{
				dwWidthCnt ++;// 仅针对黑白图象，一个位即代表一个象素点，故dwWidthCnt应加1。
				dwBitCnt ++;// 有多少个连续白点。
				byMask >>= 1;
				if(byMask == 0x00)
				{
					byMask = 0x80;
					lpImage++;
				}
			}
		    iCurLineOffset[dwCurNumChange++] = dwWidthCnt;
		}
		else
		{
			while(!(*lpImage & byMask) && (dwWidthCnt < lpTIFCVar->dwWidth))
			{
				dwWidthCnt ++;// 仅针对黑白图象，一个位即代表一个象素点，故dwWidthCnt应加1。
				dwBitCnt ++;// 有多少个连续黑点。
				byMask >>= 1;
				if(byMask == 0x00)
				{
					byMask = 0x80;
					lpImage++;
				}
			}
		    iCurLineOffset[dwCurNumChange++] = dwWidthCnt;
		}
		bWhiteRun = (bWhiteRun ? FALSE : TRUE);
	}
	iCurLineOffset[dwCurNumChange] = iCurLineOffset[dwCurNumChange-1];//虚拟点；
	iCurLineOffset[dwCurNumChange+1] = iCurLineOffset[dwCurNumChange-1];//虚拟点；
	
	if(lpTIFCVar->wEncodeMethod == CN_Group4Fax)
	{
		wLeftBits = lpTIFCVar->wLeftBits;
		dwTempCode = lpTIFCVar->dwTempCode;
		bIsGroup3 = FALSE;
		if(dwNumInStrip == 0)
		{
			iLastLineOffset[0] = iLastLineOffset[1] = iLastLineOffset[2] = lpTIFCVar->dwWidth;
			dwLastNumChange = 1;
		}
	}
	bWhiteRun = TRUE;
	DWORD dwForCurLineChange = 0; 
	ia0 = 0;//(bWhiteRun)相对于a0点；
	do
	{
	HERE:for(ii=(bWhiteRun ? 0 : 1); ii<=dwLastNumChange; ii+=2)
		{
			if((iLastLineOffset[ii]+1)>ia0)
			{
				ib1 = iLastLineOffset[ii]+1;
				ib2 = iLastLineOffset[ii+1]+1;
				break;
			}
		}
		ia1 = iCurLineOffset[dwForCurLineChange]+1;
		if(ia1>ib2)
		{	
			wCodeWord = cTifModeTab[MODEPASS];
			wCodeLen  = cTifModeTabLen[MODEPASS];
			ia0 = ib2;
			vEnGroup3WriteDate(lpTIFCVar, dwTempCode,wCodeWord, wCodeLen, wLeftBits);
			ASSERT(wLeftBits < 8);
			goto HERE;
		}
		else 
		{
			iDistanceA1B1 = ia1 - ib1;
			switch(iDistanceA1B1)
			{
			case -3: wCodeWord = cTifModeTab[MODEVERL3];
					 wCodeLen  = cTifModeTabLen[MODEVERL3];
					 break; 
			case -2: wCodeWord = cTifModeTab[MODEVERL2];
					 wCodeLen  = cTifModeTabLen[MODEVERL2];
					 break;
			case -1: wCodeWord = cTifModeTab[MODEVERL1];
					 wCodeLen  = cTifModeTabLen[MODEVERL1];
					 break;
			case 0 : wCodeWord = cTifModeTab[MODEVER0];
					 wCodeLen  = cTifModeTabLen[MODEVER0];
					 break;
			case 1 : wCodeWord = cTifModeTab[MODEVERR1];
					 wCodeLen  = cTifModeTabLen[MODEVERR1];
					 break;
			case 2 : wCodeWord = cTifModeTab[MODEVERR2];
					 wCodeLen  = cTifModeTabLen[MODEVERR2];
					 break;
			case 3 : wCodeWord = cTifModeTab[MODEVERR3];
					 wCodeLen  = cTifModeTabLen[MODEVERR3];
					 break;
			default: wCodeWord = cTifModeTab[MODEHOR];
					 wCodeLen  = cTifModeTabLen[MODEHOR];
				     vEnGroup3WriteDate(lpTIFCVar, dwTempCode,wCodeWord, wCodeLen, wLeftBits);
				     ia2 = iCurLineOffset[dwForCurLineChange+1]+1;
					 if(ia0 == 0) 
					 	 ia0 = 1;
					 vEnOneDPartline(lpTIFCVar,dwTempCode,wLeftBits,ia0,ia1,bWhiteRun);
					 bWhiteRun = (bWhiteRun ? FALSE : TRUE); 
					 dwForCurLineChange++;
					 vEnOneDPartline(lpTIFCVar, dwTempCode, wLeftBits,ia1,ia2,bWhiteRun);
					 break;
			}
			bWhiteRun = (bWhiteRun ? FALSE : TRUE);
			ia0=ia1;
			dwForCurLineChange++;
			if((iDistanceA1B1>3) || (iDistanceA1B1<-3))
				ia0=ia2; 
			else
				vEnGroup3WriteDate(lpTIFCVar, dwTempCode,wCodeWord, wCodeLen, wLeftBits);
			ASSERT(wLeftBits < 8);
		}
	}while(dwForCurLineChange<dwCurNumChange);

	dwLastNumChange = dwCurNumChange;
	for(ii=0; ii<=dwCurNumChange+1; ii++)
		iLastLineOffset[ii] = iCurLineOffset[ii];

	ASSERT(wLeftBits < 8);
	if(bIsGroup3)//加上填充位和EOL；
	{
		// ** Put This Char
		if(bEolInBound)//需要使EOL结束于字节边界；
			vEnGroup3InEnd1(lpTIFCVar,dwTempCode,wLeftBits,dwNumInStrip);
		else//不需要使EOL结束于字节边界
			vEnGroup3InEnd2(lpTIFCVar,dwTempCode,wLeftBits,dwNumInStrip);
	}
	else//仅用于Group4;
	{
		lpTIFCVar->wLeftBits = wLeftBits;
		lpTIFCVar->dwTempCode = dwTempCode;
	}
	return SUCCESS;
}

void CTIF_CCCITT::vEnGroup3InEnd1(LPTIFC_VAR lpTIFCVar,DWORD& dwTempCode,WORD& wLeftBits,DWORD dwNumInStrip)
{
	unsigned int ii;
	WORD wEolFillBits = ((wEolLength==13) ? 3 : 4); //当bAddRTC==TRUE时无效；
	WORD wBitCnt=16;//当bAddRTC==TRUE时无效；
	if(bAddRTC)//图象结尾，添加RTC，6个（EOL+1 或 EOL）；
	{		
		wEolFillBits = ((wEolLength==13) ? 2 : 8); 
		if(wLeftBits <= wEolFillBits){
			wBitCnt = wEolFillBits-wLeftBits;
			dwTempCode <<= wBitCnt; wLeftBits += wBitCnt;
		}
		else{
            wBitCnt = 8 - wLeftBits + wEolFillBits;
			dwTempCode <<= wBitCnt;
			wLeftBits += wBitCnt;
		}
		for(ii=0; ii<6 ; ii++){//wLeftBits一定是8的倍数；
			wLeftBits += wEolLength;
			if(wEolLength != 12) wEol = 0x03;
			dwTempCode <<= wEolLength; dwTempCode |= (DWORD)wEol;
			while(wLeftBits>=8){
				*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
				lpTIFCVar->wByteCount++;
				wLeftBits -= 8;
			}
		}
		ASSERT(wLeftBits==0);
	}
	else//图象结尾，不需添加RTC
	{
		if(wLeftBits<=wEolFillBits)
			dwTempCode <<= (wEolFillBits-wLeftBits);
		else 
		{
			dwTempCode <<= (8 - wLeftBits + wEolFillBits);
			wBitCnt = 24;//表明Fill位加上EOL共有3个字节；
		}
		dwTempCode <<= wEolLength;
		if(wEolLength != 12) {
			WORD wParaK = (WORD)((dwNumInStrip+1) % PAREMETERK);
			if(wParaK == 0)
				wEol = 0x03;//下一行采用一维编码；
			else	wEol = 0x02;
		}
		dwTempCode |= (DWORD)wEol;//add the EOL code
		while(wBitCnt >= 8)	// ** Put the Fill bits and EOL;
		{
			*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wBitCnt - 8));
			lpTIFCVar->wByteCount++;
			wBitCnt -= 8;
		}
		ASSERT(wBitCnt==0);
	}
}

void CTIF_CCCITT::vEnGroup3InEnd2(LPTIFC_VAR lpTIFCVar,DWORD& dwTempCode,WORD& wLeftBits,DWORD dwNumInStrip)
{
	if(wLeftBits)//此时可能没有8位
	{
		*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode << (8 - wLeftBits));//结尾可能补零；
		lpTIFCVar->wByteCount++;  wLeftBits = 0;
	}
	unsigned int ii;
	if(bAddRTC){//图象结尾，添加RTC，6个（EOL+1 或 EOL）；
		for(ii=0; ii<6 ; ii++){
			wLeftBits += wEolLength;
			if(wEolLength != 12) wEol = 0x03;
			dwTempCode <<= wEolLength; dwTempCode |= (DWORD)wEol;
			while(wLeftBits>=8){
				*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
				lpTIFCVar->wByteCount++;
				wLeftBits -= 8;
			}
		}
	}
	else{//图象结尾，不需添加RTC
		dwTempCode <<= wEolLength;
		if(wEolLength != 12) {
			WORD wParaK = (WORD)((dwNumInStrip+1) % PAREMETERK);
			if(wParaK == 0)
				wEol = 0x03;//下一行采用一维编码；
			else	wEol = 0x02;
		}
		dwTempCode |= (DWORD)wEol;//add the EOL code
		wLeftBits += wEolLength;
		while(wLeftBits>=8){
			*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
			lpTIFCVar->wByteCount++;
			wLeftBits -= 8;
		}
	}
	if(wLeftBits)//此时可能没有8位
	{
		if(dwNumInStrip == lpTIFCVar->dwRowsPerStrip-1){
			*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode << (8 - wLeftBits));//结尾可能补零；
			lpTIFCVar->wByteCount++;
		}
		else{
			lpTIFCVar->wLeftBits = wLeftBits;
			lpTIFCVar->dwTempCode = dwTempCode;
		}
	}
}

void CTIF_CCCITT::vEnGroup3WriteDate(LPTIFC_VAR lpTIFCVar, DWORD& dwTempCode,WORD wCodeWord, WORD wCodeLen,WORD& wLeftBits)
{
	dwTempCode <<= wCodeLen;
	dwTempCode |= (DWORD)wCodeWord;
	wLeftBits += wCodeLen;
	while(wLeftBits >= 8)//此时已凑齐一个字节，可以保存
	{
		// ** Put this char
		*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
		lpTIFCVar->wByteCount++;
		wLeftBits -= 8;
	}
}

void CTIF_CCCITT::vEnOneDPartline(LPTIFC_VAR lpTIFCVar,DWORD& dwTempCode,WORD& wLeftBits,int iFromPoint,int iToPoint,BOOL bWhiteRun)
{
	WORD wCodeWord, wCodeLen;
	DWORD dwBitCnt = iToPoint - iFromPoint;
	DWORD dwTempCnt; 
	WORD wTableNdx;
	while(dwBitCnt>2560UL)//最大连续长度
	{
		// ** Write Code Word
		wCodeLen = (bWhiteRun ? WhiteRun[103].wCodeLen : BlackRun[103].wCodeLen);
		wCodeWord = (bWhiteRun ? WhiteRun[103].wCodeWord : BlackRun[103].wCodeWord);
		dwTempCode <<= wCodeLen;//先腾出位置
		dwTempCode |= (DWORD)wCodeWord;//暂时赋值
		wLeftBits += wCodeLen;
		while(wLeftBits >= 8)//此时已凑齐一个字节，可以保存
		{
			// ** Put this char
			*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
			lpTIFCVar->wByteCount++;
			wLeftBits -= 8;
		}
		dwBitCnt -= 2560UL;//还剩下一些零头，留到下一个循环处理
	}
	while(dwBitCnt > 63UL)//位于Make_Up Code区中（该区以64为间隔）
	{
		dwTempCnt = dwBitCnt >> 6;//先看是64的几倍，该值加上63后，作为表的索引
		wTableNdx = (WORD)dwTempCnt + 63;
		// ** Write Code Word
		wCodeLen = (bWhiteRun ? WhiteRun[wTableNdx].wCodeLen : BlackRun[wTableNdx].wCodeLen);
		wCodeWord = (bWhiteRun ? WhiteRun[wTableNdx].wCodeWord : BlackRun[wTableNdx].wCodeWord);
		dwTempCode <<= wCodeLen;
		dwTempCode |= (DWORD)wCodeWord;
		wLeftBits += wCodeLen;
		while(wLeftBits >= 8)//此时已凑齐一个字节，可以保存
		{
			// ** Put this char
			*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
			lpTIFCVar->wByteCount++;
			wLeftBits -= 8;
		}
		dwBitCnt -= (dwTempCnt << 6);//还剩下一些零头，留到下一个循环处理
	}
	// ** Write Code Word (dwBitCnt < 63UL);
	wCodeLen = (bWhiteRun ? WhiteRun[dwBitCnt].wCodeLen : BlackRun[dwBitCnt].wCodeLen);
	wCodeWord = (bWhiteRun ? WhiteRun[dwBitCnt].wCodeWord : BlackRun[dwBitCnt].wCodeWord);
	dwTempCode <<= wCodeLen;
	dwTempCode |= (DWORD)wCodeWord;
	wLeftBits += wCodeLen;
	while(wLeftBits>=8)
	{
		*lpTIFCVar->lpEndBuff++ = (BYTE)(dwTempCode >> (wLeftBits - 8));
		lpTIFCVar->wByteCount++;
		wLeftBits -= 8;
	}
}

