// ExportGIF.cpp: implementation of the ExportGIF class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ExportGif.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为gif的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "ExportGIF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int iReturn;
CExportGIF::CExportGIF()
{

}

CExportGIF::~CExportGIF()
{

}
// 功能描述	: 创建GIF文件
// 参数		: CFile *m_DstFile, LPBYTE lpSrcImage, LPGIFC_VAR lpGIFCVar, WORD wWidthBytes
//				目标文件            源图像句柄			GIF结构					行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CExportGIF::iCreateGIF(CFile *m_DstFile, LPBYTE lpSrcImage, LPGIFC_VAR lpGIFCVar, WORD wWidthBytes)
{
	HANDLE hSrcBuff;
   BYTE   byChar;
 
   hSrcBuff = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
   lpGIFCVar->lpDataBuff = (char *)GlobalLock(hSrcBuff);
   lpGIFCVar->lpEndBuff  = lpGIFCVar->lpDataBuff;
   lpGIFCVar->dwTempCode = 0UL;
   lpGIFCVar->wByteCnt   = 0;
   lpGIFCVar->wBlockNdx  = 1;
   lpGIFCVar->byLeftBits = 0x00;

   iReturn  = m_gif_LZW.iEncodeGIF_LZW(lpSrcImage,m_DstFile,lpGIFCVar,wWidthBytes);

   if ( lpGIFCVar->wByteCnt )
   {
      m_DstFile->Write(lpGIFCVar->lpDataBuff,lpGIFCVar->wByteCnt);
   }

   /* Write The GIF Block Terminator */
   byChar   = 0x00;
   m_DstFile->Write((LPSTR)&byChar,1);

   /* Write The GIF Trailer */
   byChar   = 0x3B;
   m_DstFile->Write((LPSTR)&byChar,1);

   GlobalUnlock(hSrcBuff);
   GlobalFree(hSrcBuff);
   return 0;
}
// 功能描述	: 保存HDIB为GIF文件
// 参数		: HDIB hInBmp,图像句柄 LPSTR lpszDstFName目标文件
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

int CExportGIF::iExportGIF(HDIB hInBmp, LPSTR lpszDstFName)
{
   BITMAPINFOHEADER   BIH;
   GIFHEADER          GIFH;
   GIFSCRDESC         GIFS;
   GIFIMAGE           GIFI;
   GIFC_VAR           GIFCVar;
   RGBQUAD            TmpPal[256];
   WORD               wColors,wWidthBytes,wTemp, wi;
   BYTE               byGIF_Pal[768];
   BYTE               byChar;
   CFile*			  m_TempFile = new CFile();
   CFileException     m_error;
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

   GIFCVar.wWidth     = (WORD)BIH.biWidth;
   GIFCVar.wDepth     = (WORD)BIH.biHeight;
   GIFCVar.wBits      = BIH.biBitCount;
   GIFCVar.wLineBytes = (WORD)BYTE_WBYTES( (DWORD)GIFCVar.wWidth *
                                           (DWORD)GIFCVar.wBits );

   wWidthBytes = (WORD)DWORD_WBYTES( BIH.biWidth * (DWORD)GIFCVar.wBits );
   wColors     = IMAGE_COLORS(GIFCVar.wBits,1);
	
   if(GIFCVar.wBits > 8)
	{	AfxMessageBox("GIF不支持大于256色!,");
		iReturn = UNSUPPORT_GIF;	
		return iReturn;
	}
    memcpy((LPSTR)TmpPal,lpInBmp,(wColors<<2));
	lpInBmp += (wColors<<2);

   wTemp       = 0;
   for(wi=0;wi<wColors;wi++)
   {
      byGIF_Pal[wTemp++] = TmpPal[wi].rgbRed;
      byGIF_Pal[wTemp++] = TmpPal[wi].rgbGreen;
      byGIF_Pal[wTemp++] = TmpPal[wi].rgbBlue;
   }

   if(!m_TempFile->Open(lpszDstFName,CFile::modeCreate | CFile::modeWrite,&m_error))
	{
		m_error.ReportError();
		::GlobalUnlock(hInBmp);
		iReturn = CREATEGIF_FAILED;
		return iReturn;
	}
   /* Write The GIF File Header */
   memcpy(GIFH.bySignature,"GIF",3);
   memcpy(GIFH.byVersion,"87a",3);
   m_TempFile->Write((LPSTR)&GIFH,sizeof(GIFH));

   /* Write The GIF Logical Screen Descriptor */
   GIFS.wWidth               = GIFCVar.wWidth;
   GIFS.wDepth               = GIFCVar.wDepth;
   GIFS.GlobalFlag.PalBits   = (BYTE)(GIFCVar.wBits - 1);
   GIFS.GlobalFlag.SortFlag  = 0x00;
   GIFS.GlobalFlag.ColorRes  = (BYTE)(GIFCVar.wBits - 1);
   GIFS.GlobalFlag.GlobalPal = 0x01;
   GIFS.byBackground         = 0x00;
   GIFS.byAspect             = 0x00;
 //  m_TempFile->Write((LPSTR)&GIFS,sizeof(GIFS));
   m_TempFile->Write((LPSTR)&GIFS.wWidth,2);
   m_TempFile->Write((LPSTR)&GIFS.wDepth ,2);
   m_TempFile->Write((LPSTR)&GIFS.GlobalFlag,1);
   m_TempFile->Write((LPSTR)&GIFS.byBackground ,1);
   m_TempFile->Write((LPSTR)&GIFS.byAspect ,1);
   /* Write The GIF Global Color Table */
   m_TempFile->Write((LPSTR)byGIF_Pal,(wColors*3));

   /* Write The GIF Image Descriptor Separator */
   byChar      = 0x2C;
   m_TempFile->Write((LPSTR)&byChar,1);

   /* Write The GIF Image Descriptor */
   GIFI.wLeft                = 0;
   GIFI.wTop                 = 0;
   GIFI.wWidth               = GIFCVar.wWidth;
   GIFI.wDepth               = GIFCVar.wDepth;
   GIFI.LocalFlag.PalBits    = 0x00;
   GIFI.LocalFlag.Reserved   = 0x00;
   GIFI.LocalFlag.SortFlag   = 0x00;
   GIFI.LocalFlag.Interlace  = (BYTE)(m_gif_LZW.m_bInterlace ? 0x01 : 0x00);
   GIFI.LocalFlag.LocalPal   = 0x00;
 //  m_TempFile->Write((LPSTR)&GIFI,sizeof(GIFI));
   m_TempFile->Write((LPSTR)&GIFI.wLeft ,2);
   m_TempFile->Write((LPSTR)&GIFI.wTop ,2);
   m_TempFile->Write((LPSTR)&GIFI.wWidth ,2);
   m_TempFile->Write((LPSTR)&GIFI.wDepth ,2);
   m_TempFile->Write((LPSTR)&GIFI.LocalFlag ,1);

   iReturn = iCreateGIF(m_TempFile,lpInBmp,&GIFCVar,wWidthBytes);

   m_TempFile->Close();
	::GlobalUnlock(hInBmp);
   return 0;
}
