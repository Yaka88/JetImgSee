// ImportICO.h: interface for the CImportICO class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ImportICO.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��ICO,CUR��ͷ�ļ�
// ��������	: 2001/08/13
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_IMPORTICO_H__2BFCE536_2919_499B_8E7C_6AABF10B1ECF__INCLUDED_)
#define AFX_IMPORTICO_H__2BFCE536_2919_499B_8E7C_6AABF10B1ECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CImporICO
// ������	: ��
// ��������	: ��ICO,CUR��
// ��������	: 2000/08/13
// ����		: yaka
// ��ע 	: 
class CImportICO  
{
public:
	HDIB iImportICO(LPSTR lpszSrcFName);
	CImportICO();
	virtual ~CImportICO();

};

#endif // !defined(AFX_IMPORTICO_H__2BFCE536_2919_499B_8E7C_6AABF10B1ECF__INCLUDED_)
