// TGARaw.h: interface for the CTGARaw class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// �ļ���	: TGARaw.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TGA�ķ�ѹ����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_TGARAW_H__1C7DD016_CBCA_4CB7_A28B_134914A1F7DA__INCLUDED_)
#define AFX_TGARAW_H__1C7DD016_CBCA_4CB7_A28B_134914A1F7DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CTGARaw
// ������	: ��
// ��������	: ����TGA�ķ�ѹ��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CTGARaw  
{
public:
	int iEncodeLineTGA_RawData(CFile *m_DesFile,LPTGAC_VAR lpTGACVar,LPSTR lpImage);
	int iDecodeLineTGA_RawData(CFile *m_SrcFile,LPTGAD_VAR lpTGADVar,LPSTR lpImage);
	CTGARaw();
	virtual ~CTGARaw();

};

#endif // !defined(AFX_TGARAW_H__1C7DD016_CBCA_4CB7_A28B_134914A1F7DA__INCLUDED_)
