// JPGCMcu.h: interface for the CJPGCMcu class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGCMCU.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���봦��MCU��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 	: 
#if !defined(AFX_JPGCMCU_H__8CE30721_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCMCU_H__8CE30721_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPGCFdct.h"
#include "JPGCHuff.h"
// ����		: CJPGCMcu
// ������	: ��
// ��������	: ���봦��MCU
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGCMcu  
{
public:
	void vExtractBlock(LPSTR lpSrc, LPJPG_BLOCK DstBlock,  LPJPG_QUANT_TBL lpQuantPtr, WORD wWidthBytes);
	int iExtractMCU(CFile* m_DstFile, LPJPGC_VAR lpJCVAR, LPHANDLE lphCompData, WORD wMCU_NumRows);
	CJPGCMcu();
	virtual ~CJPGCMcu();
private:
    CJPGCHuff m_jpgchuff;
	CJPGCFdct m_jpgcfdct;
};

#endif // !defined(AFX_JPGCMCU_H__8CE30721_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
