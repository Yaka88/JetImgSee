// JPGDMark.h: interface for the CJPGDMark class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGDmark.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���봦��MARK��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGDMARK_H__D6D47261_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDMARK_H__D6D47261_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGDmark
// ������	: ��
// ��������	: ���봦��MARK
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGDMark  
{
public:
	int iReadJPGHeader(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR);
	int iRead_SOS(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_DHT(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_SOF(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_DRI(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_DQT(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_App0(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_App1(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_App2(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	CJPGDMark();
	virtual ~CJPGDMark();
	BOOL gbGray;
private:

};

#endif // !defined(AFX_JPGDMARK_H__D6D47261_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_)
