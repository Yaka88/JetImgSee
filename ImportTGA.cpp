// ImportTGA.cpp: implementation of the CImportTGA class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// �ļ���	: ImportTGA.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��ΪTGA��ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "ImportTGA.h"

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
CImportTGA::CImportTGA()
{

}

CImportTGA::~CImportTGA()
{

}
// ��������	: ��TGA�ļ�
// ����		: LPSTR lpszSrcFName
// ����ֵ	: HDIB ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB CImportTGA::iImportTGA(LPSTR lpszSrcFName)
{
   TGAHEADER          TGAH;
   TGAD_VAR           TGADVar;
   HANDLE             hBI_Struct;
   LPBITMAPINFOHEADER lpBIH;
   LPBITMAPINFO       lpBI;
   WORD               wBI_Size;
   WORD               wPalSize;
   WORD               wPalNdx;
   WORD               wPalLen;
   WORD               wPalVal;
   WORD               wWidthBytes;
   WORD               wTemp;
   WORD               wi;
   BYTE               byTGA_Pal[768];
   BYTE               byTmpPal[1024];
   BYTE               byTemp;
   BOOL				  gbGray;	
   CFile*  m_SrcFile = new CFile();
   CFileException m_error;
   if(!m_SrcFile->Open(lpszSrcFName,CFile::modeRead ,&m_error))
	{
		m_error.ReportError();
		iReturn = READSOURCEIMG_ERR;
		return NULL;
	}
   /* TGA File Header */
//   m_SrcFile->Read((LPSTR)&TGAH,sizeof(TGAH));   //������Ц�����һ�����wBottom�ᱻ��������֪��˭�Ĵ�����
   m_SrcFile->Read((LPSTR)&TGAH.byID_Length ,1);
   m_SrcFile->Read((LPSTR)&TGAH.byPalType ,1);
   m_SrcFile->Read((LPSTR)&TGAH.byImageType ,1);
   m_SrcFile->Read((LPSTR)&TGAH.wPalFirstNdx ,2);
   m_SrcFile->Read((LPSTR)&TGAH.wPalLength ,2);
   m_SrcFile->Read((LPSTR)&TGAH.byPalBits ,1);
   m_SrcFile->Read((LPSTR)&TGAH.wLeft ,2);
   m_SrcFile->Read((LPSTR)&TGAH.wBottom ,2);
   m_SrcFile->Read((LPSTR)&TGAH.wWidth ,2);
   m_SrcFile->Read((LPSTR)&TGAH.wDepth ,2);
   m_SrcFile->Read((LPSTR)&TGAH.byBits ,1);
   m_SrcFile->Read((LPSTR)&TGAH.Descriptor ,1);


   if ( (TGAH.byImageType != 0x01) && (TGAH.byImageType != 0x09) &&
        (TGAH.byImageType != 0x02) && (TGAH.byImageType != 0x0A) &&
        (TGAH.byImageType != 0x03) && (TGAH.byImageType != 0x0B) &&
        (TGAH.byPalType   != 0x00) && (TGAH.byPalType   != 0x01) )
   {
      m_SrcFile->Close();
      AfxMessageBox("��֧������TGA�ļ���ʽ");
	  iReturn = UNSUPPORT_TGA;
      return NULL;
   }

   gbGray             = ( (TGAH.byImageType == 0x03) ? TRUE :
                          (TGAH.byImageType == 0x0B) ? TRUE : FALSE );
   TGADVar.wWidth     = TGAH.wWidth;
   TGADVar.wDepth     = TGAH.wDepth;
   TGADVar.wSrcBits   = (WORD)TGAH.byBits;
   TGADVar.wNewBits   = ( (TGADVar.wSrcBits>8) ? 24 : 8 );
   TGADVar.wLineBytes = TGADVar.wWidth * ((TGADVar.wSrcBits+7) >> 3);
   TGADVar.bEOF       = FALSE;
   TGADVar.bRLE       = ( (TGAH.byImageType>3) ? TRUE : FALSE );
   TGADVar.bHorMirror = (TGAH.Descriptor.HorMirror ?  TRUE : FALSE);
   TGADVar.bVerMirror = (TGAH.Descriptor.VerMirror ? FALSE : TRUE );

   /* Skip The ID String */
   m_SrcFile->Seek((LONG)TGAH.byID_Length,CFile::current);

   if ( TGAH.byPalType )
   {
      /* Read Palette */
      wPalLen = TGAH.wPalLength * ((TGAH.byPalBits+7) >> 3);
      m_SrcFile->Read((LPSTR)byTmpPal,wPalLen);

      memset((LPSTR)byTGA_Pal,0,768);
      wPalNdx = TGAH.wPalFirstNdx * 3;
      wTemp   = 0;
      switch(TGAH.byPalBits)
      {
         case 15:
         case 16:
         {
            for(wi=0;wi<TGAH.wPalLength;wi++)
            {
               wPalVal = *(LPWORD)&byTmpPal[wTemp];
               byTGA_Pal[wPalNdx++] = (BYTE)( ((wPalVal    ) & 0x1F)<<3 );
               byTGA_Pal[wPalNdx++] = (BYTE)( ((wPalVal>> 5) & 0x1F)<<3 );
               byTGA_Pal[wPalNdx++] = (BYTE)( ((wPalVal>>10) & 0x1F)<<3 );
               wTemp  += 2;
            }
            break;
         }
         case 24:
         {
            for(wi=0;wi<TGAH.wPalLength;wi++)
            {
               byTGA_Pal[wPalNdx++] = byTmpPal[wTemp++];
               byTGA_Pal[wPalNdx++] = byTmpPal[wTemp++];
               byTGA_Pal[wPalNdx++] = byTmpPal[wTemp++];
            }
            break;
         }
         case 32:
         {
            for(wi=0;wi<TGAH.wPalLength;wi++)
            {
               byTGA_Pal[wPalNdx++] = byTmpPal[wTemp++];
               byTGA_Pal[wPalNdx++] = byTmpPal[wTemp++];
               byTGA_Pal[wPalNdx++] = byTmpPal[wTemp++];
               wTemp  ++;
            }
            break;
         }
      }
   }
   else
   {
      TGAH.wPalFirstNdx = 0;
      TGAH.wPalLength   = (gbGray ? 256 : 0);
   }

   wWidthBytes = (WORD)DWORD_WBYTES( (DWORD)TGADVar.wWidth *
                                     (DWORD)TGADVar.wNewBits );
   wPalSize    = (TGAH.wPalFirstNdx + TGAH.wPalLength) * sizeof(RGBQUAD);
   wBI_Size    = sizeof(BITMAPINFOHEADER) + wPalSize;
   hBI_Struct  = GlobalAlloc(GHND,(DWORD)wBI_Size);
   lpBIH       = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);
   lpBI        = (LPBITMAPINFO)lpBIH;

   /* Copy Bitmap Information Header Structure */
   lpBIH->biSize          = (DWORD)sizeof(BITMAPINFOHEADER);
   lpBIH->biWidth         = (DWORD)TGADVar.wWidth;
   lpBIH->biHeight        = (DWORD)TGADVar.wDepth;
   lpBIH->biPlanes        = 1;
   lpBIH->biBitCount      = TGADVar.wNewBits;
   lpBIH->biCompression   = BI_RGB;
   lpBIH->biXPelsPerMeter = 0;
   lpBIH->biYPelsPerMeter = 0;
   lpBIH->biClrUsed       = (DWORD)(TGAH.wPalFirstNdx + TGAH.wPalLength);
   lpBIH->biClrImportant  = 0;
   lpBIH->biSizeImage     = (DWORD)wWidthBytes * lpBIH->biHeight;
	g_ImageCount = 1;
	g_ImageFormat = TGA;
   if ( TGAH.byPalType || gbGray )
   {
      /* Set Palette */
      if ( gbGray )
      {
         /* Gray Palette */
         for(wi=0;wi<(WORD)lpBIH->biClrUsed;wi++)
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
         for(wi=0;wi<(WORD)lpBIH->biClrUsed;wi++)
         {
            lpBI->bmiColors[wi].rgbBlue     = byTGA_Pal[wTemp++];
            lpBI->bmiColors[wi].rgbGreen    = byTGA_Pal[wTemp++];
            lpBI->bmiColors[wi].rgbRed      = byTGA_Pal[wTemp++];
            lpBI->bmiColors[wi].rgbReserved = 0x00;
         }
      }
   }
	HDIB hOutTGA;
	LPBYTE pOutTGA;
	hOutTGA = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBI->bmiHeader.biSizeImage+wBI_Size);
	pOutTGA = (LPBYTE) ::GlobalLock((HGLOBAL) hOutTGA);	
	if (pOutTGA == NULL)
	{
		AfxMessageBox("����ռ����!");
		m_SrcFile->Close();
		return NULL;
	}
	memcpy(pOutTGA,lpBI,wBI_Size); 
   /* Write The BMP Info */
 
   GlobalUnlock(hBI_Struct);
   GlobalFree(hBI_Struct);

   iReturn  = iCreateMetaTGA(m_SrcFile,pOutTGA+wBI_Size,&TGADVar,wWidthBytes);
	if (iReturn)
	{
		AfxMessageBox("����HDIB����!");
		m_SrcFile->Close();
		::GlobalUnlock((HGLOBAL) hOutTGA);
		::GlobalFree((HGLOBAL) hOutTGA);	
		return NULL;
	}

    m_SrcFile->Close();
	delete m_SrcFile;
	GlobalUnlock((HGLOBAL) hOutTGA);
	return hOutTGA;
}
// ��������	: ����HDIB�ļ�
// ����		: CFile *m_SrcFile, LPBYTE lpOUTTGA, LPTGAD_VAR lpTGADVar, WORD wWidthBytes
//				Դ�ļ�            ͼ����			GIF�ṹ					���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CImportTGA::iCreateMetaTGA(CFile *m_SrcFile, LPBYTE pOutTGA, LPTGAD_VAR lpTGADVar, WORD wWidthBytes)
{

    HANDLE hSrcBuff;
   HANDLE hImage;
   LPSTR  lpImage;
   DWORD  dwDataBgn;
   DWORD  dwDataNdx;
   DWORD  dwRowNum;
   WORD   wi;

   dwDataBgn             = (DWORD)m_SrcFile->Seek(0L,CFile::current);
   lpTGADVar->dwDataLen  = (DWORD)m_SrcFile->Seek(0L,CFile::end);
   lpTGADVar->dwDataLen -= dwDataBgn;
   m_SrcFile->Seek((LONG)dwDataBgn,CFile::begin);
   lpTGADVar->wMemLen    = ( (lpTGADVar->dwDataLen > (DWORD)MAX_BUFF_SIZE) ?
                             (WORD)MAX_BUFF_SIZE : (WORD)lpTGADVar->dwDataLen );

   hSrcBuff = GlobalAlloc(GHND,(DWORD)lpTGADVar->wMemLen);
   lpTGADVar->lpDataBuff = (char *)GlobalLock(hSrcBuff);
   iReturn  = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,&lpTGADVar->dwDataLen,
                           lpTGADVar->lpDataBuff,&lpTGADVar->bEOF);
   lpTGADVar->lpBgnBuff  = lpTGADVar->lpDataBuff;
   lpTGADVar->lpEndBuff  = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
   hImage   = GlobalAlloc(GHND,(DWORD)wWidthBytes);
   lpImage  = (char *)GlobalLock(hImage);

   dwRowNum = (DWORD)( lpTGADVar->bVerMirror ? 0:(lpTGADVar->wDepth - 1) );
   for(wi=0;wi<lpTGADVar->wDepth;wi++)
   {
      if ( lpTGADVar->bRLE )
      {
         iReturn = m_tgaRLE.iDecodeLineTGA_RLE(m_SrcFile,lpTGADVar,lpImage);
      }
      else
      {
         iReturn = m_tgaRaw.iDecodeLineTGA_RawData(m_SrcFile,lpTGADVar,lpImage);
      }
      if ( lpTGADVar->bHorMirror )
      {
         m_utility.vSuperInvertLine(lpImage,lpTGADVar->wWidth,lpTGADVar->wNewBits);
      }
      dwDataNdx = dwRowNum * (DWORD)wWidthBytes;
 //     _llseek(iTempFile,(LONG)(dwDataBgn+dwDataNdx),SEEK_BGN);
      memcpy(pOutTGA + dwDataNdx,lpImage,wWidthBytes);
      (lpTGADVar->bVerMirror ? dwRowNum++ : dwRowNum--);
   }

   GlobalUnlock(hSrcBuff);
   GlobalUnlock(hImage);
   GlobalFree(hSrcBuff);
   GlobalFree(hImage);
   return 0;
}
