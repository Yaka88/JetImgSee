// JPGCSAMP.h: interface for the CJPGCSAMP class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: JPGCSAMP.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ���������ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_JPGCSAMP_H__8CE30727_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCSAMP_H__8CE30727_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CJPGCSAMP
// ������	: ��
// ��������	: �������
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CJPGCSAMP  
{
public:
	void vIntSubSample(WORD wDstRows, WORD wDstCols, WORD wFullWidth, WORD wHorExpand, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst);
	void vNullSubSample(WORD wDstRows, WORD wDstCols, WORD wFullWidth, LPSTR lpSrc, LPSTR lpDst);
	int iSubSample(LPJPGC_VAR lpJCVAR, WORD wFullWidth, LPSTR* lpSrcBuff, LPHANDLE lphCompData);
	CJPGCSAMP();
	virtual ~CJPGCSAMP();

};

#endif // !defined(AFX_JPGCSAMP_H__8CE30727_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)