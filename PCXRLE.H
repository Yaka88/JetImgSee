// PCXRLE.h: interface for the PCXRLE class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// 文件名	: PCXRLE
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理PCX的RLE压缩的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 

#if !defined(AFX_PCXRLE_H__57DBF692_E73C_4198_A6E7_0033362A3256__INCLUDED_)
#define AFX_PCXRLE_H__57DBF692_E73C_4198_A6E7_0033362A3256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mcspcx.h"
// 类名		: CPCSRLE
// 父类名	: 无
// 功能描述	: 处理PCX的RLE压缩
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CPCXRLE  
{
public:
	CPCXRLE();
	int iDecodeLine(CFile* m_srcFile,LPPCXD_VAR lpPCXDVar,LPSTR lpTemp);
	int iEncodeLine(CFile* m_TempFile,LPPCXC_VAR lpPCXCVar,LPSTR lpTemp);
	virtual ~CPCXRLE();

};

#endif // !defined(AFX_PCXRLE_H__57DBF692_E73C_4198_A6E7_0033362A3256__INCLUDED_)
