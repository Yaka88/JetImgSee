// TGARaw.cpp: implementation of the CTGARaw class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: TGARaw.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TGA����ѹ����ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
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
// ��������	: TGA�ķ�ѹ������
// ����		: CFile* m_srcFile,ԭ�ļ�LPTGAD_VAR lpTGADVar,PCX�ṹLPSTR lpTemp
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:

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

// ��������	: TGA�ķ�ѹ������
// ����		: CFile* m_DesFile,ԭ�ļ�LPTGAC_VAR lpTGACVar,PCX�ṹLPSTR lpTemp
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:

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
