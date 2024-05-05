// ImportTGA.h: interface for the CImportTGA class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// �ļ���	: Importtga.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��tga��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_IMPORTTGA_H__DD5916FA_8FEC_4FC3_ADC0_34BF0A9A0EAE__INCLUDED_)
#define AFX_IMPORTTGA_H__DD5916FA_8FEC_4FC3_ADC0_34BF0A9A0EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TGA_RLE.h"
#include "TGARaw.h"
// ����		: CImportTGA
// ������	: ��
// ��������	: ��TGA��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CImportTGA  
{
public:
	int iCreateMetaTGA(CFile* m_SrcFile,LPBYTE pOutTGA,LPTGAD_VAR lpTGADVar,WORD wWidthBytes);
	HDIB iImportTGA(LPSTR lpszSrcFName);
	CImportTGA();
	virtual ~CImportTGA();
private:
	CTGA_RLE m_tgaRLE;
	CTGARaw m_tgaRaw;
};

#endif // !defined(AFX_IMPORTTGA_H__DD5916FA_8FEC_4FC3_ADC0_34BF0A9A0EAE__INCLUDED_)
