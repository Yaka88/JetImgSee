// ExportPCX.h: interface for the CExportPCX class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// �ļ���	: Exportpcx.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����Ϊpcx��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_EXPORTPCX_H__80907A35_3BB8_44AB_B011_0CECAF46028B__INCLUDED_)
#define AFX_EXPORTPCX_H__80907A35_3BB8_44AB_B011_0CECAF46028B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PCXRLE.h"
// ����		: CExportPCX
// ������	: ��
// ��������	: ����PCX��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CExportPCX  
{
public:
	CExportPCX();
	int iExportPCX(HDIB hInBmp,LPSTR lpszDstFName);
    int iCreatePCX(CFile* m_DstFile,LPBYTE lpSrcImage,LPPCXC_VAR lpPCXCVar,WORD wWidthBytes);

	virtual ~CExportPCX();
private:
	CPCXRLE m_pcxRLE;
};

#endif // !defined(AFX_EXPORTPCX_H__80907A35_3BB8_44AB_B011_0CECAF46028B__INCLUDED_)
