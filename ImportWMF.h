// ImportWMF.h: interface for the CImportWMF class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ImportWMF.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��WMF��ͷ�ļ�
// ��������	: 2001/08/08
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_IMPORTWMF_H__5D135019_E6CF_4958_8818_7867B5F6E62D__INCLUDED_)
#define AFX_IMPORTWMF_H__5D135019_E6CF_4958_8818_7867B5F6E62D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����		: CImporWMF
// ������	: ��
// ��������	: ��WMF��
// ��������	: 2000/08/08
// ����		: yaka
// ��ע 	: 
class CImportWMF  
{
public:
	int iCreateMetaWMF(CFile *m_SrcFile,HENHMETAFILE *hEnhMeta);
	HDIB iImportWMF(LPSTR lpszSrcFName);
	CImportWMF();
	virtual ~CImportWMF();

};

#endif // !defined(AFX_IMPORTWMF_H__5D135019_E6CF_4958_8818_7867B5F6E62D__INCLUDED_)
