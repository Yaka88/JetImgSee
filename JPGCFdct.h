// JPGCFdct.h: interface for the CJPGCFdct class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGCFdct.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����DCT�任��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGCFDCT_H__8CE30722_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCFDCT_H__8CE30722_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGCFdct
// ������	: ��
// ��������	: ����DCT�任
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGCFdct  
{
public:
	void vDCT_Forward(LPJPG_BLOCK Block);
	CJPGCFdct();
	virtual ~CJPGCFdct();

};

#endif // !defined(AFX_JPGCFDCT_H__8CE30722_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
