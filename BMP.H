// BMP.h: interface for the CBMP class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: BMP.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����BMP��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_BMP_H__B17BFA99_AE42_434B_BFF1_CEABFC6333B4__INCLUDED_)
#define AFX_BMP_H__B17BFA99_AE42_434B_BFF1_CEABFC6333B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
// ����		: CBMP
// ������	: ��
// ��������	: ����BMP��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CBMP  
{
public:
	CSize GetSize(LPSTR lpbi);
	CSize GetDPI(LPSTR lpbi);
	BOOL IsColor(LPSTR lpbi);
	WORD PaletteSize(LPSTR lpbi);
	HDIB iImportBMP(CFile *p_SrcFName);
	int iExportBMP(HDIB hDib, CFile *p_DstFName);
	CBMP();
	virtual ~CBMP();

};

#endif // !defined(AFX_BMP_H__B17BFA99_AE42_434B_BFF1_CEABFC6333B4__INCLUDED_)