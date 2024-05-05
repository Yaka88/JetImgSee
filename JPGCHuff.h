// JPGCHuff.h: interface for the CJPGCHuff class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGChuff.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����HUFF�㷨��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGCHUFF_H__8CE30723_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCHUFF_H__8CE30723_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGCHUFF
// ������	: ��
// ��������	: ����HUFF�㷨
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGCHuff  
{
public:
	int iRestart_Encode(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iBits_Encode(CFile* m_DstFile, LPJPGC_VAR lpJCVAR, WORD wCode, WORD wSize);
	int iEntropy_Encode(CFile* m_DstFile, LPJPGC_VAR lpJCVAR, LPJPG_BLOCKROW MCU_Data);
	CJPGCHuff();
	virtual ~CJPGCHuff();

};

#endif // !defined(AFX_JPGCHUFF_H__8CE30723_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
