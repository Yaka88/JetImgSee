// ImportPCX.cpp: implementation of the CImportPCX class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// 文件名	: ImportPCX.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开为PCX的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "ImportPCX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern int iReturn;
extern int g_ImageCount;
extern BYTE g_ImageFormat;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImportPCX::CImportPCX()
{
}

CImportPCX::~CImportPCX()
{

}

// 功能描述	: 打开PCX文件
// 参数		: LPSTR lpszSrcFName
// 返回值	: HDIB 图像句柄
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
HDIB CImportPCX::iImportPCX(LPSTR lpszSrcFName)
{
   PCXHEADER          PCXH;
   PCXD_VAR           PCXDVar;
   PALETTEENTRY       SysPal256[256];
   HANDLE             hBI_Struct;
   HDC                hDC;
   LPBITMAPINFOHEADER lpBIH;
   LPBITMAPINFO       lpBI;
   WORD               wBI_Size;
   WORD               wPalSize;
   WORD               wColors;
   WORD               wWidthBytes;
   WORD               wTemp;
   WORD               wi;
   BYTE               byPCX_Pal[769];
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
   BOOL	gbGray ;
   CFile*  m_SrcFile = new CFile();

	CFileException m_error;
   if(!m_SrcFile->Open(lpszSrcFName,CFile::modeRead ,&m_error))
	{
		m_error.ReportError();
		iReturn = READSOURCEIMG_ERR;
		return NULL;
	}
   /* PCX File Header */
   m_SrcFile->SeekToBegin();
   m_SrcFile->Read((LPSTR)&PCXH,sizeof(PCXH));

   /* Is a PCX Format? */
   if ( !CHECK_PCX(PCXH.byManufacturer) )
   {
	    m_SrcFile->Close();
		iReturn = NOPCX;
		AfxMessageBox("不是PCX格式!");
		return NULL ;
   }

   if ( (PCXH.byPlanes==3) && (PCXH.byBits!=8) )
   {
      m_SrcFile->Close();
      iReturn = UNSUPPORT_PCX;
	  AfxMessageBox("不支持的PCX文件格式!");
      return NULL;
   }

   gbGray                 = ( (PCXH.wPaletteType==2) ? TRUE : FALSE );
   PCXDVar.wWidth         = PCXH.wRight  - PCXH.wLeft + 1;
   PCXDVar.wDepth         = PCXH.wBottom - PCXH.wTop  + 1;
   PCXDVar.wPlanes        = (WORD)PCXH.byPlanes;
   PCXDVar.wSrcBits       = (WORD)PCXH.byBits;
   PCXDVar.wNewBits       = PCXDVar.wSrcBits * PCXDVar.wPlanes;
   PCXDVar.wLineBytes     = PCXH.wLineBytes;
   PCXDVar.bEOF           = FALSE;

   wWidthBytes = (WORD)DWORD_WBYTES( (DWORD)PCXDVar.wWidth *
                                     (DWORD)PCXDVar.wNewBits );
   wColors     = IMAGE_COLORS(PCXDVar.wNewBits,1);
   wPalSize    = wColors * sizeof(RGBQUAD);
   wBI_Size    = sizeof(BITMAPINFOHEADER) + wPalSize;
   hBI_Struct  = GlobalAlloc(GHND,(DWORD)wBI_Size);
   lpBIH       = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);
   lpBI        = (LPBITMAPINFO)lpBIH;

   /* Copy Bitmap Information Header Structure */
   lpBIH->biSize          = (DWORD)sizeof(BITMAPINFOHEADER);
   lpBIH->biWidth         = (DWORD)PCXDVar.wWidth;
   lpBIH->biHeight        = (DWORD)PCXDVar.wDepth;
   lpBIH->biPlanes        = 1;
   lpBIH->biBitCount      = PCXDVar.wNewBits;
   lpBIH->biCompression   = BI_RGB;
   lpBIH->biXPelsPerMeter = long((PCXH.wXResolution/25.4)*1000 + 0.5); //modify yaka 20010710
   lpBIH->biYPelsPerMeter = long((PCXH.wYResolution/25.4)*1000 + 0.5);
   lpBIH->biClrUsed       = wColors;
   lpBIH->biClrImportant  = 0;
   lpBIH->biSizeImage     = (DWORD)wWidthBytes * lpBIH->biHeight;
	g_ImageCount = 1;
	g_ImageFormat = PCX;
   if ( PCXDVar.wNewBits<=8 )
   {
      /* Set Palette */
      if ( PCXH.byVersion==3 )
      {
         /* Use Default Palette */
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
               if ( gbGray )
               {
                  /* Gray Palette */
                  for(wi=0;wi<wColors;wi++)
                  {
                     byTemp                          = (BYTE)wi;
                     lpBI->bmiColors[wi].rgbRed      = byTemp;
                     lpBI->bmiColors[wi].rgbGreen    = byTemp;
                     lpBI->bmiColors[wi].rgbBlue     = byTemp;
                     lpBI->bmiColors[wi].rgbReserved = 0x00;
                  }
               }
               else
               {
                  /* Color Palette */
                  wTemp = 0;
                  for(wi=0;wi<wColors;wi++)
                  {
                     lpBI->bmiColors[wi].rgbRed      = bySysPal16[wTemp++];
                     lpBI->bmiColors[wi].rgbGreen    = bySysPal16[wTemp++];
                     lpBI->bmiColors[wi].rgbBlue     = bySysPal16[wTemp++];
                     lpBI->bmiColors[wi].rgbReserved = 0x00;
                  }
               }
               break;
            }
            case 256:
            {
               if ( gbGray )
               {
                  /* Gray Palette */
                  for(wi=0;wi<wColors;wi++)
                  {
                     byTemp                          = (BYTE)wi;
                     lpBI->bmiColors[wi].rgbRed      = byTemp;
                     lpBI->bmiColors[wi].rgbGreen    = byTemp;
                     lpBI->bmiColors[wi].rgbBlue     = byTemp;
                     lpBI->bmiColors[wi].rgbReserved = 0x00;
                  }
               }
               else
               {
                  /* Color Palette */
				  hDC = GetDC(NULL);//ghParentWnd,::GetActiveWindow()
				  GetPaletteEntries((HPALETTE)hDC,0,256,(LPPALETTEENTRY)SysPal256);
                  for(wi=0;wi<wColors;wi++)
                  {
                     lpBI->bmiColors[wi].rgbRed      = SysPal256[wi].peRed;
                     lpBI->bmiColors[wi].rgbGreen    = SysPal256[wi].peGreen;
                     lpBI->bmiColors[wi].rgbBlue     = SysPal256[wi].peBlue;
                     lpBI->bmiColors[wi].rgbReserved = 0x00;
                  }
                  ReleaseDC(NULL,hDC);
               }
               break;
            }
         }
      }
      else
      {
         /* Get PCX Palette */
         switch(wColors)
         {
            case   2:
            {
               lpBI->bmiColors[0].rgbRed          = PCXH.byPalette[0];
               lpBI->bmiColors[0].rgbGreen        = PCXH.byPalette[1];
               lpBI->bmiColors[0].rgbBlue         = PCXH.byPalette[2];
               lpBI->bmiColors[0].rgbReserved     = 0x00;
               lpBI->bmiColors[1].rgbRed          = PCXH.byPalette[3];
               lpBI->bmiColors[1].rgbGreen        = PCXH.byPalette[4];
               lpBI->bmiColors[1].rgbBlue         = PCXH.byPalette[5];
               lpBI->bmiColors[1].rgbReserved     = 0x00;
               break;
            }
            case  16:
            {
               wTemp = 0;
               for(wi=0;wi<wColors;wi++)
               {
                  lpBI->bmiColors[wi].rgbRed      = PCXH.byPalette[wTemp++];
                  lpBI->bmiColors[wi].rgbGreen    = PCXH.byPalette[wTemp++];
                  lpBI->bmiColors[wi].rgbBlue     = PCXH.byPalette[wTemp++];
                  lpBI->bmiColors[wi].rgbReserved = 0x00;
               }
               break;
            }
            case 256:
            {
				m_SrcFile->Seek(-769L,CFile::end);
				m_SrcFile->Read((LPSTR)byPCX_Pal,769);
               if ( byPCX_Pal[0] != 0x0C )
               {
                  GlobalUnlock(hBI_Struct);
                  GlobalFree(hBI_Struct);
                  m_SrcFile->Close();
				  iReturn = NOPCXPALETTE;
                  AfxMessageBox("找不到调色板信息");
                  return NULL;
               }
               wTemp = 1;
               for(wi=0;wi<wColors;wi++)
               {
                  /* byPCX_Pal[1] Is The Start of PCX Palette */
                  lpBI->bmiColors[wi].rgbRed      = byPCX_Pal[wTemp++];
                  lpBI->bmiColors[wi].rgbGreen    = byPCX_Pal[wTemp++];
                  lpBI->bmiColors[wi].rgbBlue     = byPCX_Pal[wTemp++];
                  lpBI->bmiColors[wi].rgbReserved = 0x00;
               }
               break;
            }
         }
      }
   }

    HDIB hOutPCX;
	LPBYTE pOutPCX;
	hOutPCX = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBI->bmiHeader.biSizeImage+wBI_Size);
	pOutPCX = (LPBYTE) ::GlobalLock((HGLOBAL) hOutPCX);	
	if (pOutPCX == NULL)
	{
		AfxMessageBox("申请空间错误!");
		m_SrcFile->Close();
		return NULL;
	}
	memcpy(pOutPCX,lpBI,wBI_Size); 
   /* Write The BMP Info */
  
   GlobalUnlock(hBI_Struct);
   GlobalFree(hBI_Struct);

   m_SrcFile->Seek(sizeof(PCXH),CFile::begin); 
   if (iReturn = iCreateMetaPCX(m_SrcFile,pOutPCX+wBI_Size,&PCXDVar,wWidthBytes))
   {
	    AfxMessageBox("创建HDIB出错!");
		m_SrcFile->Close();
		::GlobalUnlock((HGLOBAL) hOutPCX);
		::GlobalFree((HGLOBAL) hOutPCX);	
		return NULL;
	}


	m_SrcFile->Close();
	delete m_SrcFile;
	GlobalUnlock((HGLOBAL) hOutPCX);
	return hOutPCX;
}

// 功能描述	: 创建HDIB文件
// 参数		: CFile *m_SrcFile, LPBYTE lpOUTPCX, LPPCXD_VAR lpPCXDVar, WORD wWidthBytes
//				源文件            图像句柄			GIF结构					行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CImportPCX::iCreateMetaPCX(CFile* m_SrcFile,LPBYTE pOutPCX,
                   LPPCXD_VAR lpPCXDVar,WORD wWidthBytes)
{
   HANDLE hSrcBuff;
   HANDLE hImage;
   HANDLE hTemp;
   LPSTR  lpImage;
   LPSTR  lpTemp;
   LPSTR  lpImageBgn;
   LPSTR  lpPlane0;
   LPSTR  lpPlane1;
   LPSTR  lpPlane2;
   LPSTR  lpPlane3;
   DWORD  dwDataBgn;
   WORD   wi;
   WORD   wj;
   BYTE   byTemp;
   BYTE   byPlane0;
   BYTE   byPlane1;
   BYTE   byPlane2;
   BYTE   byPlane3;
//   int    iRatio;
   iReturn = 0;

   dwDataBgn             = (DWORD)m_SrcFile->Seek(0L,CFile::current);
   lpPCXDVar->dwDataLen  = (DWORD)m_SrcFile->Seek(0L,CFile::end);
   lpPCXDVar->dwDataLen -= dwDataBgn;
   m_SrcFile->Seek((LONG)dwDataBgn,CFile::begin);
   lpPCXDVar->dwDataLen -= ( (lpPCXDVar->wNewBits==8) ? 769 : 0 );
   lpPCXDVar->wMemLen    = ( (lpPCXDVar->dwDataLen > (DWORD)MAX_BUFF_SIZE) ?
                             (WORD)MAX_BUFF_SIZE : (WORD)lpPCXDVar->dwDataLen );

   hSrcBuff   = GlobalAlloc(GHND,(DWORD)lpPCXDVar->wMemLen);
   lpPCXDVar->lpDataBuff = (char*)GlobalLock(hSrcBuff);
   iReturn    = m_utility.iReadSrcData(m_SrcFile,&lpPCXDVar->wMemLen,&lpPCXDVar->dwDataLen,
                             lpPCXDVar->lpDataBuff,&lpPCXDVar->bEOF);
   lpPCXDVar->lpBgnBuff  = lpPCXDVar->lpDataBuff;
   lpPCXDVar->lpEndBuff  = lpPCXDVar->lpBgnBuff + lpPCXDVar->wMemLen;
   hImage     = GlobalAlloc(GHND,(DWORD)wWidthBytes);
   hTemp      = GlobalAlloc(GHND,(DWORD)wWidthBytes);
   lpImage    = (char*)GlobalLock(hImage);
   lpTemp     = (char*)GlobalLock(hTemp);
   lpImageBgn = lpImage;

    //从HDIB底部开始写数据
   pOutPCX = pOutPCX + (lpPCXDVar->wDepth - 1)*wWidthBytes;
  
   for(wi=0;wi<lpPCXDVar->wDepth;wi++)
   {
      iReturn = m_pcxRLE.iDecodeLine(m_SrcFile,lpPCXDVar,lpTemp);
      switch(lpPCXDVar->wNewBits)
      {
         case  1:
         case  8:
         {
            memcpy(lpImage,lpTemp,lpPCXDVar->wLineBytes);
            break;
         }
         case  4:
         {
            lpImage  = lpImageBgn;
            lpPlane0 = lpTemp;
            lpPlane1 = lpPlane0 + lpPCXDVar->wLineBytes;
            lpPlane2 = lpPlane1 + lpPCXDVar->wLineBytes;
            lpPlane3 = lpPlane2 + lpPCXDVar->wLineBytes;
            for(wj=0;wj<lpPCXDVar->wLineBytes;wj++)
            {
               byPlane0   = *lpPlane0++;
               byPlane1   = *lpPlane1++;
               byPlane2   = *lpPlane2++;
               byPlane3   = *lpPlane3++;

               byTemp     = 0x00;
               byTemp    |= ( (byPlane3 & 0x80) ? 0x80 : byTemp );
               byTemp    |= ( (byPlane2 & 0x80) ? 0x40 : byTemp );
               byTemp    |= ( (byPlane1 & 0x80) ? 0x20 : byTemp );
               byTemp    |= ( (byPlane0 & 0x80) ? 0x10 : byTemp );
               byTemp    |= ( (byPlane3 & 0x40) ? 0x08 : byTemp );
               byTemp    |= ( (byPlane2 & 0x40) ? 0x04 : byTemp );
               byTemp    |= ( (byPlane1 & 0x40) ? 0x02 : byTemp );
               byTemp    |= ( (byPlane0 & 0x40) ? 0x01 : byTemp );
               *lpImage++ = byTemp;

               byTemp     = 0x00;
               byTemp    |= ( (byPlane3 & 0x20) ? 0x80 : byTemp );
               byTemp    |= ( (byPlane2 & 0x20) ? 0x40 : byTemp );
               byTemp    |= ( (byPlane1 & 0x20) ? 0x20 : byTemp );
               byTemp    |= ( (byPlane0 & 0x20) ? 0x10 : byTemp );
               byTemp    |= ( (byPlane3 & 0x10) ? 0x08 : byTemp );
               byTemp    |= ( (byPlane2 & 0x10) ? 0x04 : byTemp );
               byTemp    |= ( (byPlane1 & 0x10) ? 0x02 : byTemp );
               byTemp    |= ( (byPlane0 & 0x10) ? 0x01 : byTemp );
               *lpImage++ = byTemp;

               byTemp     = (BYTE)0x00;
               byTemp    |= ( (byPlane3 & 0x08) ? 0x80 : byTemp );
               byTemp    |= ( (byPlane2 & 0x08) ? 0x40 : byTemp );
               byTemp    |= ( (byPlane1 & 0x08) ? 0x20 : byTemp );
               byTemp    |= ( (byPlane0 & 0x08) ? 0x10 : byTemp );
               byTemp    |= ( (byPlane3 & 0x04) ? 0x08 : byTemp );
               byTemp    |= ( (byPlane2 & 0x04) ? 0x04 : byTemp );
               byTemp    |= ( (byPlane1 & 0x04) ? 0x02 : byTemp );
               byTemp    |= ( (byPlane0 & 0x04) ? 0x01 : byTemp );
               *lpImage++ = byTemp;

               byTemp     = 0x00;
               byTemp    |= ( (byPlane3 & 0x02) ? 0x80 : byTemp );
               byTemp    |= ( (byPlane2 & 0x02) ? 0x40 : byTemp );
               byTemp    |= ( (byPlane1 & 0x02) ? 0x20 : byTemp );
               byTemp    |= ( (byPlane0 & 0x02) ? 0x10 : byTemp );
               byTemp    |= ( (byPlane3 & 0x01) ? 0x08 : byTemp );
               byTemp    |= ( (byPlane2 & 0x01) ? 0x04 : byTemp );
               byTemp    |= ( (byPlane1 & 0x01) ? 0x02 : byTemp );
               byTemp    |= ( (byPlane0 & 0x01) ? 0x01 : byTemp );
               *lpImage++ = byTemp;
            }
            break;
         }
         case 24:
         {
            lpImage  = lpImageBgn;
            lpPlane0 = lpTemp;
            lpPlane1 = lpPlane0 + lpPCXDVar->wLineBytes;
            lpPlane2 = lpPlane1 + lpPCXDVar->wLineBytes;
            for(wj=0;wj<lpPCXDVar->wLineBytes;wj++)
            {
               *lpImage++ = *lpPlane2++;
               *lpImage++ = *lpPlane1++;
               *lpImage++ = *lpPlane0++;
            }
            break;
         }
      }
      memcpy(pOutPCX,lpImageBgn,wWidthBytes);
	  pOutPCX -= wWidthBytes;
     // iRatio = (int)( (100L * (LONG)(wi+1)) / (LONG)lpPCXDVar->wDepth );
  
   }

   GlobalUnlock(hSrcBuff);
   GlobalUnlock(hImage);
   GlobalUnlock(hTemp);
   GlobalFree(hSrcBuff);
   GlobalFree(hImage);
   GlobalFree(hTemp);
   return iReturn;
}
