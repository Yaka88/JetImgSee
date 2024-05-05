// ImportPCX.h: interface for the CImportPCX class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// �ļ���	: Importpcx.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��pcx��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_IMPORTPCX_H__82514BD6_F205_4C98_BEE8_96E45C47A236__INCLUDED_)
#define AFX_IMPORTPCX_H__82514BD6_F205_4C98_BEE8_96E45C47A236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PCXRLE.h"
// ����		: CImportPCX
// ������	: ��
// ��������	: ��PCX��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CImportPCX  
{
public:
	CImportPCX();
	HDIB iImportPCX(LPSTR lpszSrcFName);
	int iCreateMetaPCX(CFile* m_srcFile,LPBYTE pOutPCX,
                   LPPCXD_VAR lpPCXDVar,WORD wWidthBytes);

	virtual ~CImportPCX();
private:
	CPCXRLE m_pcxRLE;
};

#endif // !defined(AFX_IMPORTPCX_H__82514BD6_F205_4C98_BEE8_96E45C47A236__INCLUDED_)
