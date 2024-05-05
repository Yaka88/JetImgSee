// TGARaw.cpp: implementation of the CTGARaw class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: TGARaw.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TGA的无压缩的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "TGARaw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
CTGARaw::CTGARaw()
{

}

CTGARaw::~CTGARaw()
{

}
extern int iReturn ;
// 功能描述	: TGA的非压缩解码
// 参数		: CFile* m_srcFile,原文件LPTGAD_VAR lpTGADVar,PCX结构LPSTR lpTemp
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CTGARaw::iDecodeLineTGA_RawData(CFile *m_SrcFile, LPTGAD_VAR lpTGADVar, LPSTR lpImage)
{
   WORD wBack;
   WORD wValue;
   WORD wi;

   switch(lpTGADVar->wSrcBits)
   {
      case  8:
      case 24:
      {
         if ( (lpTGADVar->lpBgnBuff + lpTGADVar->wLineBytes) >
              lpTGADVar->lpEndBuff )
         {
            wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
    		m_SrcFile->Seek(-(LONG)wBack,CFile::current);
            lpTGADVar->dwDataLen += (DWORD)wBack;
            if (iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                   &lpTGADVar->dwDataLen,
                                   lpTGADVar->lpDataBuff,&lpTGADVar->bEOF))
								   return iReturn;
            lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
            lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
         }

         memcpy(lpImage,lpTGADVar->lpBgnBuff,lpTGADVar->wLineBytes);
         lpTGADVar->lpBgnBuff += lpTGADVar->wLineBytes;
         if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
              !lpTGADVar->bEOF )
         {
            if (iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                   &lpTGADVar->dwDataLen,
                                   lpTGADVar->lpDataBuff,&lpTGADVar->bEOF))
								   return iReturn;
            lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
            lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
         }
         break;
      }
      case 15:
      case 16:
      {
         for(wi=0;wi<lpTGADVar->wWidth;wi++)
         {
            if ( (lpTGADVar->lpBgnBuff + 2) > lpTGADVar->lpEndBuff )
            {
               wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
	    	   m_SrcFile->Seek(-(LONG)wBack,CFile::current);
               lpTGADVar->dwDataLen += (DWORD)wBack;
               if (iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                   &lpTGADVar->dwDataLen,
                                   lpTGADVar->lpDataBuff,&lpTGADVar->bEOF))
								   return iReturn;
               lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
               lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
            }

            wValue     = *(LPWORD)lpTGADVar->lpBgnBuff;
            *lpImage++ = (BYTE)( ((wValue    ) & 0x1F)<<3 );
            *lpImage++ = (BYTE)( ((wValue>> 5) & 0x1F)<<3 );
            *lpImage++ = (BYTE)( ((wValue>>10) & 0x1F)<<3 );
            lpTGADVar->lpBgnBuff += 2;
            if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                 !lpTGADVar->bEOF )
            {
               if (iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                   &lpTGADVar->dwDataLen,
                                   lpTGADVar->lpDataBuff,&lpTGADVar->bEOF))
								   return iReturn;
               lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
               lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
            }
         }
         break;
      }
      case 32:
      {
         for(wi=0;wi<lpTGADVar->wWidth;wi++)
         {
            if ( (lpTGADVar->lpBgnBuff + 4) > lpTGADVar->lpEndBuff )
            {
               wBack   = (WORD)(lpTGADVar->lpEndBuff - lpTGADVar->lpBgnBuff);
		       m_SrcFile->Seek(-(LONG)wBack,CFile::current);
               lpTGADVar->dwDataLen += (DWORD)wBack;
               if (iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                   &lpTGADVar->dwDataLen,
                                   lpTGADVar->lpDataBuff,&lpTGADVar->bEOF))
								   return iReturn;
               lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
               lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
            }

            *lpImage++ = *lpTGADVar->lpBgnBuff++;
            *lpImage++ = *lpTGADVar->lpBgnBuff++;
            *lpImage++ = *lpTGADVar->lpBgnBuff++;
            lpTGADVar->lpBgnBuff ++;
            if ( (lpTGADVar->lpBgnBuff == lpTGADVar->lpEndBuff) &&
                 !lpTGADVar->bEOF )
            {
              if (iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTGADVar->wMemLen,
                                   &lpTGADVar->dwDataLen,
                                   lpTGADVar->lpDataBuff,&lpTGADVar->bEOF))
								   return iReturn;
               lpTGADVar->lpBgnBuff = lpTGADVar->lpDataBuff;
               lpTGADVar->lpEndBuff = lpTGADVar->lpBgnBuff + lpTGADVar->wMemLen;
            }
         }
         break;
      }
   }
   return SUCCESS;
}

// 功能描述	: TGA的非压缩解码
// 参数		: CFile* m_DesFile,原文件LPTGAC_VAR lpTGACVar,PCX结构LPSTR lpTemp
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:

int CTGARaw::iEncodeLineTGA_RawData(CFile *m_DesFile, LPTGAC_VAR lpTGACVar, LPSTR lpImage)
{
	if ( (lpTGACVar->wByteCnt + lpTGACVar->wLineBytes) > MAX_BUFF_SIZE )
   {
      m_DesFile->Write(lpTGACVar->lpDataBuff,lpTGACVar->wByteCnt);
      lpTGACVar->wByteCnt  = 0;
      lpTGACVar->lpEndBuff = lpTGACVar->lpDataBuff;
   }

   memcpy(lpTGACVar->lpEndBuff,lpImage,lpTGACVar->wLineBytes);
   lpTGACVar->lpEndBuff += lpTGACVar->wLineBytes;
   lpTGACVar->wByteCnt  += lpTGACVar->wLineBytes;
   return SUCCESS;
}
