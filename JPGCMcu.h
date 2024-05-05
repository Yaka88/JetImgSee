// JPGCMcu.h: interface for the CJPGCMcu class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCMCU.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码处理MCU的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 	: 
#if !defined(AFX_JPGCMCU_H__8CE30721_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCMCU_H__8CE30721_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPGCFdct.h"
#include "JPGCHuff.h"
// 类名		: CJPGCMcu
// 父类名	: 无
// 功能描述	: 编码处理MCU
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGCMcu  
{
public:
	void vExtractBlock(LPSTR lpSrc, LPJPG_BLOCK DstBlock,  LPJPG_QUANT_TBL lpQuantPtr, WORD wWidthBytes);
	int iExtractMCU(CFile* m_DstFile, LPJPGC_VAR lpJCVAR, LPHANDLE lphCompData, WORD wMCU_NumRows);
	CJPGCMcu();
	virtual ~CJPGCMcu();
private:
    CJPGCHuff m_jpgchuff;
	CJPGCFdct m_jpgcfdct;
};

#endif // !defined(AFX_JPGCMCU_H__8CE30721_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
