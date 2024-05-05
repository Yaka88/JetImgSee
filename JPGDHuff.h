// JPGDHuff.h: interface for the CJPGDHuff class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGDhuff.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����HUFF�㷨��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGDHUFF_H__B737EC03_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDHUFF_H__B737EC03_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGDhuff
// ������	: ��
// ��������	: ����HUFF�㷨
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGDHuff  
{
public:
	int HUFF_EXTEND(MYWORD iDC, MYWORD iAC);
	int iRestart_Decode(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR);
	int iHuff_Decode(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, LPJPGD_HUFF_TBL lpHuffPtr, LPMYWORD lpiVal);
	int iEntropy_Decode(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, LPJPG_BLOCKROW MCU_Data);
	CJPGDHuff();
	virtual ~CJPGDHuff();
private:

};

#endif // !defined(AFX_JPGDHUFF_H__B737EC03_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
