// JPGDMcu.h: interface for the CJPGDMcu class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDMCU.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码处理MCU的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGDMCU_H__B737EC01_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDMCU_H__B737EC01_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPGDHuff.h"
#include "JPGDIDct.h"
// 类名		: CJPGDMCU
// 父类名	: 无
// 功能描述	: 解码处理MCU
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGDMcu  
{
public:
	void vReverseDCT(LPJPGD_VAR lpJDVAR, LPHANDLE lphCompData, LPSTR* lpRealData, WORD wStartBlockRow);
	void vQuant_Zigzag(LPJPG_BLOCK SrcBlock, LPJPG_BLOCK DstBlock, LPJPG_QUANT_TBL lpQuantPtr);
	int iDisassembleMCU(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, LPHANDLE lphCompData);
	CJPGDMcu();
	virtual ~CJPGDMcu();
private:
	CJPGDHuff m_jpgdhuff;
	CJPGDIDct m_jpgdidct;
};

#endif // !defined(AFX_JPGDMCU_H__B737EC01_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
