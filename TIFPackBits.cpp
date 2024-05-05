// TIF_PackBits.cpp: implementation of the CTIF_PackBits class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: TIFFPackbits.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIFF��Packbitsѹ����ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "TIFPackBits.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTIF_PackBits::CTIF_PackBits()
{
}

CTIF_PackBits::~CTIF_PackBits()
{

}

extern int iReturn ;

// ��������	: TIF��Packѹ������
// ����		:  LPSTR lpTemp, WORD wRunCnt�� LPSTR LPImage,ͼ���� LPTIFD_VAR lpTIFDVar, TIF�ṹ
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:
int CTIF_PackBits::iEncodeLineTIF_PackBits(LPTIFC_VAR lpTIFCVar, LPSTR lpImage, LPSTR lpTemp, WORD wRunCnt)
{
	// lpTmep���������һ����ÿ���ظ�Ƭ���ֽ���(һ��һ����ж���ظ�Ƭ.)
	LPSTR lpTempEnd;
	int iCount, iTempCount;

	iCount = 0;
	lpTempEnd = lpTemp + wRunCnt;//lpTemp��һ���ַ����飬����wRunCnt����
	while(lpTemp < lpTempEnd)
	{
		iTempCount = (int)(*lpTemp++ & 0xFF);
		if((iTempCount<3) && ((iCount + iTempCount) <= 128))
			iCount += iTempCount;
		else
		{
			if(iCount)
			{	
				// ** ��ͼ�������ظ�����ΪiCount,�����TIFF�ļ��еĳ���ֵΪiCount-1;
				*lpTIFCVar->lpEndBuff++ = (BYTE)(iCount-1);
				lpTIFCVar->wByteCount ++;//ʵ������д���ļ�һ���ֽڣ�����¼���ظ����ݵĳ��ȣ���

				memcpy(lpTIFCVar->lpEndBuff, lpImage,iCount);
				lpTIFCVar->lpEndBuff += iCount;//����д���ļ�һ�����ظ��ֽڣ��ȷ���lpEndBuff->lpDataBuff�С�
				lpTIFCVar->wByteCount += (WORD)iCount;
				lpImage += iCount;
				iCount = 0;
			}
			// ** �������ͼ�����ݵ��ظ����֡���ʵ�ʽ��������ֽڴ���һ���ظ����ݣ�
			*lpTIFCVar->lpEndBuff++ = (BYTE)(-(iTempCount - 1));//�󷴼�һ����������
			lpTIFCVar->wByteCount++;
			*lpTIFCVar->lpEndBuff ++ = *lpImage;
			lpTIFCVar->wByteCount++;
			lpImage += iTempCount;
		}
	}
	if(iCount)
	{
		*lpTIFCVar->lpEndBuff++ = (BYTE)(iCount - 1);
		lpTIFCVar->wByteCount++;
		memcpy(lpTIFCVar->lpEndBuff,lpImage, iCount);
		lpTIFCVar->lpEndBuff += iCount;
		lpTIFCVar->wByteCount += (WORD)iCount;
	}
	return SUCCESS;
}
// ��������	: TIF��Packѹ������
// ����		: CFile *m_SrcFile,ԭ�ļ� LPSTR LPImage,ͼ���� LPTIFD_VAR lpTIFDVar, TIF�ṹ
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:

int CTIF_PackBits::iDecodeLineTIF_PackBits(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, LPSTR lpImage)
{
	LPSTR lpImageEnd;
	WORD wBack;
	BYTE byChar;
	int iCount;

	lpImageEnd = lpImage + lpTIFDVar->wLineBytes;
	while(lpImage < lpImageEnd)
	{
		byChar = *lpTIFDVar->lpBgnBuff++;
		if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
		{
			if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
				return iReturn;
			lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
			lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
		}
		if(byChar & 0x80) //Ϊ�ظ���ͼ�����ݲ��֡�
		{
			if(byChar != 0x80)
			{
				iCount = -(int)(char)byChar + 1;
				byChar = *lpTIFDVar ->lpBgnBuff++;
				if((lpTIFDVar->lpBgnBuff ==  lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
				{
					if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
						return iReturn;
					lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
					lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
				}
				memset(lpImage,(int)byChar,iCount);
				lpImage += iCount;
			}
		}
		else //Ϊ���ظ����֡�
		{
			iCount = (int)byChar + 1;
			if((lpTIFDVar->lpBgnBuff + iCount) > lpTIFDVar->lpEndBuff)
			{
				wBack = (WORD)(lpTIFDVar->lpEndBuff - lpTIFDVar->lpBgnBuff);
				m_SrcFile->Seek(-(LONG)wBack,CFile::current);
				lpTIFDVar->dwDataLen =+ (DWORD)wBack;
				if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
					return iReturn;
				lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
				lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
			}
			
			memcpy(lpImage,lpTIFDVar->lpBgnBuff,iCount);
			lpImage += iCount;
			lpTIFDVar->lpBgnBuff += iCount;
			if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
			{
				if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
					return iReturn;
				lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
				lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
			}
		}
	}
	return SUCCESS;
}



