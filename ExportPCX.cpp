// ExportPCX.cpp: implementation of the CExportPCX class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// 文件名	: ExportPCX.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为PCX的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "ExportPCX.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExportPCX::CExportPCX()
{

}

CExportPCX::~CExportPCX()
{

}
extern int iReturn;
// 功能描述	: 保存HDIB为PCX文件
// 参数		: HDIB hInBmp,图像句柄 LPSTR lpszDstFName目标文件,
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:
int CExportPCX::iExportPCX(HDIB hInBmp,LPSTR lpszDstFName)
{
   BITMAPINFOHEADER   BIH;
   PCXHEADER          PCXH;
   PCXC_VAR           PCXCVar;
   RGBQUAD            TmpPal[256];
   WORD               wColors;
   WORD               wWidthBytes;
   WORD               wTemp;
   WORD               wi;
   BYTE               byPCX_Pal[769];
   CFile*			 m_TempFile = new CFile();
   CFileException m_error;
	
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

   PCXCVar.wWidth     = (WORD)BIH.biWidth;
   PCXCVar.wDepth     = (WORD)BIH.biHeight;
   PCXCVar.wPlanes    = ( (BIH.biBitCount== 4) ? 4 :
                          (BIH.biBitCount==24) ? 3 : 1 );
   PCXCVar.wBits      = BIH.biBitCount;
   PCXCVar.wLineBytes = ( (PCXCVar.wBits>=8) ? m_utility.wRoundUp(PCXCVar.wWidth,2) :
                                               WORD_WBYTES( PCXCVar.wWidth ) );

   wWidthBytes = (WORD)DWORD_WBYTES( BIH.biWidth * (DWORD)PCXCVar.wBits );
   wColors     = IMAGE_COLORS(PCXCVar.wBits,1);

   if ( wColors )
   {
      /* Read Palette */
      memcpy((LPSTR)TmpPal,lpInBmp,(wColors<<2));
	  lpInBmp += (wColors<<2);
      if ( PCXCVar.wBits==8 )
      {
         byPCX_Pal[0] = 0x0C;
         wTemp = 1;
         for(wi=0;wi<wColors;wi++)
         {
            byPCX_Pal[wTemp++] = TmpPal[wi].rgbRed;
            byPCX_Pal[wTemp++] = TmpPal[wi].rgbGreen;
            byPCX_Pal[wTemp++] = TmpPal[wi].rgbBlue;
         }
      }
      else
      {
         memset(PCXH.byPalette,0,48);
         wTemp = 0;
         for(wi=0;wi<wColors;wi++)
         {
            PCXH.byPalette[wTemp++] = TmpPal[wi].rgbRed;
            PCXH.byPalette[wTemp++] = TmpPal[wi].rgbGreen;
            PCXH.byPalette[wTemp++] = TmpPal[wi].rgbBlue;
         }
      }
   }

   	if(!m_TempFile->Open(lpszDstFName,CFile::modeCreate | CFile::modeWrite,&m_error))
	{
		m_error.ReportError();
		::GlobalUnlock(hInBmp);
		iReturn = CREATEPCX_FAILED;
		return iReturn;
	}
   /* Write The PCX File Header */
   PCXH.byManufacturer = PCX_MARK;
   PCXH.byVersion      = 5;
   PCXH.byEncoding     = 1;
   PCXH.byBits         = (BYTE)( (PCXCVar.wBits>= 8) ? 8 : 1 );
   PCXH.wLeft          = 0;
   PCXH.wTop           = 0;
   PCXH.wRight         = PCXCVar.wWidth - 1;
   PCXH.wBottom        = PCXCVar.wDepth - 1;
   PCXH.wXResolution   = (WORD)((BIH.biXPelsPerMeter * 25.4) / 1000 + 0.5);//modify by yaka 20010710
   PCXH.wYResolution   = (WORD)((BIH.biYPelsPerMeter * 25.4) / 1000 + 0.5);
   PCXH.byReserved     = 0;
   PCXH.byPlanes       = (BYTE)PCXCVar.wPlanes;
   PCXH.wLineBytes     = PCXCVar.wLineBytes;
   PCXH.wPaletteType   = ((PCXCVar.wBits == 8) ? 2 : 1);
   PCXH.wScrWidth      = 0;
   PCXH.wScrDepth      = 0;
   memset(PCXH.byFiller,0,54);
   m_TempFile->Write((LPSTR)&PCXH,sizeof(PCXH));

   iReturn  = iCreatePCX(m_TempFile,lpInBmp,&PCXCVar,wWidthBytes);

   if ( PCXCVar.wBits==8 )
   {
      /* Write The PCX 256-Color Palette */
      m_TempFile->Write((LPSTR)byPCX_Pal,769);
   }

   	m_TempFile->Close();
	::GlobalUnlock(hInBmp);
   return 0;
}

// 功能描述	: 创建PCX文件
// 参数		: CFile *m_DstFile, LPBYTE lpSrcImage, LPPCXC_VAR lpPCXCVar, WORD wWidthBytes
//				目标文件            源图像句柄			GIF结构					行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

int CExportPCX::iCreatePCX(CFile* m_DstFile, LPBYTE lpSrcImage,LPPCXC_VAR lpPCXCVar,WORD wWidthBytes)
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
   WORD   wi;
   WORD   wj;
   BYTE   byTemp;
   BYTE   byPlane0;
   BYTE   byPlane1;
   BYTE   byPlane2;
   BYTE   byPlane3;
   
   hSrcBuff   = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
   lpPCXCVar->lpDataBuff = (char*)GlobalLock(hSrcBuff);
   lpPCXCVar->lpEndBuff  = lpPCXCVar->lpDataBuff;
   lpPCXCVar->wByteCnt   = 0;
   hImage     = GlobalAlloc(GHND,(DWORD)wWidthBytes);
   hTemp      = GlobalAlloc(GHND,(DWORD)(wWidthBytes+1));
   lpImage    = (char*)GlobalLock(hImage);
   lpTemp     = (char*)GlobalLock(hTemp);
   lpImageBgn = lpImage;

   //从HDIB底部开始读数据
   lpSrcImage = lpSrcImage + (lpPCXCVar->wDepth - 1)*wWidthBytes;
   for(wi=0;wi<lpPCXCVar->wDepth;wi++)
   {
      memcpy(lpImageBgn,lpSrcImage,wWidthBytes);
	  lpSrcImage -= wWidthBytes;

      switch(lpPCXCVar->wBits)
      {
         case  1:
         case  8:
         {
            memcpy(lpTemp,lpImage,lpPCXCVar->wLineBytes);
            break;
         }
         case  4:
         {
            lpImage  = lpImageBgn;
            lpPlane0 = lpTemp;
            lpPlane1 = lpPlane0 + lpPCXCVar->wLineBytes;
            lpPlane2 = lpPlane1 + lpPCXCVar->wLineBytes;
            lpPlane3 = lpPlane2 + lpPCXCVar->wLineBytes;
            for(wj=0;wj<lpPCXCVar->wLineBytes;wj++)
            {
               byPlane0    = 0x00;
               byPlane1    = 0x00;
               byPlane2    = 0x00;
               byPlane3    = 0x00;

               byTemp      = *lpImage++;
               byPlane3   |= ( (byTemp & 0x80) ? 0x80 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x40) ? 0x80 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x20) ? 0x80 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x10) ? 0x80 : byPlane0 );
               byPlane3   |= ( (byTemp & 0x08) ? 0x40 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x04) ? 0x40 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x02) ? 0x40 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x01) ? 0x40 : byPlane0 );

               byTemp      = *lpImage++;
               byPlane3   |= ( (byTemp & 0x80) ? 0x20 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x40) ? 0x20 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x20) ? 0x20 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x10) ? 0x20 : byPlane0 );
               byPlane3   |= ( (byTemp & 0x08) ? 0x10 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x04) ? 0x10 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x02) ? 0x10 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x01) ? 0x10 : byPlane0 );

               byTemp      = *lpImage++;
               byPlane3   |= ( (byTemp & 0x80) ? 0x08 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x40) ? 0x08 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x20) ? 0x08 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x10) ? 0x08 : byPlane0 );
               byPlane3   |= ( (byTemp & 0x08) ? 0x04 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x04) ? 0x04 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x02) ? 0x04 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x01) ? 0x04 : byPlane0 );

               byTemp      = *lpImage++;
               byPlane3   |= ( (byTemp & 0x80) ? 0x02 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x40) ? 0x02 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x20) ? 0x02 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x10) ? 0x02 : byPlane0 );
               byPlane3   |= ( (byTemp & 0x08) ? 0x01 : byPlane3 );
               byPlane2   |= ( (byTemp & 0x04) ? 0x01 : byPlane2 );
               byPlane1   |= ( (byTemp & 0x02) ? 0x01 : byPlane1 );
               byPlane0   |= ( (byTemp & 0x01) ? 0x01 : byPlane0 );

               *lpPlane0++ = byPlane0;
               *lpPlane1++ = byPlane1;
               *lpPlane2++ = byPlane2;
               *lpPlane3++ = byPlane3;
            }
            break;
         }
         case 24:
         {
            lpImage  = lpImageBgn;
            lpPlane0 = lpTemp;
            lpPlane1 = lpPlane0 + lpPCXCVar->wLineBytes;
            lpPlane2 = lpPlane1 + lpPCXCVar->wLineBytes;
            for(wj=0;wj<lpPCXCVar->wLineBytes;wj++)
            {
               *lpPlane2++ = *lpImage++;
               *lpPlane1++ = *lpImage++;
               *lpPlane0++ = *lpImage++;
            }
            break;
         }
      }
      iReturn = m_pcxRLE.iEncodeLine(m_DstFile,lpPCXCVar,lpTemp);
   }

   if ( lpPCXCVar->wByteCnt )
   {
      m_DstFile->Write(lpPCXCVar->lpDataBuff,lpPCXCVar->wByteCnt);
   }

   GlobalUnlock(hSrcBuff);
   GlobalUnlock(hImage);
   GlobalUnlock(hTemp);
   GlobalFree(hSrcBuff);
   GlobalFree(hImage);
   GlobalFree(hTemp);
   return 0;
}