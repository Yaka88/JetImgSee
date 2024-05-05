// TGA_RLE.cpp: implementation of the CTGA_RLE class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: TGARLE.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TGA的RLE压缩的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "TGA_RLE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
CTGA_RLE::CTGA_RLE()
{

}

CTGA_RLE::~CTGA_RLE()
{

}
extern int iReturn ;
// 功能描述	: TAG的RLE压缩解码
// 参数		: CFile* m_srcFile,原文件LPTGAD_VAR lpTGADVar,TGA结构LPSTR lpTemp
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CTGA_RLE::iDecodeLineTGA_RLE(CFile *m_SrcFile, LPTGAD_VAR lpTGADVar, LPSTR lpImage)
{
   LPSTR lpImageEnd;
   WORD  wBack;
   WORD  wValue;
   BYTE  byChar;
   BYTE  byR;
   BYTE  byG;
   BYTE  byB;
   int   iCount;
	iReturn = 0;
   lpImageEnd = lpImage + lpTGADVar->wWidth * ((lpTGADVar->wNewBits+7) >> 3);
   while( lpImage<lpImageEnd )
   {
      byChar = *lpTGADVar->lpBgnBuff++;
      if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
           !lpTGADVar->bEOF )
      {
         iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                &lpTGADVar->dwDataLen,
                                lpTGADVar->lpDataBuff,&lpTGADVar->bEOF);
         lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
         lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
      }

      iCount = (int)(byChar & 0x7F) + 1;
      if ( byChar & 0x80 )
      {
         switch(lpTGADVar->wSrcBits)
         {
            case  8:
            {
               byChar   = *lpTGADVar->lpBgnBuff++;
               if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                    !lpTGADVar->bEOF )
               {
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }
               memset(lpImage,(int)byChar,iCount);
               lpImage += iCount;
               break;
            }
            case 15:
            case 16:
            {
               if ( (lpTGADVar->lpBgnBuff + 2) > lpTGADVar->lpEndBuff )
               {
                  wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
                  m_SrcFile->Seek(-(LONG)wBack,CFile::current);
                  lpTGADVar->dwDataLen += (DWORD)wBack;
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }

               wValue   = *(LPWORD)lpTGADVar->lpBgnBuff;
               byB      = (BYTE)( ((wValue    ) & 0x1F)<<3 );
               byG      = (BYTE)( ((wValue>> 5) & 0x1F)<<3 );
               byR      = (BYTE)( ((wValue>>10) & 0x1F)<<3 );
               while( iCount-- )
               {
                  *lpImage++ = byB;
                  *lpImage++ = byG;
                  *lpImage++ = byR;
               }

               lpTGADVar->lpBgnBuff += 2;
               if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                    !lpTGADVar->bEOF )
               {
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }
               break;
            }
            case 24:
            {
               if ( (lpTGADVar->lpBgnBuff + 3) > lpTGADVar->lpEndBuff )
               {
                  wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
                  m_SrcFile->Seek(-(LONG)wBack,CFile::current);
                  lpTGADVar->dwDataLen += (DWORD)wBack;
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }

               byB      = *lpTGADVar->lpBgnBuff++;
               byG      = *lpTGADVar->lpBgnBuff++;
               byR      = *lpTGADVar->lpBgnBuff++;
               while( iCount-- )
               {
                  *lpImage++ = byB;
                  *lpImage++ = byG;
                  *lpImage++ = byR;
               }
               if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                    !lpTGADVar->bEOF )
               {
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }
               break;
            }
            case 32:
            {
               if ( (lpTGADVar->lpBgnBuff + 4) > lpTGADVar->lpEndBuff )
               {
                  wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
                  m_SrcFile->Seek(-(LONG)wBack,CFile::current);
                  lpTGADVar->dwDataLen += (DWORD)wBack;
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }

               byB      = *lpTGADVar->lpBgnBuff++;
               byG      = *lpTGADVar->lpBgnBuff++;
               byR      = *lpTGADVar->lpBgnBuff++;
               while( iCount-- )
               {
                  *lpImage++ = byB;
                  *lpImage++ = byG;
                  *lpImage++ = byR;
               }
               lpTGADVar->lpBgnBuff ++;
               if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                    !lpTGADVar->bEOF )
               {
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }
               break;
            }
         }
      }
      else
      {
         switch(lpTGADVar->wSrcBits)
         {
            case  8:
            {
               if ( (lpTGADVar->lpBgnBuff + iCount) > lpTGADVar->lpEndBuff )
               {
                  wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
                  m_SrcFile->Seek(-(LONG)wBack,CFile::current);
                  lpTGADVar->dwDataLen += (DWORD)wBack;
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }

               memcpy(lpImage,lpTGADVar->lpBgnBuff,iCount);
               lpImage              += iCount;
               lpTGADVar->lpBgnBuff += iCount;
               if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                    !lpTGADVar->bEOF )
               {
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }
               break;
            }
            case 15:
            case 16:
            {
               while( iCount-- )
               {
                  if ( (lpTGADVar->lpBgnBuff + 2) > lpTGADVar->lpEndBuff )
                  {
                     wBack   = (WORD)(lpTGADVar->lpEndBuff -
                                      lpTGADVar->lpBgnBuff);
                     m_SrcFile->Seek(-(LONG)wBack,CFile::current);
                     lpTGADVar->dwDataLen += (DWORD)wBack;
                     iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                            &lpTGADVar->dwDataLen,
                                            lpTGADVar->lpDataBuff,
                                            &lpTGADVar->bEOF);
                     lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                     lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                            lpTGADVar->wMemLen;
                  }

                  wValue     = *(LPWORD)lpTGADVar->lpBgnBuff;
                  *lpImage++ = (BYTE)( ((wValue    ) & 0x1F)<<3 );
                  *lpImage++ = (BYTE)( ((wValue>> 5) & 0x1F)<<3 );
                  *lpImage++ = (BYTE)( ((wValue>>10) & 0x1F)<<3 );
                  lpTGADVar->lpBgnBuff += 2;
                  if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                       !lpTGADVar->bEOF )
                  {
                     iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                            &lpTGADVar->dwDataLen,
                                            lpTGADVar->lpDataBuff,
                                            &lpTGADVar->bEOF);
                     lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                     lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                            lpTGADVar->wMemLen;
                  }
               }
               break;
            }
            case 24:
            {
               iCount *= 3;
               if ( (lpTGADVar->lpBgnBuff + iCount) > lpTGADVar->lpEndBuff )
               {
                  wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
                  m_SrcFile->Seek(-(LONG)wBack,CFile::current);
                  lpTGADVar->dwDataLen += (DWORD)wBack;
                  iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }

               memcpy(lpImage,lpTGADVar->lpBgnBuff,iCount);
               lpImage              += iCount;
               lpTGADVar->lpBgnBuff += iCount;
               if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                    !lpTGADVar->bEOF )
               {
                  iReturn =m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                         &lpTGADVar->dwDataLen,
                                         lpTGADVar->lpDataBuff,
                                         &lpTGADVar->bEOF);
                  lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                  lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                         lpTGADVar->wMemLen;
               }
               break;
            }
            case 32:
            {
               while( iCount-- )
               {
                  if ( (lpTGADVar->lpBgnBuff + 4) > lpTGADVar->lpEndBuff )
                  {
                     wBack   = (WORD)(lpTGADVar->lpEndBuff -
                                      lpTGADVar->lpBgnBuff);
                     m_SrcFile->Seek(-(LONG)wBack,CFile::current);
                     lpTGADVar->dwDataLen += (DWORD)wBack;
                     iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                            &lpTGADVar->dwDataLen,
                                            lpTGADVar->lpDataBuff,
                                            &lpTGADVar->bEOF);
                     lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                     lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                            lpTGADVar->wMemLen;
                  }

                  *lpImage++ = *lpTGADVar->lpBgnBuff++;
                  *lpImage++ = *lpTGADVar->lpBgnBuff++;
                  *lpImage++ = *lpTGADVar->lpBgnBuff++;
                  lpTGADVar->lpBgnBuff ++;
                  if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                       !lpTGADVar->bEOF )
                  {
                     iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                            &lpTGADVar->dwDataLen,
                                            lpTGADVar->lpDataBuff,
                                            &lpTGADVar->bEOF);
                     lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
                     lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff +
                                            lpTGADVar->wMemLen;
                  }
               }
               break;
            }
         }
      }
   }
   return SUCCESS;
}
// 功能描述	: PCX的RLE压缩编码
// 参数		: CFile* m_DesFile,原文件LPTGAC_VAR lpTGACVar,PCX结构LPSTR lpTemp
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CTGA_RLE::iEncodeLineTGA_RLE(CFile *m_DesFile, LPTGAC_VAR lpTGACVar, LPSTR lpImage, LPSTR lpTemp, WORD wRunCnt)
{
	LPSTR lpTempEnd;
   int   iCount;
   int   iTempCount;

   iCount    = 0;
   lpTempEnd = lpTemp + wRunCnt;
   if ( lpTGACVar->wBits==8 )
   {
      while( lpTemp<lpTempEnd )
      {
         iTempCount = (int)(*lpTemp++ & 0xFF);
         if ( (iTempCount < 3) && ((iCount+iTempCount) <= 128) )
         {
            iCount += iTempCount;
         }
         else
         {
            if ( iCount )
            {
               *lpTGACVar->lpEndBuff++ = (BYTE)(iCount - 1);
               lpTGACVar->wByteCnt    ++;
               if ( (lpTGACVar->wByteCnt + (WORD)iCount) >= MAX_BUFF_SIZE )
               {
                  m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
                  lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
                  lpTGACVar->wByteCnt  = 0;
               }
               memcpy(lpTGACVar->lpEndBuff,lpImage,iCount);
               lpTGACVar->lpEndBuff   += iCount;
               lpTGACVar->wByteCnt    += (WORD)iCount;
               lpImage                += iCount;
               iCount                  = 0;
            }

            *lpTGACVar->lpEndBuff++ = (BYTE)((iTempCount - 1) | 0x80);
            lpTGACVar->wByteCnt    ++;
            if ( lpTGACVar->wByteCnt==MAX_BUFF_SIZE )
            {
               m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
               lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
               lpTGACVar->wByteCnt  = 0;
            }
            *lpTGACVar->lpEndBuff++ = *lpImage;
            lpTGACVar->wByteCnt    ++;
            if ( lpTGACVar->wByteCnt==MAX_BUFF_SIZE )
            {
               m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
               lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
               lpTGACVar->wByteCnt  = 0;
            }
            lpImage += iTempCount;
         }
      }
      if ( iCount )
      {
         *lpTGACVar->lpEndBuff++ = (BYTE)(iCount - 1);
         lpTGACVar->wByteCnt    ++;
         if ( (lpTGACVar->wByteCnt + (WORD)iCount) >= MAX_BUFF_SIZE )
         {
            m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
            lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
            lpTGACVar->wByteCnt  = 0;
         }
         memcpy(lpTGACVar->lpEndBuff,lpImage,iCount);
         lpTGACVar->lpEndBuff   += iCount;
         lpTGACVar->wByteCnt    += (WORD)iCount;
      }
   }
   else
   {
      while( lpTemp<lpTempEnd )
      {
         iTempCount = (int)(*lpTemp++ & 0xFF);
         if ( (iTempCount == 1) && ((iCount+iTempCount) <= 128) )
         {
            iCount += iTempCount;
         }
         else
         {
            if ( iCount )
            {
               *lpTGACVar->lpEndBuff++ = (BYTE)(iCount - 1);
               lpTGACVar->wByteCnt    ++;
               iCount  *= 3;
               if ( (lpTGACVar->wByteCnt + (WORD)iCount) >= MAX_BUFF_SIZE )
               {
                  m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
                  lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
                  lpTGACVar->wByteCnt  = 0;
               }
               memcpy(lpTGACVar->lpEndBuff,lpImage,iCount);
               lpTGACVar->lpEndBuff   += iCount;
               lpTGACVar->wByteCnt    += (WORD)iCount;
               lpImage                += iCount;
               iCount                  = 0;
            }

            iTempCount --;
            *lpTGACVar->lpEndBuff++ = (BYTE)(iTempCount | 0x80);
            lpTGACVar->wByteCnt    ++;
            if ( (lpTGACVar->wByteCnt + 3) >= MAX_BUFF_SIZE )
            {
               m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
               lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
               lpTGACVar->wByteCnt  = 0;
            }
            *lpTGACVar->lpEndBuff++ = *lpImage++;
            *lpTGACVar->lpEndBuff++ = *lpImage++;
            *lpTGACVar->lpEndBuff++ = *lpImage++;
            lpTGACVar->wByteCnt    += 3;
            lpImage                += (iTempCount * 3);
         }
      }
      if ( iCount )
      {
         *lpTGACVar->lpEndBuff++ = (BYTE)(iCount - 1);
         lpTGACVar->wByteCnt    ++;
         iCount *= 3;
         if ( (lpTGACVar->wByteCnt + (WORD)iCount) >= MAX_BUFF_SIZE )
         {
            m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
            lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
            lpTGACVar->wByteCnt  = 0;
         }
         memcpy(lpTGACVar->lpEndBuff,lpImage,iCount);
         lpTGACVar->lpEndBuff   += iCount;
         lpTGACVar->wByteCnt    += (WORD)iCount;
      }
   }
   return 0;
}
