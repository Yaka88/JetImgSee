// ExportJPG.h: interface for the CExportJPG class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ExportJPG.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为JPG的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_EXPORTJPG_H__05E4DDB8_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_EXPORTJPG_H__05E4DDB8_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPGCInit.h"
#include "JPGCMark.h"
#include "JPGCSAMP.h"
#include "JPGCMcu.h"
// 类名		: CExporJPG
// 父类名	: 无
// 功能描述	: 保存JPG类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

class CExportJPG  
{
public:
	BOOL Turn16To24(LPSTR lpImage,LONG wWidthBytes,LONG wDefRows);
	void vFreeEncodeMem(LPJPGC_VAR lpJCVAR);
	void vEdgeExpand(LPJPGC_VAR lpJCVAR, WORD wFullWidth, WORD wProcRows, LPSTR* lpSrcBuff);
	int iCreateJPG(CFile* m_DstFile, LPJPGC_VAR lpJCVAR, LPSTR lpImage, WORD wProcRows, WORD wWidthBytes, BOOL bEOI);
	int iExportJPGEncode(HDIB hInBmp, LPSTR lpszDstFName, short CompFactor,short HeadOffset);
	CExportJPG();
	virtual ~CExportJPG();
private:
	CJPGInit  m_jpginit;
	CJPGCMark m_jpgcmark;
	CJPGCSAMP m_jpgcsamp;
	CJPGCMcu  m_jpgcmcu;
    CJPGCHuff m_jpgchuff;
};

#endif // !defined(AFX_EXPORTJPG_H__05E4DDB8_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
