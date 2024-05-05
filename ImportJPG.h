// ImportJPG.h: interface for the CImportJPG class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ImportJPG.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��JPG��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_IMPORTJPG_H__D6D47262_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_IMPORTJPG_H__D6D47262_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPGDMark.h"
#include "JPGDInit.h"
#include "JPGDMcu.h"
#include "JPGDExpa.h"
// ����		: CImportJPG
// ������	: ��
// ��������	: ��JPG��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CImportJPG  
{
public:
	BOOL Turn24To16(LPSTR lpImage,WORD biWidth,WORD wDefRows);
	void vFreeDecodeMem(LPJPGD_VAR lpJDVAR);
	int iCreatTmpJPG(CFile* m_SrcFile, LPBYTE pOutJpeg, LPJPGD_VAR lpJDVAR, WORD wWidthBytes);
	HDIB iTmportJPGDecode(LPSTR lpszSrcFName,long HeadOffset);
	CImportJPG();
	virtual ~CImportJPG();
private:
	CJPGDMark m_jpgdmark;
	CJPGDInit m_jpgdinit;
	CJPGDExpa m_jpgdexpa;
	CJPGDMcu  m_jpgdmcu;
};

#endif // !defined(AFX_IMPORTJPG_H__D6D47262_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_)
