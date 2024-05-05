// McsUtil.cpp: implementation of the CMcsUtil class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: McsUtil.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ������������ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
//#include "McsUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMcsUtil::CMcsUtil()
{

}

CMcsUtil::~CMcsUtil()
{

}

extern int iReturn;
CMcsUtil m_utility;
// ��������	: �����ֽ�
// ����		: LPSTR lpSrcPtr, WORD wCount
// ����ֵ	: void
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
void CMcsUtil::vSwapByte(LPSTR lpSrcPtr, WORD wCount)
{
	union
	{
		WORD wTemp;
		BYTE byTemp[2];
	} SWAPBYTE;
	while( wCount --)
	{
		SWAPBYTE.wTemp = *(LPWORD)lpSrcPtr;
		*lpSrcPtr ++ = SWAPBYTE.byTemp[1];
		*lpSrcPtr ++ = SWAPBYTE.byTemp[0];
	}

}
// ��������	: ������
// ����		: LPSTR lpSrcPtr, WORD wCount
// ����ֵ	: void
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
void CMcsUtil::vSwapWord(LPSTR lpSrcPtr, WORD wCount)
{
	union
	{
		DWORD dwTemp;
		BYTE byTemp[4];
	} SWAPWORD;
	while( wCount --)
	{
		SWAPWORD.dwTemp = *(LPDWORD)lpSrcPtr;
		*lpSrcPtr ++ = SWAPWORD.byTemp[3];
		*lpSrcPtr ++ = SWAPWORD.byTemp[2];
		*lpSrcPtr ++ = SWAPWORD.byTemp[1];
		*lpSrcPtr ++ = SWAPWORD.byTemp[0];

	}
}
WORD CMcsUtil::wRoundUp(WORD wA, WORD wB)
{
	/* Compute a rounded up to next multiple of wB: wA>=0, wB>0 */
	wA += (wB - 1);
	return wA - (wA % wB);
}
// ��������	: ��һ�����ݴ�������,�������ݴ�����MAX_BUFF_SIZE,�������(��lpTIFDVar->bEOFΪ��־
// ����		: CFile *m_SrcFile,ԭ�ļ� LPDWORD lpdwMemLen,�ڴ泤�� LPDWORD lpdwDataLen,���ݳ��� LPSTR lpSrcBuff, LPBOOL lpbEOF
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CMcsUtil::iReadSrcData(CFile *m_SrcFile, LPWORD lpwMemLen, LPDWORD lpdwDataLen, LPSTR lpSrcBuff, LPBOOL lpbEOF)
{
	if((*lpdwDataLen) > (DWORD)(*lpwMemLen))
		(*lpdwDataLen) -= (DWORD)(*lpwMemLen);
	else
	{
		(*lpwMemLen) = (WORD)(*lpdwDataLen);
		(*lpbEOF) = TRUE;
	}
	try
	{ m_SrcFile->Read(lpSrcBuff,(*lpwMemLen));}
	catch(CFileException e)
	{
		e.ReportError();
		m_SrcFile->Close();
		iReturn = READSOURCEIMG_ERR;
		return iReturn;
	}
	return 0;

}



// ��������	: ��һ�����ݴ�������,�������ݴ�����MAX_BUFF_SIZE,�������(��lpTIFDVar->bEOFΪ��־
// ����		: CFile *m_SrcFile,ԭ�ļ� LPDWORD lpdwMemLen,�ڴ泤�� LPDWORD lpdwDataLen,���ݳ��� LPSTR lpSrcBuff, LPBOOL lpbEOF
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CMcsUtil::iReadSrcData(CFile *m_SrcFile, LPDWORD lpdwMemLen, LPDWORD lpdwDataLen, LPSTR lpSrcBuff, LPBOOL lpbEOF)
{
	if((*lpdwDataLen) > (*lpdwMemLen))
		(*lpdwDataLen) -= (*lpdwMemLen);
	else
	{
		(*lpdwMemLen) = (*lpdwDataLen);
		(*lpbEOF) = TRUE;
	}
	try
	{ m_SrcFile->Read(lpSrcBuff,(*lpdwMemLen));}
	catch(CFileException e)
	{
		e.ReportError();
		m_SrcFile->Close();
		iReturn = READSOURCEIMG_ERR;
		return iReturn;
	}
	return 0;
}



// ��������	: RRR...GGG...BBB...���з�ʽ�ĳ�RGBRGBRGBRGB...��ʽ��
// ����		: LPSTR lpImage��WORD wLineBytes���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CMcsUtil::iDeArrangeRGB(WORD wLineBytes, LPSTR lpImage)
{
	//WORD wLineBytes = lpTIFDVar->wLineBytes;
	HGLOBAL hTempGlobal = GlobalAlloc(GHND,wLineBytes);
	char* lpbDst = (char*)GlobalLock(hTempGlobal);
	char* lpbDstTemp = NULL;
	if(!(lpbDstTemp = lpbDst))
	{
		AfxMessageBox("����ʱ��λ��ת���ɵ�λ�������ڴ�ʧ��!");
		iReturn = DERGB_AllocMemERR;
		return iReturn;
	}
	char* lpbTemp = lpImage;
	for(int i=0; i<(wLineBytes/3); i++)
	{
		lpImage = lpbTemp+i;
		for(int j=0; j<3; j++)
		{			
			*lpbDst = *lpImage;
			lpbDst ++;
			lpImage += (wLineBytes/3);

		}
	}	
	lpImage = lpbTemp;
	memcpy(lpImage,lpbDstTemp,wLineBytes); //���ܶ������������ͷ�hTempGlobal����Ч.
	GlobalUnlock(hTempGlobal);
	GlobalFree(hTempGlobal);
	return SUCCESS;
}

// ��������	: ��RGBRGBRGBRGB...���з�ʽ�ĳ�RRR...GGG...BBB...��ʽ��
// ����		:  LPSTR lpImage��WORD wLineBytes���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CMcsUtil::iEnArrangeRGB(WORD wLineBytes, LPSTR lpImage)
{
	//WORD wLineBytes = lpTIFCVar->wLineBytes;
	HGLOBAL hTempGlobal = GlobalAlloc(GHND,wLineBytes);
	char* lpbDst = (char*)GlobalLock(hTempGlobal);
	char* lpbDstTemp = NULL;
	if(!(lpbDstTemp = lpbDst))
	{
		AfxMessageBox("����ʱ��λ��ת���ɶ�λ�������ڴ�ʧ��!");
		iReturn = ENRGB_AllocMemERR;
		return iReturn;
	}
	char* lpbTemp = lpImage;
	for(int i=0; i<3; i++)
	{
		lpImage = lpbTemp+i;
		for(int j=0; j<(wLineBytes/3); j++)
		{
			*lpbDst = *lpImage;
			lpbDst++;
			lpImage += 3;
		}
	}
	lpImage = lpbTemp;
	memcpy(lpImage,lpbDstTemp,wLineBytes); //���ܶ������������ͷ�hTempGlobal����Ч.
	GlobalUnlock(hTempGlobal);
	GlobalFree(hTempGlobal);
	return SUCCESS;
}

// ��������	: ��Դ洢��ͼ���Ǿ���ˮƽ��ת�ģ���ѹʱ���øú�����ͼ��ԭ��
// ����		: LPSTR lpImage,WORD dwWidth,unsigned short BitCount
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
void CMcsUtil::vSuperInvertLine(LPSTR lpImage,WORD dwWidth,unsigned short BitCount)
{
	HGLOBAL hConver = GlobalAlloc(GHND,dwWidth);
	char* lpbTempImg = (char*)GlobalLock(hConver);
	if(lpbTempImg == NULL)
	{
		AfxMessageBox("ˮƽ��תͼ��ʱ�����ڴ����!");
		return;
	}
	memcpy(lpbTempImg,lpImage,dwWidth);
	WORD wi;

	switch(BitCount)
	{
	case 1:
		{
			lpImage += (dwWidth-1);
			for(wi=0;wi<dwWidth;wi++) //��һ��������λ�ߵ���dwWidthΪ�ֽ�����
			{
				*lpImage = *lpbTempImg;
				lpImage--;
				lpbTempImg++;
			}
			for(wi=0;wi<dwWidth;wi++) //��һ���ֽ�8λ�ߵ�
			{
				unsigned char  tempchar1 ,tempchar2, tempchar3;
				tempchar1 = (tempchar1 & 0x00 );
				tempchar3 = tempchar3 | 0xff;
				for(int i=0;i<=7;i++)
				{
					tempchar2 = lpImage[wi];
					tempchar3 = (tempchar3 >> i);
					tempchar2 = (tempchar2 & tempchar3);
					tempchar2 = (tempchar2 >> (7-i));
					tempchar2 = (tempchar2 << i);
					tempchar1 = (tempchar1 | tempchar2);
					tempchar3 = (tempchar3 | 0xff);
				}
				lpImage[wi] = tempchar1 ;
			} 
			break;
		}
	case 4:
		{
			lpImage += (dwWidth-1);
			for(wi=0;wi<dwWidth;wi++) //��һ��������λ�ߵ���dwWidthΪ�ֽ�����
			{
				*lpImage = *lpbTempImg;
				lpImage--;
				lpbTempImg++;
			}
			for(wi=0;wi<dwWidth;wi++) //��һ���ֽ�ǰ��4λ�ߵ�
			{
				unsigned char  tempchar1;
				tempchar1 = lpImage[wi];
				lpImage[wi] = (lpImage[wi] << 4);
				tempchar1 = (tempchar1 >> 4);
				lpImage[wi] |= tempchar1;
			}
			break;
		}
	case 8:
		{
			lpImage += (dwWidth-1);
			for(wi=0;wi<dwWidth;wi++)
			{
				*lpImage = *lpbTempImg;
				lpImage--;
				lpbTempImg++;
			}
			break;
		}
	case 16:
		{
			lpImage += (dwWidth-2);
			for(wi=0;wi<(dwWidth/2);wi++) //��һ��������λ�ߵ���dwWidthΪ�ֽ�����
			{			
				for(int i=0;i<2;i++)
				{

					*lpImage = *lpbTempImg;
					lpImage++;
					lpbTempImg++;
				}
				lpImage -= 4;
			}
			break;
		}
	case 24:
		{
			lpImage += (dwWidth-3);
			for(wi=0;wi<(dwWidth/3);wi++)
			{			
				for(int i=0;i<3;i++)
				{

					*lpImage = *lpbTempImg;
					lpImage++;
					lpbTempImg++;
				}
				lpImage -= 6;
			}
			break;
		}

	default:
		AfxMessageBox("��֧������λ���ͼ��!");
		break;
	}

	GlobalUnlock(hConver);
	GlobalFree(hConver);
}

//add by yaka 20010608
// ��������	: ����RLEѹ��������һ��ͼ������(lpImage��)�ж��ٸ��ظ�Ƭ,ÿ���ظ�Ƭ���ֽڸ���
// ����		: LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes
// ����ֵ	: WORD �ظ�Ƭ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
WORD CMcsUtil::wCompute8BitRun(LPSTR lpImage, LPSTR lpTemp, WORD wLineBytes)
{
	LPSTR lpImageEnd;
   WORD  wRunCnt;
   WORD  wRunLen;

   wRunCnt    = 0;
   lpImageEnd = lpImage + wLineBytes;
   while( lpImage<lpImageEnd )
   {
      wRunLen = 1;
      while( (*lpImage == *(lpImage+1)) &&
             (wRunLen < 128) && ((lpImage+1) < lpImageEnd) )
      {
         wRunLen ++;
         lpImage ++;
      }
      *lpTemp++ = (BYTE)wRunLen;
      wRunCnt  ++;
      lpImage  ++;
   }
   return wRunCnt;
}


// ��������	: ����RLEѹ��������һ��ͼ������(lpImage��)�ж��ٸ��ظ�Ƭ,ÿ���ظ�Ƭ���ֽڸ���
// ����		: LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes
// ����ֵ	: WORD �ظ�Ƭ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
//add by yaka 20010608
WORD CMcsUtil::wCompute24BitRun(LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes)
{
   LPSTR lpImageEnd;
   WORD  wRunCnt;
   WORD  wRunLen;

   wRunCnt    = 0;
   lpImageEnd = lpImage + wLineBytes;
   while( lpImage<lpImageEnd )
   {
      wRunLen = 1;
      while( (*(lpImage  ) == *(lpImage+3)) &&
             (*(lpImage+1) == *(lpImage+4)) &&
             (*(lpImage+2) == *(lpImage+5)) &&
             (wRunLen < 128) && ((lpImage+3) < lpImageEnd) )
      {
         wRunLen ++;
         lpImage += 3;
      }
      *lpTemp++ = (BYTE)wRunLen;
      wRunCnt  ++;
      lpImage  += 3;
   }
   return wRunCnt;
}

//add end