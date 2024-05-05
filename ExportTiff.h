// CExportTIF.h: interface for the CExportTIF class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: Exporttiff.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����Ϊtiff��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_CExportTIF_H__69331D83_B784_11D3_93FE_0080C8DBBF78__INCLUDED_)
#define AFX_CExportTIF_H__69331D83_B784_11D3_93FE_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ImportTiff.h"
#include "TIF_LZW.h"

#include "TIFCCCITT.h"
#include "TIFPackBits.h"

// ** TIFF 6.0 Color System Names
#define CS_WhiteIsZero		0
#define CS_BlackIsZero		1
#define CS_RGB				2
#define CS_Palette			3
#define CS_Transparency	    4
#define CS_CMYK				5
#define CS_YCbCr			6
#define CS_CIELab			8
// ����		: CExportTIF
// ������	: ��
// ��������	: ����TIF��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CExportTIF  
{
public:
	int iEncodeLineTIF_RawData(LPTIFC_VAR lpTIFCVar, LPSTR lpImage);
	int iCreateTIF(LPBYTE pInBmp, CFile *m_TempFile, LPTIFC_VAR lpTIFCVar, WORD wWidthBytes);
	int iExportTIF(HDIB hInBmp,const char *lpszDstFName,long wEncodeNumber);
	CExportTIF();
	virtual ~CExportTIF();
private:
	CTIF_PackBits m_EnPackBits;
	CTIF_LZW	m_LZW;
	CTIF_CCCITT m_CCCITT;

	WORD gwEncode ;
	BOOL m_bGray ;
	DWORD m_dwBlackOrWhite;
	WORD   wTIF_Pal[768];//256*3��WORD��TIFF color ����ÿ����ɫҪ����WORD�������ֽڣ���ͬ��BMP��

};

#endif // !defined(AFX_CExportTIF_H__69331D83_B784_11D3_93FE_0080C8DBBF78__INCLUDED_)
