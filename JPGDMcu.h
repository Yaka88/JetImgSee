// JPGDMcu.h: interface for the CJPGDMcu class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGDMCU.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���봦��MCU��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGDMCU_H__B737EC01_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDMCU_H__B737EC01_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPGDHuff.h"
#include "JPGDIDct.h"
// ����		: CJPGDMCU
// ������	: ��
// ��������	: ���봦��MCU
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGDMcu  
{
public:
	void vReverseDCT(LPJPGD_VAR lpJDVAR, LPHANDLE lphCompData, LPSTR* lpRealData, WORD wStartBlockRow);
	void vQuant_Zigzag(LPJPG_BLOCK SrcBlock, LPJPG_BLOCK DstBlock, LPJPG_QUANT_TBL lpQuantPtr);
	int iDisassembleMCU(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, LPHANDLE lphCompData);
	CJPGDMcu();
	virtual ~CJPGDMcu();
private:
	CJPGDHuff m_jpgdhuff;
	CJPGDIDct m_jpgdidct;
};

#endif // !defined(AFX_JPGDMCU_H__B737EC01_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
