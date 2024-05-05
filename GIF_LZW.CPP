// GIF_LZW.cpp: implementation of the CGIF_LZW class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010618
// 文件名	: GIF_LZW.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: GIF文件的LZW编码解码的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "GIF_LZW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern int iReturn;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGIF_LZW::CGIF_LZW()
{
   m_bInterlace = TRUE;
}

CGIF_LZW::~CGIF_LZW()
{

}
// 功能描述	: GIF的LZW压缩解码
// 参数		: CFile *m_SrcFile,原文件 LPBYTE pOutGIF,图像句柄 LPGIFD_VAR lpGIFDVar, gif结构WORD wWidthBytes行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:
int CGIF_LZW::iDecodeGIF_LZW(CFile *m_SrcFile, LPBYTE pOutGIF, LPGIFD_VAR lpGIFDVar, WORD wWidthBytes)
{
   i_Imagesize = 2;
   HANDLE hPrefix, hSuffix, hStack, hImage;
   LPWORD lpwPrefix;
   LPBYTE lpbySuffix, lpbyStack, lpbyStackBgn;
   LPSTR  lpImage, lpImageBgn;
   DWORD  dwDataNdx, dwData;
   WORD   wCode, wOldCode, wLZW_CLEAR, wLZW_EOI, wNowTableNdx;
   WORD   wTopTableNdx, wTempNdx, wBitCnt, wRowNum, wRowCnt, wWidthCnt,  wi;
   WORD   wIncTable[5]  = { 8,8,4,2,0 }; /* Interlace Pass Increments */
   WORD   wBgnTable[5]  = { 0,4,2,1,0 }; /* Interlace Pass Beginnings */
   WORD   wCodeMask[13] = { 0x0000,
                            0x0001,0x0003,0x0007,0x000F,
                            0x001F,0x003F,0x007F,0x00FF,
                            0x01FF,0x03FF,0x07FF,0x0FFF
                          };
   BYTE   byLZW_MinCodeLen,  byBlockSize;
   BYTE   byBlockNdx, byCurrentBits, byLeftBits, byFirstChar, byTempChar, byCode, byPass;

   hPrefix          = GlobalAlloc(GHND,(DWORD)(MAX_TABLE_SIZE<<1));
   hSuffix          = GlobalAlloc(GHND,(DWORD)MAX_TABLE_SIZE);
   hStack           = GlobalAlloc(GHND,(DWORD)MAX_TABLE_SIZE);
   hImage           = GlobalAlloc(GHND,(DWORD)wWidthBytes);
   lpwPrefix        = (LPWORD)GlobalLock(hPrefix);
   lpbySuffix       = (LPBYTE)GlobalLock(hSuffix);
   lpbyStack        = (LPBYTE)GlobalLock(hStack);
   lpImage          = (char *)GlobalLock(hImage);
   lpbyStackBgn     = lpbyStack;
   lpImageBgn       = lpImage;
	if((lpbyStack==NULL) || (lpwPrefix==NULL) || (lpbySuffix==NULL) || (lpImage == NULL))
	{
		AfxMessageBox("LZW解码申请内存出错!");
		iReturn = DELZW_ALLOCMEMERR;
		return iReturn;
	}
   /* Get GIF LZW Minimum Code Size */
   byLZW_MinCodeLen = *lpGIFDVar->lpBgnBuff++;

   byCurrentBits    = byLZW_MinCodeLen + (BYTE)0x01;
   wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
   wLZW_EOI         = wLZW_CLEAR + 1;
   wNowTableNdx     = wLZW_CLEAR + 2;
   wTopTableNdx     = 1 << byCurrentBits;
   dwData           = 0UL;
   wBitCnt          = lpGIFDVar->wBits;
   wRowNum          = 0;
   wRowCnt          = 1;
   wWidthCnt        = 0;
   wCode            = 0;
   wOldCode         = 0xFFFF;
   byBlockSize      = 0x01;
   byBlockNdx       = 0x00;
   byLeftBits       = 0x00;
   byTempChar       = 0x00;
   byPass           = 0x00;

   /* Get Next Code */
   while( byLeftBits<byCurrentBits )
   {
      /* Get Next Char */
      if ( ++byBlockNdx == byBlockSize )
      {
         /* Get Next Block */
         byBlockSize = *lpGIFDVar->lpBgnBuff++;
		 i_Imagesize += byBlockSize +1;
         byBlockNdx  = 0x00;
         if ( (lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
              !lpGIFDVar->bEOF )
         {
            iReturn =m_utility.iReadSrcData(m_SrcFile,&lpGIFDVar->wMemLen,
                                   &lpGIFDVar->dwDataLen,
                                   lpGIFDVar->lpDataBuff,&lpGIFDVar->bEOF);
            lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
            lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
         }
      }
      byCode      = *lpGIFDVar->lpBgnBuff++;
      dwData     |= ((DWORD)byCode << byLeftBits);
      byLeftBits += 0x08;
      if ( (lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
           !lpGIFDVar->bEOF )
      {
         iReturn = m_utility.iReadSrcData(m_SrcFile,&lpGIFDVar->wMemLen,
                                &lpGIFDVar->dwDataLen,
                                lpGIFDVar->lpDataBuff,&lpGIFDVar->bEOF);
         lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
         lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
      }
   }
   wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
   dwData    >>= byCurrentBits;
   byLeftBits -= byCurrentBits;

   while( wCode!=wLZW_EOI )
   {
      if ( wCode==wLZW_CLEAR )
      {
         /* Initialize The Table */
         for(wi=0;wi<wLZW_CLEAR;wi++)
         {
            *(lpwPrefix+wi)  = 0xFFFF;
            *(lpbySuffix+wi) = (BYTE)wi;
         }
         for(wi=wNowTableNdx;wi<MAX_TABLE_SIZE;wi++)
         {
            *(lpwPrefix+wi)  = 0xFFFF;
            *(lpbySuffix+wi) = 0x00;
         }
         byCurrentBits = byLZW_MinCodeLen + (BYTE)0x01;
         wNowTableNdx  = wLZW_CLEAR + 2;
         wTopTableNdx  = 1 << byCurrentBits;
         wOldCode      = 0xFFFF;

         /* Get Next Code */
         while( byLeftBits<byCurrentBits )
         {
            /* Get Next Char */
            if ( ++byBlockNdx == byBlockSize )
            {
               /* Get Next Block */
               byBlockSize = *lpGIFDVar->lpBgnBuff++;
			   i_Imagesize += byBlockSize +1;
               byBlockNdx  = 0x00;
               if ( (lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
                    !lpGIFDVar->bEOF )
               {
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpGIFDVar->wMemLen,
                                         &lpGIFDVar->dwDataLen,
                                         lpGIFDVar->lpDataBuff,
                                         &lpGIFDVar->bEOF);
                  lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
                  lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff +
                                         lpGIFDVar->wMemLen;
               }
            }
            byCode      = *lpGIFDVar->lpBgnBuff++;
            dwData     |= ((DWORD)byCode << byLeftBits);
            byLeftBits += 0x08;
            if ( (lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
                 !lpGIFDVar->bEOF )
            {
               iReturn = m_utility.iReadSrcData(m_SrcFile,&lpGIFDVar->wMemLen,
                                      &lpGIFDVar->dwDataLen,
                                      lpGIFDVar->lpDataBuff,&lpGIFDVar->bEOF);
               lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
               lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
            }
         }
         wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
         dwData    >>= byCurrentBits;
         byLeftBits -= byCurrentBits;

         if ( wCode!=wLZW_EOI )
         {
            /* lpbyStack[0] Is Not Used */
            lpbyStack  ++;

            /* Put Data Into Stack */
            while( (*(lpwPrefix+wCode)) != 0xFFFF )
            {
               *lpbyStack++ = *(lpbySuffix+wCode);
               wCode        = *(lpwPrefix+wCode);
            }
            *lpbyStack  = *(lpbySuffix+wCode);
            byFirstChar = *lpbyStack;

            /* Write Data */
            while( lpbyStack>lpbyStackBgn )
            {
               byTempChar |= (*lpbyStack-- << (8-wBitCnt));
               if ( wBitCnt==8 )
               {
                  *lpImage++ = byTempChar;
                  byTempChar = 0x00;
                  wBitCnt    = lpGIFDVar->wBits;
               }
               else
               {
                  wBitCnt   += lpGIFDVar->wBits;
               }

               wWidthCnt  ++;
               if ( wWidthCnt==lpGIFDVar->wWidth )
               {
                  if ( wBitCnt!=lpGIFDVar->wBits )
                  {
                     *lpImage   = byTempChar;
                     byTempChar = 0x00;
                     wBitCnt    = lpGIFDVar->wBits;
                  }
                  dwDataNdx = (DWORD)(lpGIFDVar->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
				  memcpy(pOutGIF+dwDataNdx,lpImageBgn,wWidthBytes);
				  wRowCnt++;
                  lpImage   = lpImageBgn;
                  if ( lpGIFDVar->bInterlace )
                  {
                     wRowNum += wIncTable[byPass];
                     if ( wRowNum>=lpGIFDVar->wDepth )
                     {
                        byPass ++;
                        wRowNum = wBgnTable[byPass];
                     }
                  }
                  else
                  {
                     wRowNum ++;
                  }
                  wWidthCnt = 0;
               }
            }
         }
      }
      else
      {
         /* lpbyStack[0] Is Not Used */
         lpbyStack  ++;

         if ( wCode<wNowTableNdx )
         {
            /* The String Is In The Table */
            wTempNdx   = wCode;
         }
         else
         {
            wTempNdx     = wOldCode;
            *lpbyStack++ = byFirstChar;
         }

         /* Put Data Into Stack */
         while( (*(lpwPrefix+wTempNdx)) != 0xFFFF )
         {
            *lpbyStack++ = *(lpbySuffix+wTempNdx);
            wTempNdx     = *(lpwPrefix+wTempNdx);
         }
         *lpbyStack  = *(lpbySuffix+wTempNdx);
         byFirstChar = *lpbyStack;

         /* Add String To The Table */
         *(lpwPrefix+wNowTableNdx)  = wOldCode;
         *(lpbySuffix+wNowTableNdx) = byFirstChar;
         if ( ++wNowTableNdx==wTopTableNdx && byCurrentBits<12 )
         {
            byCurrentBits ++;
            wTopTableNdx   = 1 << byCurrentBits;
         }

         /* Write Data */
         while( lpbyStack>lpbyStackBgn )
         {
            byTempChar |= (*lpbyStack-- << (8-wBitCnt));
            if ( wBitCnt==8 )
            {
               *lpImage++ = byTempChar;
               byTempChar = 0x00;
               wBitCnt    = lpGIFDVar->wBits;
            }
            else
            {
               wBitCnt   += lpGIFDVar->wBits;
            }

            wWidthCnt  ++;
            if ( wWidthCnt==lpGIFDVar->wWidth )
            {
               if ( wBitCnt!=lpGIFDVar->wBits )
               {
                  *lpImage   = byTempChar;
                  byTempChar = 0x00;
                  wBitCnt    = lpGIFDVar->wBits;
               }
               dwDataNdx = (DWORD)(lpGIFDVar->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
               memcpy(pOutGIF+dwDataNdx,lpImageBgn,wWidthBytes);
			   wRowCnt++;
               lpImage   = lpImageBgn;
               if ( lpGIFDVar->bInterlace )
               {
                  wRowNum += wIncTable[byPass];
                  if ( wRowNum>=lpGIFDVar->wDepth )
                  {
                     byPass ++;
                     wRowNum = wBgnTable[byPass];
                  }
               }
               else
               {
                  wRowNum ++;
               }
               wWidthCnt = 0;
            }
         }
      }
      wOldCode = wCode;

      /* Get Next Code */
      while( byLeftBits<byCurrentBits )
      {
         /* Get Next Char */
         if ( ++byBlockNdx == byBlockSize )
         {
            /* Get Next Block */
            byBlockSize = *lpGIFDVar->lpBgnBuff++;
			i_Imagesize += byBlockSize + 1;
            byBlockNdx  = 0x00;
            if ( (lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
                 !lpGIFDVar->bEOF )
            {
               iReturn = m_utility.iReadSrcData(m_SrcFile,&lpGIFDVar->wMemLen,
                                      &lpGIFDVar->dwDataLen,
                                      lpGIFDVar->lpDataBuff,&lpGIFDVar->bEOF);
               lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
               lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
            }
         }
         byCode      = *lpGIFDVar->lpBgnBuff++;
         dwData     |= ((DWORD)byCode << byLeftBits);
         byLeftBits += 0x08;
         if ( (lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
              !lpGIFDVar->bEOF )
         {
            iReturn = m_utility.iReadSrcData(m_SrcFile,&lpGIFDVar->wMemLen,
                                   &lpGIFDVar->dwDataLen,
                                   lpGIFDVar->lpDataBuff,&lpGIFDVar->bEOF);
            lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
            lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
         }
      }
      wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
      dwData    >>= byCurrentBits;
      byLeftBits -= byCurrentBits;
   }
   GlobalUnlock(hPrefix);
   GlobalUnlock(hSuffix);
   GlobalUnlock(hStack);
   GlobalUnlock(hImage);
   GlobalFree(hPrefix);
   GlobalFree(hSuffix);
   GlobalFree(hStack);
   GlobalFree(hImage);
   return 0;
}
// 功能描述	: GIF的LZW压缩编码
// 参数		: CFile *m_DstFile,原文件 LPBYTE pInbmp,图像句柄 LPGIFC_VAR lpGIFCVar, gif结构WORD wWidthBytes行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CGIF_LZW::iEncodeGIF_LZW(LPBYTE &pInBmp, CFile *m_DstFile, LPGIFC_VAR lpGIFCVar, WORD wWidthBytes)
{
   HANDLE hTableNdx, hPrefix, hSuffix, hImage;
   LPWORD lpwTableNdx, lpwPrefix;
   LPBYTE lpbySuffix;
   LPSTR  lpImage,  lpImageBgn;
   DWORD  dwDataNdx;
   WORD wLZW_CLEAR, wLZW_EOI, wPreTableNdx, wNowTableNdx, wTopTableNdx, wHashGap, wHashNdx, wPrefix;
   WORD   wShiftBits, wRowNum, wWidthCnt, wi,  wj;
   WORD   wIncTable[5]  = { 8,8,4,2,0 }; /* Interlace Pass Increments */
   WORD   wBgnTable[5]  = { 0,4,2,1,0 }; /* Interlace Pass Beginnings */
   BOOL   bStart;
   BYTE   bySuffix, byLZW_MinCodeLen;
   BYTE   bySubBlock[256];
   BYTE   byCurrentBits, byMask, byChar, byTemp, byPass;

   hTableNdx        = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE<<1));
   hPrefix          = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE<<1));
   hSuffix          = GlobalAlloc(GHND,(DWORD)MAX_HASH_SIZE);
   hImage           = GlobalAlloc(GHND,(DWORD)wWidthBytes);
   lpwTableNdx      = (LPWORD)GlobalLock(hTableNdx);
   lpwPrefix        = (LPWORD)GlobalLock(hPrefix);
   lpbySuffix       = (LPBYTE)GlobalLock(hSuffix);
   lpImage          = (char *)GlobalLock(hImage);
   lpImageBgn       = lpImage;
	if((lpwTableNdx==NULL) || (lpwPrefix==NULL) || (lpbySuffix==NULL) || (lpImage == NULL))
	{
		AfxMessageBox("LZW编码申请内存出错!");
		iReturn = ENLZW_ALLOCMEMERR;
		return iReturn;
	}
   byLZW_MinCodeLen = (BYTE)( (lpGIFCVar->wBits>1) ? lpGIFCVar->wBits : 0x02 );

   /* Write GIF LZW Minimum Code Size */
   m_DstFile->Write((LPSTR)&byLZW_MinCodeLen,1);

//   dwDataBgn        = (DWORD)_llseek(iFile,0L,SEEK_CUR);
   wRowNum          = 0;
   bStart           = TRUE;
   byPass           = 0x00;

   /* Initialize The Table */
   byCurrentBits    = byLZW_MinCodeLen + (BYTE)0x01;
   wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
   wLZW_EOI         = wLZW_CLEAR + 1;
   wNowTableNdx     = wLZW_CLEAR + 2;
   wTopTableNdx     = 1 << byCurrentBits;
   for(wi=0;wi<MAX_HASH_SIZE;wi++)
   {
      *(lpwTableNdx+wi) = 0xFFFF;
   }

   /* Write This Code */
   iGIF_LZW_WriteCode(m_DstFile,wLZW_CLEAR,(LPSTR)bySubBlock,
                      &byCurrentBits,lpGIFCVar);

   for(wi=0;wi<lpGIFCVar->wDepth;wi++)
   {
      dwDataNdx = (DWORD)(lpGIFCVar->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
      memcpy(lpImageBgn,pInBmp + dwDataNdx, wWidthBytes);
      lpImage    = lpImageBgn;
      wWidthCnt  = 0;
      wShiftBits = 8 - lpGIFCVar->wBits;
      byMask     = (BYTE)( (lpGIFCVar->wBits==1) ? 0x80 : 0xF0 );

      if ( bStart )
      {
         if ( lpGIFCVar->wBits==8 )
         {
            byTemp      = *lpImage++;
         }
         else
         {
            wShiftBits  = 8 - lpGIFCVar->wBits;
            byMask      = (BYTE)( (lpGIFCVar->wBits==1) ? 0x80 : 0xF0 );
            byTemp      = (BYTE)( (*lpImage & byMask) >> wShiftBits );
            byMask    >>= lpGIFCVar->wBits;
            wShiftBits -= lpGIFCVar->wBits;
         }
         wPrefix    = (WORD)byTemp;
         bStart     = FALSE;
         wWidthCnt ++;
      }
      while( wWidthCnt<lpGIFCVar->wWidth )
      {
         if ( lpGIFCVar->wBits==8 )
         {
            byTemp = *lpImage++;
         }
         else
         {
            byChar = *lpImage;
            byTemp = (BYTE)( (byChar & byMask) >> wShiftBits );
            if ( wShiftBits )
            {
               byMask    >>= lpGIFCVar->wBits;
               wShiftBits -= lpGIFCVar->wBits;
            }
            else
            {
               wShiftBits  = 8 - lpGIFCVar->wBits;
               byMask      = (BYTE)( (lpGIFCVar->wBits==1) ? 0x80 : 0xF0 );
               lpImage    ++;
            }
         }
         bySuffix   = byTemp;
         wWidthCnt ++;

         /* See If The String Is In The Table */
         wHashNdx = wPrefix ^ ((WORD)bySuffix << 4);
         wHashGap = (wHashNdx ? (MAX_HASH_SIZE - wHashNdx) : 1);
         while( TRUE )
         {
            if ( *(lpwTableNdx+wHashNdx) == 0xFFFF )
            {
               /* The String Is Not In The Table */
               break;
            }
            if ( (*(lpwPrefix+wHashNdx)  == wPrefix) &&
                 (*(lpbySuffix+wHashNdx) == bySuffix) )
            {
               /* String Found */
               break;
            }

            /* Secondary Hash */
            if ( wHashNdx<wHashGap )
            {
               wHashNdx += MAX_HASH_SIZE;
            }
            wHashNdx -= wHashGap;
         }

         if ( *(lpwTableNdx+wHashNdx) != 0xFFFF )
         {
            /* The String Is In The Table */
            wPrefix = *(lpwTableNdx+wHashNdx);
         }
         else
         {
            /* The String Is Not In The Table */

            /* Write This Code */
            iGIF_LZW_WriteCode(m_DstFile,wPrefix,(LPSTR)bySubBlock,
                               &byCurrentBits,lpGIFCVar);

            wPreTableNdx = wNowTableNdx;
            if ( wNowTableNdx<MAX_TABLE_SIZE )
            {
               *(lpwTableNdx+wHashNdx) = wNowTableNdx++;
               *(lpwPrefix+wHashNdx)   = wPrefix;
               *(lpbySuffix+wHashNdx)  = bySuffix;
            }

            if ( wPreTableNdx==wTopTableNdx )
            {
               if ( byCurrentBits<12 )
               {
                  byCurrentBits ++;
                  wTopTableNdx <<= 1;
               }
               else
               {
                  /* Write This Code */
                  iGIF_LZW_WriteCode(m_DstFile,wLZW_CLEAR,(LPSTR)bySubBlock,
                                     &byCurrentBits,lpGIFCVar);

                  /* Initialize The Table */
                  byCurrentBits    = byLZW_MinCodeLen + (BYTE)0x01;
                  wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
                  wLZW_EOI         = wLZW_CLEAR + 1;
                  wNowTableNdx     = wLZW_CLEAR + 2;
                  wTopTableNdx     = 1 << byCurrentBits;
                  for(wj=0;wj<MAX_HASH_SIZE;wj++)
                  {
                     *(lpwTableNdx+wj) = 0xFFFF;
                  }
               }
            }
            wPrefix = (WORD)bySuffix;
         }
      }

      if ( m_bInterlace )
      {
         wRowNum += wIncTable[byPass];
         if ( wRowNum>=lpGIFCVar->wDepth )
         {
            byPass ++;
            wRowNum = wBgnTable[byPass];
         }
      }
      else
      {
         wRowNum ++;
      }
   }

   /* Write This Code */
   iGIF_LZW_WriteCode(m_DstFile,wPrefix,(LPSTR)bySubBlock,
                      &byCurrentBits,lpGIFCVar);

   /* Write This Code */
   iGIF_LZW_WriteCode(m_DstFile,wLZW_EOI,(LPSTR)bySubBlock,
                      &byCurrentBits,lpGIFCVar);

   if ( lpGIFCVar->byLeftBits )
   {
      /* Put This Char */
      bySubBlock[lpGIFCVar->wBlockNdx++] = (BYTE)lpGIFCVar->dwTempCode;
      if ( lpGIFCVar->wBlockNdx>MAX_SUBBLOCK_SIZE )
      {
         /* Put This Block */
         if ( (lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE )
         {
            m_DstFile->Write(lpGIFCVar->lpDataBuff,
                              lpGIFCVar->wByteCnt);
            lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
            lpGIFCVar->wByteCnt  = 0;
         }
         bySubBlock[0]           = (BYTE)(lpGIFCVar->wBlockNdx - 1);
         memcpy(lpGIFCVar->lpEndBuff,(LPSTR)bySubBlock,lpGIFCVar->wBlockNdx);
         lpGIFCVar->lpEndBuff   += lpGIFCVar->wBlockNdx;
         lpGIFCVar->wByteCnt    += lpGIFCVar->wBlockNdx;
         lpGIFCVar->wBlockNdx    = 1;
      }
      lpGIFCVar->dwTempCode = 0UL;
      lpGIFCVar->byLeftBits = 0x00;
   }

   if ( lpGIFCVar->wBlockNdx>1 )
   {
      /* Put This Block */
      if ( (lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE )
      {
         m_DstFile->Write(lpGIFCVar->lpDataBuff,
                           lpGIFCVar->wByteCnt);
         lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
         lpGIFCVar->wByteCnt  = 0;
      }
      bySubBlock[0]           = (BYTE)(lpGIFCVar->wBlockNdx - 1);
      memcpy(lpGIFCVar->lpEndBuff,(LPSTR)bySubBlock,lpGIFCVar->wBlockNdx);
      lpGIFCVar->lpEndBuff   += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wByteCnt    += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wBlockNdx    = 1;
   }

   GlobalUnlock(hTableNdx);
   GlobalUnlock(hPrefix);
   GlobalUnlock(hSuffix);
   GlobalUnlock(hImage);
   GlobalFree(hTableNdx);
   GlobalFree(hPrefix);
   GlobalFree(hSuffix);
   GlobalFree(hImage);
   return 0;
}

int CGIF_LZW::iGIF_LZW_WriteCode(CFile *m_DstFile, WORD wCode, LPSTR lpSubBlock, LPBYTE lpbyCurrentBits, LPGIFC_VAR lpGIFCVar)
{
   /* Put This Code */
   lpGIFCVar->dwTempCode |= ((DWORD)wCode << lpGIFCVar->byLeftBits);
   lpGIFCVar->byLeftBits += (*lpbyCurrentBits);

   while( lpGIFCVar->byLeftBits>=0x08 )
   {
      /* Put This Char */
      lpSubBlock[lpGIFCVar->wBlockNdx++] = (BYTE)lpGIFCVar->dwTempCode;
      if ( lpGIFCVar->wBlockNdx>MAX_SUBBLOCK_SIZE )
      {
         /* Put This Block */
         if ( (lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE )
         {
            m_DstFile->Write(lpGIFCVar->lpDataBuff,
                              lpGIFCVar->wByteCnt);
            lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
            lpGIFCVar->wByteCnt  = 0;
         }
         lpSubBlock[0]           = (BYTE)(lpGIFCVar->wBlockNdx - 1);
         memcpy(lpGIFCVar->lpEndBuff,lpSubBlock,lpGIFCVar->wBlockNdx);
         lpGIFCVar->lpEndBuff   += lpGIFCVar->wBlockNdx;
         lpGIFCVar->wByteCnt    += lpGIFCVar->wBlockNdx;
         lpGIFCVar->wBlockNdx    = 1;
      }
      lpGIFCVar->dwTempCode >>= 8;
      lpGIFCVar->byLeftBits  -= 0x08;
   }
   return 0;
}
