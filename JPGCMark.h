// JPGCMark.h: interface for the CJPGCMark class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGCmark.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���봦��MARK��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGCMARK_H__0B749641_892D_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCMARK_H__0B749641_892D_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGCMark
// ������	: ��
// ��������	: ���봦��MARK
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGCMark  
{
public:
	int iWrite_SOS(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_DHT(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_SOF(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_DRI(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_DQT(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_APP0(CFile* m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWriteJPGHeader(CFile* m_DstFile, LPJPGC_VAR lpJCVAR);
	CJPGCMark();
	virtual ~CJPGCMark();
private:
};

#endif // !defined(AFX_JPGCMARK_H__0B749641_892D_11D3_93FC_0080C8DBBF78__INCLUDED_)
