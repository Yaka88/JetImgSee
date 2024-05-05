// JPGInit.h: interface for the CJPGInit class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGCinit.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: �����ʼ����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGINIT_H__05E4DDB7_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGINIT_H__05E4DDB7_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGinit
// ������	: ��
// ��������	: �����ʼ��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGInit  
{
public:
	void vEncode_Interleaved_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_NonInterleaved_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_Generate_HuffTBL(HANDLE hHuffTBL);
	void vEncode_HuffTBL_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_QuantTBL_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_Component_Init(LPJPGC_VAR lpJCVAR);
	int iEncode_Init(LPJPGC_VAR lpJCVAR);
	CJPGInit();
	virtual ~CJPGInit();
private:
	YUV YUV_Sample ;
public:
	WORD gwQuality ;
	BOOL gbGray ;
};

#endif // !defined(AFX_JPGINIT_H__05E4DDB7_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
