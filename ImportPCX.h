// ImportPCX.h: interface for the CImportPCX class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// 文件名	: Importpcx.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开pcx的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_IMPORTPCX_H__82514BD6_F205_4C98_BEE8_96E45C47A236__INCLUDED_)
#define AFX_IMPORTPCX_H__82514BD6_F205_4C98_BEE8_96E45C47A236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PCXRLE.h"
// 类名		: CImportPCX
// 父类名	: 无
// 功能描述	: 打开PCX类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CImportPCX  
{
public:
	CImportPCX();
	HDIB iImportPCX(LPSTR lpszSrcFName);
	int iCreateMetaPCX(CFile* m_srcFile,LPBYTE pOutPCX,
                   LPPCXD_VAR lpPCXDVar,WORD wWidthBytes);

	virtual ~CImportPCX();
private:
	CPCXRLE m_pcxRLE;
};

#endif // !defined(AFX_IMPORTPCX_H__82514BD6_F205_4C98_BEE8_96E45C47A236__INCLUDED_)
