// TIF_PackBits.cpp: implementation of the CTIF_PackBits class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: TIFFPackbits.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TIFF的Packbits压缩的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "TIFPackBits.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTIF_PackBits::CTIF_PackBits()
{
}

CTIF_PackBits::~CTIF_PackBits()
{

}

extern int iReturn ;

// 功能描述	: TIF的Pack压缩编码
// 参数		:  LPSTR lpTemp, WORD wRunCnt， LPSTR LPImage,图像句柄 LPTIFD_VAR lpTIFDVar, TIF结构
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:
int CTIF_PackBits::iEncodeLineTIF_PackBits(LPTIFC_VAR lpTIFCVar, LPSTR lpImage, LPSTR lpTemp, WORD wRunCnt)
{
	// lpTmep参数存放着一行中每个重复片的字节数(一行一般会有多个重复片.)
	LPSTR lpTempEnd;
	int iCount, iTempCount;

	iCount = 0;
	lpTempEnd = lpTemp + wRunCnt;//lpTemp是一个字符数组，共有wRunCnt个。
	while(lpTemp < lpTempEnd)
	{
		iTempCount = (int)(*lpTemp++ & 0xFF);
		if((iTempCount<3) && ((iCount + iTempCount) <= 128))
			iCount += iTempCount;
		else
		{
			if(iCount)
			{	
				// ** 若图象数据重复长度为iCount,则存入TIFF文件中的长度值为iCount-1;
				*lpTIFCVar->lpEndBuff++ = (BYTE)(iCount-1);
				lpTIFCVar->wByteCount ++;//实际上面写入文件一个字节（即记录不重复数据的长度），

				memcpy(lpTIFCVar->lpEndBuff, lpImage,iCount);
				lpTIFCVar->lpEndBuff += iCount;//上面写入文件一批不重复字节，先放在lpEndBuff->lpDataBuff中。
				lpTIFCVar->wByteCount += (WORD)iCount;
				lpImage += iCount;
				iCount = 0;
			}
			// ** 下面针对图象数据的重复部分。（实际仅需两个字节代表一批重复数据）
			*lpTIFCVar->lpEndBuff++ = (BYTE)(-(iTempCount - 1));//求反加一，即补数。
			lpTIFCVar->wByteCount++;
			*lpTIFCVar->lpEndBuff ++ = *lpImage;
			lpTIFCVar->wByteCount++;
			lpImage += iTempCount;
		}
	}
	if(iCount)
	{
		*lpTIFCVar->lpEndBuff++ = (BYTE)(iCount - 1);
		lpTIFCVar->wByteCount++;
		memcpy(lpTIFCVar->lpEndBuff,lpImage, iCount);
		lpTIFCVar->lpEndBuff += iCount;
		lpTIFCVar->wByteCount += (WORD)iCount;
	}
	return SUCCESS;
}
// 功能描述	: TIF的Pack压缩解码
// 参数		: CFile *m_SrcFile,原文件 LPSTR LPImage,图像句柄 LPTIFD_VAR lpTIFDVar, TIF结构
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CTIF_PackBits::iDecodeLineTIF_PackBits(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, LPSTR lpImage)
{
	LPSTR lpImageEnd;
	WORD wBack;
	BYTE byChar;
	int iCount;

	lpImageEnd = lpImage + lpTIFDVar->wLineBytes;
	while(lpImage < lpImageEnd)
	{
		byChar = *lpTIFDVar->lpBgnBuff++;
		if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
		{
			if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
				return iReturn;
			lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
			lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
		}
		if(byChar & 0x80) //为重复的图象数据部分。
		{
			if(byChar != 0x80)
			{
				iCount = -(int)(char)byChar + 1;
				byChar = *lpTIFDVar ->lpBgnBuff++;
				if((lpTIFDVar->lpBgnBuff ==  lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
				{
					if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
						return iReturn;
					lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
					lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
				}
				memset(lpImage,(int)byChar,iCount);
				lpImage += iCount;
			}
		}
		else //为非重复部分。
		{
			iCount = (int)byChar + 1;
			if((lpTIFDVar->lpBgnBuff + iCount) > lpTIFDVar->lpEndBuff)
			{
				wBack = (WORD)(lpTIFDVar->lpEndBuff - lpTIFDVar->lpBgnBuff);
				m_SrcFile->Seek(-(LONG)wBack,CFile::current);
				lpTIFDVar->dwDataLen =+ (DWORD)wBack;
				if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
					return iReturn;
				lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
				lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
			}
			
			memcpy(lpImage,lpTIFDVar->lpBgnBuff,iCount);
			lpImage += iCount;
			lpTIFDVar->lpBgnBuff += iCount;
			if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
			{
				if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
					return iReturn;
				lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
				lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
			}
		}
	}
	return SUCCESS;
}



