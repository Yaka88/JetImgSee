// JPGDExpa.h: interface for the CJPGDExpa class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGDExpa.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���뷴������ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGDEXPA_H__B737EC02_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDEXPA_H__B737EC02_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGDExpa
// ������	: ��
// ��������	: ���뷴����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGDExpa  
{
public:
	void vIntExpand(WORD wSrcRows, WORD wSrcCols, WORD wFullWidth, WORD wHorExpand, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst);
	void vHor2Expand(WORD wSrcRows,WORD wSrcCols, WORD wFullWidth, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst);
	void vNullExpand(WORD wSrcRows, WORD wSrcCols, WORD wFullWidth, LPSTR lpSrc, LPSTR lpDst);
	int iExpand(LPJPGD_VAR lpJDVAR, WORD wFullWidth, LPSTR* lpRealData, LPSTR* lpDstBuff);
	CJPGDExpa();
	virtual ~CJPGDExpa();

};

#endif // !defined(AFX_JPGDEXPA_H__B737EC02_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
