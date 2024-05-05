// ExportTGA.h: interface for the CExportTGA class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: Exporttga.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����Ϊtga��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_EXPORTTGA_H__1F622D44_C861_42D4_B9C4_4BCA72B4CD31__INCLUDED_)
#define AFX_EXPORTTGA_H__1F622D44_C861_42D4_B9C4_4BCA72B4CD31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TGA_RLE.h"
#include "TGARaw.h"
// ����		: CExportTGA
// ������	: ��
// ��������	: ����TGA��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CExportTGA  
{
public:
	int iCreateTGA(CFile* m_DstFile,LPBYTE lpSrcImage,LPTGAC_VAR lpTGACVar,WORD wWidthBytes);
	int iExportTGA(HDIB hInBmp,LPSTR lpszDstFName,BOOL p_gbRLE);
	CExportTGA();
	virtual ~CExportTGA();
private:
	CTGA_RLE m_tgaRLE;
	CTGARaw m_tgaRaw;
	BOOL gbGray;
	BOOL gbRLE;

};

#endif // !defined(AFX_EXPORTTGA_H__1F622D44_C861_42D4_B9C4_4BCA72B4CD31__INCLUDED_)
