// ImportGIF.cpp: implementation of the CImportGIF class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ImportGIF.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开为GIF的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "ImportGIF.h"
#include <afxtempl.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int iReturn;
extern int g_ImageCount;
extern BYTE g_ImageFormat;
CImportGIF::CImportGIF()
{

}

CImportGIF::~CImportGIF()
{

}
// 功能描述	: 打开GIF文件
// 参数		: LPSTR lpszSrcFName
// 返回值	: HDIB 图像句柄
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
HDIB CImportGIF::iImportGIF(LPSTR lpszSrcFName)
{
   GIFHEADER          GIFH;
   GIFSCRDESC         GIFS;
   GIFIMAGE           GIFI;
   GIFCONTROL         GIFC;
   GIFPLAINTEXT       GIFP;
   GIFAPPLICATION     GIFA;
   GIFD_VAR           GIFDVar;
   PALETTEENTRY       SysPal256[256];
   HANDLE             hBI_Struct;
   HANDLE             hTemp;
   HDC                hDC;
   LPBITMAPINFOHEADER lpBIH;
   LPBITMAPINFO       lpBI;
   LPSTR              lpTemp;
   WORD               wBI_Size;
   WORD               wPalSize;
   WORD               wColors,wWidth,wDepth;
   WORD               wWidthBytes;
   WORD               wTemp;
   WORD               wi;
   BYTE               byLabel;
   BYTE               byBlockSize;
   BYTE               byGIF_Pal[768];
   BYTE               bySysPal16[48] = {    0,  0,  0,  0,  0,128,
                                            0,128,  0,  0,128,128,
                                          128,  0,  0,128,  0,128,
                                          128,128,  0,128,128,128,
                                          192,192,192,  0,  0,255,
                                            0,255,  0,  0,255,255,
                                          255,  0,  0,255,  0,255,
                                          255,255,  0,255,255,255
                                       };
   BYTE               byTemp;
   DWORD i_ImageCount = 0;
   CArray<HGLOBAL,HGLOBAL> a_Hdib;
   CArray<LPBYTE, LPBYTE> a_lpdib;	
   CFile*  m_SrcFile = new CFile();
   CFileException m_error;
   if(!m_SrcFile->Open(lpszSrcFName,CFile::modeRead ,&m_error))
	{
		m_error.ReportError();
		iReturn = READSOURCEIMG_ERR;
		return NULL;
	}
   /* GIF File Header */
   m_SrcFile->Read((LPSTR)&GIFH,sizeof(GIFH));
   
   /* Is a GIF Format? */
   if ( memicmp((LPSTR)GIFH.bySignature,"GIF",3) != 0 )
   {
        m_SrcFile->Close();
		iReturn = NOGIF;
		AfxMessageBox("不是GIF格式!");
		return NULL ;
   }

   if ( (memicmp((LPSTR)GIFH.byVersion,"87a",3) != 0) &&
        (memicmp((LPSTR)GIFH.byVersion,"89a",3) != 0) )
   {
      m_SrcFile->Close();
      iReturn = UNSUPPORT_GIF;
	  AfxMessageBox("不支持的GIF文件格式!");
      return NULL;
   }

   /* GIF Logical Screen Descriptor */
//   m_SrcFile->Read((LPSTR)&GIFS,sizeof(GIFS));
   m_SrcFile->Read((LPSTR)&GIFS.wWidth,2);
   m_SrcFile->Read((LPSTR)&GIFS.wDepth ,2);
   m_SrcFile->Read((LPSTR)&GIFS.GlobalFlag,1);
   m_SrcFile->Read((LPSTR)&GIFS.byBackground ,1);
   m_SrcFile->Read((LPSTR)&GIFS.byAspect ,1);


   GIFDVar.wBits = (WORD)GIFS.GlobalFlag.PalBits + 1;

    if(GIFDVar.wBits > 8)
	{
		m_SrcFile->Close();
		AfxMessageBox("不支持的GIF文件格式!,");
		iReturn = UNSUPPORT_GIF;	
		return NULL;
	}
   if ( GIFS.GlobalFlag.GlobalPal )
   {
      memset((LPSTR)byGIF_Pal,0,768);

      /* Read Global Color Table */
      wPalSize = 3 * (1 << GIFDVar.wBits);
      m_SrcFile->Read((LPSTR)byGIF_Pal,wPalSize);
   }
   GIFDVar.wBits          = ( (GIFDVar.wBits==1) ? 1 :
									  (GIFDVar.wBits<=4) ? 4 : 8 );
   m_SrcFile->Read((LPSTR)&byTemp,1);

   while( TRUE )
   {
      if ( byTemp==0x2C )
      {
         /* GIF Image Descriptor */
		   //m_SrcFile->Read((LPSTR)&GIFI,sizeof(GIFI));
		   m_SrcFile->Read((LPSTR)&GIFI.wLeft ,2);
		   m_SrcFile->Read((LPSTR)&GIFI.wTop ,2);
		   m_SrcFile->Read((LPSTR)&GIFI.wWidth ,2);
		   m_SrcFile->Read((LPSTR)&GIFI.wDepth ,2);
		   m_SrcFile->Read((LPSTR)&GIFI.LocalFlag ,1);

		   if ( GIFI.LocalFlag.LocalPal )
		   {
			  memset((LPSTR)byGIF_Pal,0,768);

			  /* Read Local Color Table */
			  wPalSize      = 3 * (1 << ((WORD)GIFI.LocalFlag.PalBits + 1));
			  m_SrcFile->Read((LPSTR)byGIF_Pal,wPalSize);
		   }
		   GIFDVar.wWidth         = GIFI.wWidth;
		   GIFDVar.wDepth         = GIFI.wDepth;
		   WORD wloWidthBytes = (WORD)DWORD_WBYTES( (DWORD)GIFDVar.wWidth *
											 (DWORD)GIFDVar.wBits );
			if ((i_ImageCount++) == 0)
			{
				wWidth     = GIFI.wWidth;
				wDepth     = GIFI.wDepth;
				wWidthBytes = (WORD)DWORD_WBYTES( (DWORD)GIFDVar.wWidth *
											 (DWORD)GIFDVar.wBits );
			}

		   
		   GIFDVar.wLineBytes     = (WORD)BYTE_WBYTES( (DWORD)GIFDVar.wWidth *
													   (DWORD)GIFDVar.wBits );
		   GIFDVar.bEOF           = FALSE;
		   GIFDVar.bInterlace     = (GIFI.LocalFlag.Interlace ? TRUE : FALSE);

		   
		   HGLOBAL T_Hdib = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,(DWORD)wloWidthBytes * GIFDVar.wDepth) ;
		   LPBYTE T_lpdib = (LPBYTE)::GlobalLock(T_Hdib);
		   a_Hdib.Add(T_Hdib);
		   a_lpdib.Add(T_lpdib);
		   if (iReturn = iCreateMetaGIF(m_SrcFile,T_lpdib,&GIFDVar,wloWidthBytes))
				{
					AfxMessageBox("创建HDIB出错!");
					m_SrcFile->Close();
					for (DWORD i = 0 ; i < i_ImageCount;i++)
					{
					::GlobalUnlock(a_Hdib[i]);
					::GlobalFree(a_Hdib[i]);	
					}
					return NULL;
				}
		   if (GIFDVar.wWidth != wWidth || GIFDVar.wDepth != wDepth)
		   {
			   HGLOBAL T_Hdib2 = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,(DWORD)wWidthBytes * wDepth) ;
			   LPBYTE T_lpdib2 = (LPBYTE)::GlobalLock(T_Hdib2);
			   memcpy(T_lpdib2,a_lpdib[i_ImageCount-2],(DWORD)wWidthBytes * wDepth);
			   a_Hdib[i_ImageCount -1] = T_Hdib2;
			   a_lpdib[i_ImageCount -1] = T_lpdib2;
			   DWORD wTop = (wDepth - GIFI.wTop - GIFI.wDepth) * wWidthBytes + GIFI.wLeft * GIFDVar.wBits/8 ;
			   for (int k = 0 ; k < GIFDVar.wDepth ; k++)
			   {
				   memcpy(T_lpdib2 + wTop, T_lpdib + k * wloWidthBytes,wloWidthBytes);
				   wTop += wWidthBytes;
			   }
			   ::GlobalUnlock(T_Hdib);
			   ::GlobalFree(T_Hdib);

		   }

	  }
      else if ( byTemp==0x21 )
      {
         /* GIF Extension */
         hTemp  = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
         lpTemp = (char *)GlobalLock(hTemp);

         m_SrcFile->Read((LPSTR)&byLabel,1);

         switch(byLabel)
         {
           case 0xF9:
           {
              /* Graphic Control Extension */
              //m_SrcFile->Read((LPSTR)&GIFC,sizeof(GIFC));
			  m_SrcFile->Read((LPSTR)&GIFC.byBlockSize,1); 
			  m_SrcFile->Read((LPSTR)&GIFC.Flag ,1);
			  m_SrcFile->Read((LPSTR)&GIFC.wDelayTime,2);
			  m_SrcFile->Read((LPSTR)&GIFC.byTransparencyIndex,1);
			  m_SrcFile->Read((LPSTR)&GIFC.byTerminator ,1);
              break;
           }
           case 0x01:
           {
              /* Plain Text Extension */
              //m_SrcFile->Read((LPSTR)&GIFP,sizeof(GIFP));
			   m_SrcFile->Read((LPSTR)&GIFP.byBlockSize ,1);
			   m_SrcFile->Read((LPSTR)&GIFP.wTextGridLeft ,2);
			   m_SrcFile->Read((LPSTR)&GIFP.wTextGridTop ,2);
			   m_SrcFile->Read((LPSTR)&GIFP.wTextGridWidth ,2);
			   m_SrcFile->Read((LPSTR)&GIFP.wTextGridDepth ,2);
			   m_SrcFile->Read((LPSTR)&GIFP.byCharCellWidth ,1);
			   m_SrcFile->Read((LPSTR)&GIFP.byCharCellDepth ,1);
			   m_SrcFile->Read((LPSTR)&GIFP.byForeColorIndex ,1);
			   m_SrcFile->Read((LPSTR)&GIFP.byBackColorIndex ,1);

              m_SrcFile->Read((LPSTR)&byBlockSize,1);
              while( byBlockSize )
              {
                 m_SrcFile->Read(lpTemp,byBlockSize);
                 m_SrcFile->Read((LPSTR)&byBlockSize,1);
              }
              break;
           }
           case 0xFE:
           {
              /* Comment Extension */
              m_SrcFile->Read((LPSTR)&byBlockSize,1);
              while( byBlockSize )
              {
                 m_SrcFile->Read(lpTemp,byBlockSize);
                 m_SrcFile->Read((LPSTR)&byBlockSize,1);
              }
              break;
           }
           case 0xFF:
           {
              /* Application Extension */
              m_SrcFile->Read((LPSTR)&GIFA,sizeof(GIFA));

              m_SrcFile->Read((LPSTR)&byBlockSize,1);
              while( byBlockSize )
              {
                 m_SrcFile->Read(lpTemp,byBlockSize);
                 m_SrcFile->Read((LPSTR)&byBlockSize,1);
              }
              break;
           }
           default:
           {
              /* Skip Unknown Extension */
              m_SrcFile->Read((LPSTR)&byBlockSize,1);
              while( byBlockSize )
              {
                 m_SrcFile->Read(lpTemp,byBlockSize);
                 m_SrcFile->Read((LPSTR)&byBlockSize,1);
              }
              break;
           }
         }

         GlobalUnlock(hTemp);
         GlobalFree(hTemp);
      }
	  else if ( byTemp==0x3B )
	  {
		  break;
	  }
	  else
	  {
		m_SrcFile->Close();
		AfxMessageBox("不支持的GIF文件格式!,");
		iReturn = UNSUPPORT_GIF;	
		for (DWORD i = 0 ; i < i_ImageCount;i++)
		{
					::GlobalUnlock(a_Hdib[i]);
					::GlobalFree(a_Hdib[i]);	
		}
		return NULL;
	  }

	  m_SrcFile->Read((LPSTR)&byTemp,1);
   }

   //////
   wColors     = IMAGE_COLORS(GIFDVar.wBits,1);
   wPalSize    = wColors * sizeof(RGBQUAD);
   wBI_Size    = sizeof(BITMAPINFOHEADER) + wPalSize;
   hBI_Struct  = GlobalAlloc(GHND,(DWORD)wBI_Size);
   lpBIH       = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);
   lpBI        = (LPBITMAPINFO)lpBIH;

   /* Copy Bitmap Information Header Structure */
   lpBIH->biSize          = (DWORD)sizeof(BITMAPINFOHEADER);
   lpBIH->biWidth         = (DWORD)wWidth;
   lpBIH->biHeight        = (DWORD)wDepth * i_ImageCount;
   lpBIH->biPlanes        = 1;
   lpBIH->biBitCount      = GIFDVar.wBits;
   lpBIH->biCompression   = BI_RGB;
   lpBIH->biXPelsPerMeter = 0;
   lpBIH->biYPelsPerMeter = 0;
   lpBIH->biClrUsed       = wColors;
   lpBIH->biSizeImage     = (DWORD)wWidthBytes * lpBIH->biHeight;
   if (i_ImageCount > 1)
	   lpBIH->biClrImportant  = i_ImageCount + 0x80000000;
   else
	   lpBIH->biClrImportant = 0;

	g_ImageCount = i_ImageCount;
	g_ImageFormat = GIF;
   /* Set Palette */
   if ( GIFS.GlobalFlag.GlobalPal || GIFI.LocalFlag.LocalPal )
   {
      wTemp = 0;
      for(wi=0;wi<wColors;wi++)
      {
         lpBI->bmiColors[wi].rgbRed      = byGIF_Pal[wTemp++];
         lpBI->bmiColors[wi].rgbGreen    = byGIF_Pal[wTemp++];
         lpBI->bmiColors[wi].rgbBlue     = byGIF_Pal[wTemp++];
         lpBI->bmiColors[wi].rgbReserved = 0x00;
      }
   }
   else
   {
      switch(wColors)
      {
         case   2:
         {
            lpBI->bmiColors[0].rgbRed          = 0x00;
            lpBI->bmiColors[0].rgbGreen        = 0x00;
            lpBI->bmiColors[0].rgbBlue         = 0x00;
            lpBI->bmiColors[0].rgbReserved     = 0x00;
            lpBI->bmiColors[1].rgbRed          = 0xFF;
            lpBI->bmiColors[1].rgbGreen        = 0xFF;
            lpBI->bmiColors[1].rgbBlue         = 0xFF;
            lpBI->bmiColors[1].rgbReserved     = 0x00;
            break;
         }
         case  16:
         {
            wTemp = 0;
            for(wi=0;wi<wColors;wi++)
            {
               lpBI->bmiColors[wi].rgbRed      = bySysPal16[wTemp++];
               lpBI->bmiColors[wi].rgbGreen    = bySysPal16[wTemp++];
               lpBI->bmiColors[wi].rgbBlue     = bySysPal16[wTemp++];
               lpBI->bmiColors[wi].rgbReserved = 0x00;
            }
            break;
         }
         case 256:
         {
            hDC = GetDC(NULL);
            GetPaletteEntries((HPALETTE)hDC,0,256,(LPPALETTEENTRY)SysPal256);
            for(wi=0;wi<wColors;wi++)
            {
               lpBI->bmiColors[wi].rgbRed      = SysPal256[wi].peRed;
               lpBI->bmiColors[wi].rgbGreen    = SysPal256[wi].peGreen;
               lpBI->bmiColors[wi].rgbBlue     = SysPal256[wi].peBlue;
               lpBI->bmiColors[wi].rgbReserved = 0x00;
            }
            ReleaseDC(NULL,hDC);
            break;
         }
      }
   }

    HDIB hOutGIF;
	LPBYTE pOutGIF;
	hOutGIF = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBI->bmiHeader.biSizeImage+wBI_Size);
	pOutGIF = (LPBYTE) ::GlobalLock((HGLOBAL) hOutGIF);	
	if (pOutGIF == NULL)
	{
		AfxMessageBox("申请空间错误!");
		m_SrcFile->Close();
		for (DWORD i = 0 ; i < i_ImageCount;i++)
		{
					::GlobalUnlock(a_Hdib[i]);
					::GlobalFree(a_Hdib[i]);	
		}
		return NULL;
	}
	memcpy(pOutGIF,lpBI,wBI_Size); 
	pOutGIF += wBI_Size;
   /* Write The BMP Info */
   GlobalUnlock(hBI_Struct);
   GlobalFree(hBI_Struct);
	
   for (DWORD j = 0 ; j < i_ImageCount ; j++)
   {
	    memcpy(pOutGIF,a_lpdib[j],(DWORD)wWidthBytes * wDepth);
		::GlobalUnlock(a_Hdib[j]);
		::GlobalFree(a_Hdib[j]);	
	    pOutGIF += wWidthBytes * wDepth;
   }
	/////
    m_SrcFile->Close();
	delete m_SrcFile;
	GlobalUnlock((HGLOBAL) hOutGIF);
	return hOutGIF;
}
// 功能描述	: 创建HDIB文件
// 参数		: CFile *m_SrcFile, LPBYTE lpOUTGIF, LPGIFD_VAR lpGIFDVar, WORD wWidthBytes
//				源文件            图像句柄			GIF结构					行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CImportGIF::iCreateMetaGIF(CFile *m_SrcFile, LPBYTE pOutGIF, LPGIFD_VAR lpGIFDVar, WORD wWidthBytes)
{
   HANDLE hSrcBuff;
   DWORD  dwDataBgn;
   
   dwDataBgn             = (DWORD)m_SrcFile->Seek(0L,CFile::current);
   lpGIFDVar->dwDataLen  = (DWORD)m_SrcFile->Seek(0L,CFile::end);
   lpGIFDVar->dwDataLen -= dwDataBgn;
   m_SrcFile->Seek((LONG)dwDataBgn,CFile::begin);
   lpGIFDVar->wMemLen    = ( (lpGIFDVar->dwDataLen > (DWORD)MAX_BUFF_SIZE) ?
                             (WORD)MAX_BUFF_SIZE : (WORD)lpGIFDVar->dwDataLen );

   hSrcBuff = GlobalAlloc(GHND,(DWORD)lpGIFDVar->wMemLen);
   lpGIFDVar->lpDataBuff = (char *)GlobalLock(hSrcBuff);
   iReturn  = m_utility.iReadSrcData(m_SrcFile,&lpGIFDVar->wMemLen,&lpGIFDVar->dwDataLen,
                           lpGIFDVar->lpDataBuff,&lpGIFDVar->bEOF);
   lpGIFDVar->lpBgnBuff  = lpGIFDVar->lpDataBuff;
   lpGIFDVar->lpEndBuff  = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;

   iReturn  = m_gif_LZW.iDecodeGIF_LZW(m_SrcFile,pOutGIF,lpGIFDVar,wWidthBytes);
   m_SrcFile->Seek((LONG)dwDataBgn + m_gif_LZW.i_Imagesize,CFile::begin);
   GlobalUnlock(hSrcBuff);
   GlobalFree(hSrcBuff);
   return 0;
}
