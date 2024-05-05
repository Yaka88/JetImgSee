// McsUtil.cpp: implementation of the CMcsUtil class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: McsUtil.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理公共函数的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
//#include "McsUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMcsUtil::CMcsUtil()
{

}

CMcsUtil::~CMcsUtil()
{

}

extern int iReturn;
CMcsUtil m_utility;
// 功能描述	: 交换字节
// 参数		: LPSTR lpSrcPtr, WORD wCount
// 返回值	: void
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
void CMcsUtil::vSwapByte(LPSTR lpSrcPtr, WORD wCount)
{
	union
	{
		WORD wTemp;
		BYTE byTemp[2];
	} SWAPBYTE;
	while( wCount --)
	{
		SWAPBYTE.wTemp = *(LPWORD)lpSrcPtr;
		*lpSrcPtr ++ = SWAPBYTE.byTemp[1];
		*lpSrcPtr ++ = SWAPBYTE.byTemp[0];
	}

}
// 功能描述	: 交换字
// 参数		: LPSTR lpSrcPtr, WORD wCount
// 返回值	: void
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
void CMcsUtil::vSwapWord(LPSTR lpSrcPtr, WORD wCount)
{
	union
	{
		DWORD dwTemp;
		BYTE byTemp[4];
	} SWAPWORD;
	while( wCount --)
	{
		SWAPWORD.dwTemp = *(LPDWORD)lpSrcPtr;
		*lpSrcPtr ++ = SWAPWORD.byTemp[3];
		*lpSrcPtr ++ = SWAPWORD.byTemp[2];
		*lpSrcPtr ++ = SWAPWORD.byTemp[1];
		*lpSrcPtr ++ = SWAPWORD.byTemp[0];

	}
}
WORD CMcsUtil::wRoundUp(WORD wA, WORD wB)
{
	/* Compute a rounded up to next multiple of wB: wA>=0, wB>0 */
	wA += (wB - 1);
	return wA - (wA % wB);
}
// 功能描述	: 读一个数据带的数据,若该数据带大于MAX_BUFF_SIZE,则分批读(以lpTIFDVar->bEOF为标志
// 参数		: CFile *m_SrcFile,原文件 LPDWORD lpdwMemLen,内存长度 LPDWORD lpdwDataLen,数据长度 LPSTR lpSrcBuff, LPBOOL lpbEOF
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CMcsUtil::iReadSrcData(CFile *m_SrcFile, LPWORD lpwMemLen, LPDWORD lpdwDataLen, LPSTR lpSrcBuff, LPBOOL lpbEOF)
{
	if((*lpdwDataLen) > (DWORD)(*lpwMemLen))
		(*lpdwDataLen) -= (DWORD)(*lpwMemLen);
	else
	{
		(*lpwMemLen) = (WORD)(*lpdwDataLen);
		(*lpbEOF) = TRUE;
	}
	try
	{ m_SrcFile->Read(lpSrcBuff,(*lpwMemLen));}
	catch(CFileException e)
	{
		e.ReportError();
		m_SrcFile->Close();
		iReturn = READSOURCEIMG_ERR;
		return iReturn;
	}
	return 0;

}



// 功能描述	: 读一个数据带的数据,若该数据带大于MAX_BUFF_SIZE,则分批读(以lpTIFDVar->bEOF为标志
// 参数		: CFile *m_SrcFile,原文件 LPDWORD lpdwMemLen,内存长度 LPDWORD lpdwDataLen,数据长度 LPSTR lpSrcBuff, LPBOOL lpbEOF
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CMcsUtil::iReadSrcData(CFile *m_SrcFile, LPDWORD lpdwMemLen, LPDWORD lpdwDataLen, LPSTR lpSrcBuff, LPBOOL lpbEOF)
{
	if((*lpdwDataLen) > (*lpdwMemLen))
		(*lpdwDataLen) -= (*lpdwMemLen);
	else
	{
		(*lpdwMemLen) = (*lpdwDataLen);
		(*lpbEOF) = TRUE;
	}
	try
	{ m_SrcFile->Read(lpSrcBuff,(*lpdwMemLen));}
	catch(CFileException e)
	{
		e.ReportError();
		m_SrcFile->Close();
		iReturn = READSOURCEIMG_ERR;
		return iReturn;
	}
	return 0;
}



// 功能描述	: RRR...GGG...BBB...排列方式改成RGBRGBRGBRGB...方式。
// 参数		: LPSTR lpImage，WORD wLineBytes行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CMcsUtil::iDeArrangeRGB(WORD wLineBytes, LPSTR lpImage)
{
	//WORD wLineBytes = lpTIFDVar->wLineBytes;
	HGLOBAL hTempGlobal = GlobalAlloc(GHND,wLineBytes);
	char* lpbDst = (char*)GlobalLock(hTempGlobal);
	char* lpbDstTemp = NULL;
	if(!(lpbDstTemp = lpbDst))
	{
		AfxMessageBox("解码时多位面转换成单位面申请内存失败!");
		iReturn = DERGB_AllocMemERR;
		return iReturn;
	}
	char* lpbTemp = lpImage;
	for(int i=0; i<(wLineBytes/3); i++)
	{
		lpImage = lpbTemp+i;
		for(int j=0; j<3; j++)
		{			
			*lpbDst = *lpImage;
			lpbDst ++;
			lpImage += (wLineBytes/3);

		}
	}	
	lpImage = lpbTemp;
	memcpy(lpImage,lpbDstTemp,wLineBytes); //不能丢，否则下面释放hTempGlobal后即无效.
	GlobalUnlock(hTempGlobal);
	GlobalFree(hTempGlobal);
	return SUCCESS;
}

// 功能描述	: 将RGBRGBRGBRGB...排列方式改成RRR...GGG...BBB...方式。
// 参数		:  LPSTR lpImage，WORD wLineBytes行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CMcsUtil::iEnArrangeRGB(WORD wLineBytes, LPSTR lpImage)
{
	//WORD wLineBytes = lpTIFCVar->wLineBytes;
	HGLOBAL hTempGlobal = GlobalAlloc(GHND,wLineBytes);
	char* lpbDst = (char*)GlobalLock(hTempGlobal);
	char* lpbDstTemp = NULL;
	if(!(lpbDstTemp = lpbDst))
	{
		AfxMessageBox("编码时单位面转换成多位面申请内存失败!");
		iReturn = ENRGB_AllocMemERR;
		return iReturn;
	}
	char* lpbTemp = lpImage;
	for(int i=0; i<3; i++)
	{
		lpImage = lpbTemp+i;
		for(int j=0; j<(wLineBytes/3); j++)
		{
			*lpbDst = *lpImage;
			lpbDst++;
			lpImage += 3;
		}
	}
	lpImage = lpbTemp;
	memcpy(lpImage,lpbDstTemp,wLineBytes); //不能丢，否则下面释放hTempGlobal后即无效.
	GlobalUnlock(hTempGlobal);
	GlobalFree(hTempGlobal);
	return SUCCESS;
}

// 功能描述	: 针对存储的图象是经过水平反转的，解压时调用该函数将图象还原；
// 参数		: LPSTR lpImage,WORD dwWidth,unsigned short BitCount
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
void CMcsUtil::vSuperInvertLine(LPSTR lpImage,WORD dwWidth,unsigned short BitCount)
{
	HGLOBAL hConver = GlobalAlloc(GHND,dwWidth);
	char* lpbTempImg = (char*)GlobalLock(hConver);
	if(lpbTempImg == NULL)
	{
		AfxMessageBox("水平反转图象时申请内存出错!");
		return;
	}
	memcpy(lpbTempImg,lpImage,dwWidth);
	WORD wi;

	switch(BitCount)
	{
	case 1:
		{
			lpImage += (dwWidth-1);
			for(wi=0;wi<dwWidth;wi++) //将一行数据首位颠倒，dwWidth为字节数；
			{
				*lpImage = *lpbTempImg;
				lpImage--;
				lpbTempImg++;
			}
			for(wi=0;wi<dwWidth;wi++) //将一个字节8位颠倒
			{
				unsigned char  tempchar1 ,tempchar2, tempchar3;
				tempchar1 = (tempchar1 & 0x00 );
				tempchar3 = tempchar3 | 0xff;
				for(int i=0;i<=7;i++)
				{
					tempchar2 = lpImage[wi];
					tempchar3 = (tempchar3 >> i);
					tempchar2 = (tempchar2 & tempchar3);
					tempchar2 = (tempchar2 >> (7-i));
					tempchar2 = (tempchar2 << i);
					tempchar1 = (tempchar1 | tempchar2);
					tempchar3 = (tempchar3 | 0xff);
				}
				lpImage[wi] = tempchar1 ;
			} 
			break;
		}
	case 4:
		{
			lpImage += (dwWidth-1);
			for(wi=0;wi<dwWidth;wi++) //将一行数据首位颠倒，dwWidth为字节数；
			{
				*lpImage = *lpbTempImg;
				lpImage--;
				lpbTempImg++;
			}
			for(wi=0;wi<dwWidth;wi++) //将一个字节前后4位颠倒
			{
				unsigned char  tempchar1;
				tempchar1 = lpImage[wi];
				lpImage[wi] = (lpImage[wi] << 4);
				tempchar1 = (tempchar1 >> 4);
				lpImage[wi] |= tempchar1;
			}
			break;
		}
	case 8:
		{
			lpImage += (dwWidth-1);
			for(wi=0;wi<dwWidth;wi++)
			{
				*lpImage = *lpbTempImg;
				lpImage--;
				lpbTempImg++;
			}
			break;
		}
	case 16:
		{
			lpImage += (dwWidth-2);
			for(wi=0;wi<(dwWidth/2);wi++) //将一行数据首位颠倒，dwWidth为字节数；
			{			
				for(int i=0;i<2;i++)
				{

					*lpImage = *lpbTempImg;
					lpImage++;
					lpbTempImg++;
				}
				lpImage -= 4;
			}
			break;
		}
	case 24:
		{
			lpImage += (dwWidth-3);
			for(wi=0;wi<(dwWidth/3);wi++)
			{			
				for(int i=0;i<3;i++)
				{

					*lpImage = *lpbTempImg;
					lpImage++;
					lpbTempImg++;
				}
				lpImage -= 6;
			}
			break;
		}

	default:
		AfxMessageBox("不支持这种位深的图象!");
		break;
	}

	GlobalUnlock(hConver);
	GlobalFree(hConver);
}

//add by yaka 20010608
// 功能描述	: 用于RLE压缩，计算一行图象数据(lpImage中)有多少个重复片,每个重复片的字节个数
// 参数		: LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes
// 返回值	: WORD 重复片个数
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
WORD CMcsUtil::wCompute8BitRun(LPSTR lpImage, LPSTR lpTemp, WORD wLineBytes)
{
	LPSTR lpImageEnd;
   WORD  wRunCnt;
   WORD  wRunLen;

   wRunCnt    = 0;
   lpImageEnd = lpImage + wLineBytes;
   while( lpImage<lpImageEnd )
   {
      wRunLen = 1;
      while( (*lpImage == *(lpImage+1)) &&
             (wRunLen < 128) && ((lpImage+1) < lpImageEnd) )
      {
         wRunLen ++;
         lpImage ++;
      }
      *lpTemp++ = (BYTE)wRunLen;
      wRunCnt  ++;
      lpImage  ++;
   }
   return wRunCnt;
}


// 功能描述	: 用于RLE压缩，计算一行图象数据(lpImage中)有多少个重复片,每个重复片的字节个数
// 参数		: LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes
// 返回值	: WORD 重复片个数
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
//add by yaka 20010608
WORD CMcsUtil::wCompute24BitRun(LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes)
{
   LPSTR lpImageEnd;
   WORD  wRunCnt;
   WORD  wRunLen;

   wRunCnt    = 0;
   lpImageEnd = lpImage + wLineBytes;
   while( lpImage<lpImageEnd )
   {
      wRunLen = 1;
      while( (*(lpImage  ) == *(lpImage+3)) &&
             (*(lpImage+1) == *(lpImage+4)) &&
             (*(lpImage+2) == *(lpImage+5)) &&
             (wRunLen < 128) && ((lpImage+3) < lpImageEnd) )
      {
         wRunLen ++;
         lpImage += 3;
      }
      *lpTemp++ = (BYTE)wRunLen;
      wRunCnt  ++;
      lpImage  += 3;
   }
   return wRunCnt;
}

//add end