// JPGDIDct.h: interface for the CJPGDIDct class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGDidct.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����DCT�任��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGDIDCT_H__B737EC04_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDIDCT_H__B737EC04_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGDidct
// ������	: ��
// ��������	: ����DCT�任
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGDIDct  
{
public:
	void vDCT_Inverse(LPJPG_BLOCK Block);
	CJPGDIDct();
	virtual ~CJPGDIDct();

};

#endif // !defined(AFX_JPGDIDCT_H__B737EC04_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
