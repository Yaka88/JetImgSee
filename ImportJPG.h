// ImportJPG.h: interface for the CImportJPG class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ImportJPG.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开JPG的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_IMPORTJPG_H__D6D47262_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_IMPORTJPG_H__D6D47262_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPGDMark.h"
#include "JPGDInit.h"
#include "JPGDMcu.h"
#include "JPGDExpa.h"
// 类名		: CImportJPG
// 父类名	: 无
// 功能描述	: 打开JPG类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

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
