// PCXRLE.h: interface for the PCXRLE class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// �ļ���	: PCXRLE
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����PCX��RLEѹ����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 

#if !defined(AFX_PCXRLE_H__57DBF692_E73C_4198_A6E7_0033362A3256__INCLUDED_)
#define AFX_PCXRLE_H__57DBF692_E73C_4198_A6E7_0033362A3256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mcspcx.h"
// ����		: CPCSRLE
// ������	: ��
// ��������	: ����PCX��RLEѹ��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CPCXRLE  
{
public:
	CPCXRLE();
	int iDecodeLine(CFile* m_srcFile,LPPCXD_VAR lpPCXDVar,LPSTR lpTemp);
	int iEncodeLine(CFile* m_TempFile,LPPCXC_VAR lpPCXCVar,LPSTR lpTemp);
	virtual ~CPCXRLE();

};

#endif // !defined(AFX_PCXRLE_H__57DBF692_E73C_4198_A6E7_0033362A3256__INCLUDED_)