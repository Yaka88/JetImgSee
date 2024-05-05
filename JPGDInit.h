// JPGDInit.h: interface for the CJPGDInit class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGDinit.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: �����ʼ����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGDINIT_H__EA727581_8AC0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDINIT_H__EA727581_8AC0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����		: CJPGDInit
// ������	: ��
// ��������	: �����ʼ��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGDInit  
{
public:
	void vDecode_Interleaved_Init(LPJPGD_VAR lpJDVAR);
	void vDecode_NonInterleaved_Init(LPJPGD_VAR lpJDVAR);
	void vDecode_Generate_HuffTBL(HANDLE hHuffTBL);
	void vDecode_HuffTBL_Init(LPJPGD_VAR lpJDVAR);
	void vDecode_Component_Init(LPJPGD_VAR lpJDVAR);
	int iDecode_Init(LPJPGD_VAR lpJDVAR);
	CJPGDInit();
	virtual ~CJPGDInit();
private:

};

#endif // !defined(AFX_JPGDINIT_H__EA727581_8AC0_11D3_93FC_0080C8DBBF78__INCLUDED_)
