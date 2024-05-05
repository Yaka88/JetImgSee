// ExportTGA.cpp: implementation of the CExportTGA class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// 文件名	: ExportTGA.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为TGA的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "ExportTGA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int iReturn;
CExportTGA::CExportTGA()
{

}

CExportTGA::~CExportTGA()
{

}
// 功能描述	: 保存HDIB为TGA文件
// 参数		: HDIB hInBmp,图像句柄 LPSTR lpszDstFName目标文件BOOL p_gbRLE,是否RLE压缩
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:
int CExportTGA::iExportTGA(HDIB hInBmp, LPSTR lpszDstFName,BOOL p_gbRLE)
{
   BITMAPINFOHEADER   BIH;
   TGAHEADER          TGAH;
   TGAC_VAR           TGACVar;
   RGBQUAD            TmpPal[256];
   WORD               wColors;
   WORD               wWidthBytes;
   WORD               wTemp;
   WORD               wi;
   BYTE               byTGA_Pal[768];
   CFile*			  m_TempFile = new CFile();
   CFileException     m_error;
	gbRLE = p_gbRLE;
   LPBYTE lpInBmp = (LPBYTE)::GlobalLock(hInBmp);
	if (lpInBmp == NULL)
	{
		iReturn = GLOBALLOCKERROR;
		return iReturn;
	}
	BIH = *((LPBITMAPINFOHEADER)lpInBmp);
	lpInBmp += sizeof(BITMAPINFOHEADER);

	if(BIH.biCompression != BI_RGB)
	{	AfxMessageBox("该BMP文件已经压缩过!");
		iReturn = BMP_HAVECOMPRESS;	
		return iReturn;
	}
 
   TGACVar.wWidth     = (WORD)BIH.biWidth;
   TGACVar.wDepth     = (WORD)BIH.biHeight;
   TGACVar.wBits      = BIH.biBitCount;
   TGACVar.wLineBytes = (WORD)BYTE_WBYTES( (DWORD)TGACVar.wWidth *
                                           (DWORD)TGACVar.wBits );

   wWidthBytes = (WORD)DWORD_WBYTES( BIH.biWidth * (DWORD)TGACVar.wBits );
   wColors     = IMAGE_COLORS(TGACVar.wBits,1);
   gbGray = (TGACVar.wBits == 8) ;

   if ( wColors )
   {
      /* Read Palette */
      memcpy((LPSTR)TmpPal,lpInBmp,(wColors<<2));
		lpInBmp += (wColors<<2);
      wTemp = 0;
      for(wi=0;wi<wColors;wi++)
      {
         byTGA_Pal[wTemp++] = TmpPal[wi].rgbBlue;
         byTGA_Pal[wTemp++] = TmpPal[wi].rgbGreen;
         byTGA_Pal[wTemp++] = TmpPal[wi].rgbRed;
      }
   }

   if(!m_TempFile->Open(lpszDstFName,CFile::modeCreate | CFile::modeWrite,&m_error))
	{
		m_error.ReportError();
		::GlobalUnlock(hInBmp);
		iReturn = CREATETGA_FAILED;
		return iReturn;
	}
   /* Write The TGA File Header */
   TGAH.byID_Length          = 0x00;
   TGAH.byPalType            = (BYTE)(gbGray ? 0x00 : (wColors ? 0x01 : 0x00));
   if ( gbRLE )
   {
      TGAH.byImageType = (BYTE)(gbGray ? 0x0B : (wColors ? 0x09 : 0x0A));
   }
   else
   {
      TGAH.byImageType = (BYTE)(gbGray ? 0x03 : (wColors ? 0x01 : 0x02));
   }
   TGAH.wPalFirstNdx         = 0;
   TGAH.wPalLength           = (TGAH.byPalType ? 256 : 0);
   TGAH.byPalBits            = (BYTE)(TGAH.byPalType ? 0x18 : 0x00);
   TGAH.wLeft                = 0;
   TGAH.wBottom              = 0;
   TGAH.wWidth               = TGACVar.wWidth;
   TGAH.wDepth               = TGACVar.wDepth;
   TGAH.byBits               = (BYTE)TGACVar.wBits;
   TGAH.Descriptor.AlphaBits = 0x00;
   TGAH.Descriptor.HorMirror = 0x00;
   TGAH.Descriptor.VerMirror = 0x00;
   TGAH.Descriptor.Reserved  = 0x00;
 //  m_TempFile->Write((LPSTR)&TGAH,sizeof(TGAH));//这里同样搞笑！！
   m_TempFile->Write((LPSTR)&TGAH.byID_Length ,1);
   m_TempFile->Write((LPSTR)&TGAH.byPalType ,1);
   m_TempFile->Write((LPSTR)&TGAH.byImageType ,1);
   m_TempFile->Write((LPSTR)&TGAH.wPalFirstNdx ,2);
   m_TempFile->Write((LPSTR)&TGAH.wPalLength ,2);
   m_TempFile->Write((LPSTR)&TGAH.byPalBits ,1);
   m_TempFile->Write((LPSTR)&TGAH.wLeft ,2);
   m_TempFile->Write((LPSTR)&TGAH.wBottom ,2);
   m_TempFile->Write((LPSTR)&TGAH.wWidth ,2);
   m_TempFile->Write((LPSTR)&TGAH.wDepth ,2);
   m_TempFile->Write((LPSTR)&TGAH.byBits ,1);
   m_TempFile->Write((LPSTR)&TGAH.Descriptor ,1);
   if ( wColors && !gbGray )
   {
      /* Write The TGA 256-Color Palette */
      m_TempFile->Write((LPSTR)byTGA_Pal,768);
   }

   iReturn     = iCreateTGA(m_TempFile,lpInBmp,&TGACVar,wWidthBytes);
	m_TempFile->Close();
	::GlobalUnlock(hInBmp);
   return 0;

}
// 功能描述	: 创建TGA文件
// 参数		: CFile *m_DstFile, LPBYTE lpSrcImage, LPTGAC_VAR lpTGACVar, WORD wWidthBytes
//				目标文件            源图像句柄			GIF结构					行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

int CExportTGA::iCreateTGA(CFile *m_DstFile, LPBYTE lpSrcImage, LPTGAC_VAR lpTGACVar, WORD wWidthBytes)
{

   HANDLE hSrcBuff;
   HANDLE hImage;
   HANDLE hTemp;
   LPSTR  lpImage;
   LPSTR  lpTemp;
   WORD   wRunCnt;
   WORD   wi;

   hSrcBuff = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
   lpTGACVar->lpDataBuff = (char*)GlobalLock(hSrcBuff);
   lpTGACVar->lpEndBuff  = lpTGACVar->lpDataBuff;
   lpTGACVar->wByteCnt   = 0;
   hImage   = GlobalAlloc(GHND,(DWORD)wWidthBytes);
   hTemp    = GlobalAlloc(GHND,(DWORD)(wWidthBytes+1));
   lpImage  = (char*)GlobalLock(hImage);
   lpTemp   = (char*)GlobalLock(hTemp);

   for(wi=0;wi<lpTGACVar->wDepth;wi++)
   {
      memcpy(lpImage,lpSrcImage,wWidthBytes);
	  lpSrcImage += wWidthBytes;

      if ( gbRLE )
      {
         if ( lpTGACVar->wBits==8 )
         {
            wRunCnt = m_utility.wCompute8BitRun(lpImage,lpTemp,lpTGACVar->wLineBytes);
         }
         else
         {
            wRunCnt = m_utility.wCompute24BitRun(lpImage,lpTemp,lpTGACVar->wLineBytes);
         }
         iReturn = m_tgaRLE.iEncodeLineTGA_RLE(m_DstFile,lpTGACVar,
                                      lpImage,lpTemp,wRunCnt);
      }
      else
      {
         iReturn = m_tgaRaw.iEncodeLineTGA_RawData(m_DstFile,lpTGACVar,lpImage);
      }
   }

   if ( lpTGACVar->wByteCnt )
   {
      m_DstFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
   }

   GlobalUnlock(hSrcBuff);
   GlobalUnlock(hImage);
   GlobalUnlock(hTemp);
   GlobalFree(hSrcBuff);
   GlobalFree(hImage);
   GlobalFree(hTemp);
   return 0;
}
