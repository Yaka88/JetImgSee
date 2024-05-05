// PCXRLE.cpp: implementation of the PCXRLE class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// 文件名	: PCXRLE.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理PCX的RLE压缩的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "PCXRLE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCXRLE::CPCXRLE()
{

}

CPCXRLE::~CPCXRLE()
{

}
extern int iReturn ;
// 功能描述	: PCX的RLE压缩编码
// 参数		: CFile* m_TEMPFile,目标文件LPPCXC_VAR lpPCXCVar,PCX结构LPSTR lpTemp
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CPCXRLE::iEncodeLine(CFile* m_TempFile,LPPCXC_VAR lpPCXCVar,LPSTR lpTemp)
{
   LPSTR lpTempEnd;
   WORD  wi;
   int   iCount;

   for(wi=0;wi<lpPCXCVar->wPlanes;wi++)
   {
      lpTempEnd = lpTemp + lpPCXCVar->wLineBytes;
      while( lpTemp<lpTempEnd )
      {
         iCount = 1;
         while( (*lpTemp == *(lpTemp+1)) &&
                (iCount < 63) && ((lpTemp+1) < lpTempEnd) )
         {
            iCount ++;
            lpTemp ++;
         }

         if ( iCount>1 )
         {
            *lpPCXCVar->lpEndBuff++ = (BYTE)(iCount | 0xC0);
            lpPCXCVar->wByteCnt    ++;
            if ( lpPCXCVar->wByteCnt==MAX_BUFF_SIZE )
            {
               m_TempFile->Write(lpPCXCVar->lpDataBuff,lpPCXCVar->wByteCnt);
               lpPCXCVar->wByteCnt  = 0;
               lpPCXCVar->lpEndBuff = lpPCXCVar->lpDataBuff;
            }
         }
         else
         {
            if ( (*lpTemp & 0xC0) == 0xC0 )
            {
               *lpPCXCVar->lpEndBuff++ = (char)0xC1;
               lpPCXCVar->wByteCnt    ++;
               if ( lpPCXCVar->wByteCnt==MAX_BUFF_SIZE )
               {
                  m_TempFile->Write(lpPCXCVar->lpDataBuff,lpPCXCVar->wByteCnt);
                  lpPCXCVar->wByteCnt  = 0;
                  lpPCXCVar->lpEndBuff = lpPCXCVar->lpDataBuff;
               }
            }
         }
         *lpPCXCVar->lpEndBuff++ = *lpTemp++;
         lpPCXCVar->wByteCnt    ++;
         if ( lpPCXCVar->wByteCnt==MAX_BUFF_SIZE )
         {
            m_TempFile->Write(lpPCXCVar->lpDataBuff,lpPCXCVar->wByteCnt);
            lpPCXCVar->wByteCnt  = 0;
            lpPCXCVar->lpEndBuff = lpPCXCVar->lpDataBuff;
         }
      }
   }
   return 0;
}
// 功能描述	: PCX的RLE压缩解码
// 参数		: CFile* m_srcFile,原文件LPPCXD_VAR lpPCXDVar,PCX结构LPSTR lpTemp
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CPCXRLE::iDecodeLine(CFile* m_srcFile,LPPCXD_VAR lpPCXDVar,LPSTR lpTemp)
{
   LPSTR lpTempEnd;
   WORD  wi;
   BYTE  byChar;
   int   iCount;

   for(wi=0;wi<lpPCXDVar->wPlanes;wi++)
   {
      lpTempEnd = lpTemp + lpPCXDVar->wLineBytes;
      while( lpTemp<lpTempEnd )
      {
         byChar = *lpPCXDVar->lpBgnBuff++;
         if ( (lpPCXDVar->lpBgnBuff == lpPCXDVar->lpEndBuff) &&
              !lpPCXDVar->bEOF )
         {
            iReturn = m_utility.iReadSrcData( m_srcFile,&lpPCXDVar->wMemLen,
                                   &lpPCXDVar->dwDataLen,
                                   lpPCXDVar->lpDataBuff,&lpPCXDVar->bEOF);
            lpPCXDVar->lpBgnBuff = lpPCXDVar->lpDataBuff;
            lpPCXDVar->lpEndBuff = lpPCXDVar->lpBgnBuff + lpPCXDVar->wMemLen;
         }

         if ( (byChar & 0xC0) == 0xC0 )
         {
            iCount = (int)(byChar & 0x3F);
            byChar = *lpPCXDVar->lpBgnBuff++;
            if ( (lpPCXDVar->lpBgnBuff == lpPCXDVar->lpEndBuff) &&
                 !lpPCXDVar->bEOF )
            {
               iReturn = m_utility.iReadSrcData(m_srcFile,&lpPCXDVar->wMemLen,
                                      &lpPCXDVar->dwDataLen,
                                      lpPCXDVar->lpDataBuff,&lpPCXDVar->bEOF);
               lpPCXDVar->lpBgnBuff = lpPCXDVar->lpDataBuff;
               lpPCXDVar->lpEndBuff = lpPCXDVar->lpBgnBuff + lpPCXDVar->wMemLen;
            }
            while( iCount-- )
            {
               *lpTemp++ = byChar;
            }
         }
         else
         {
            *lpTemp++ = byChar;
         }
      }
   }
   return 0;
}


