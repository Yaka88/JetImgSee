// ExportPCX.h: interface for the CExportPCX class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010530
// 文件名	: Exportpcx.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为pcx的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_EXPORTPCX_H__80907A35_3BB8_44AB_B011_0CECAF46028B__INCLUDED_)
#define AFX_EXPORTPCX_H__80907A35_3BB8_44AB_B011_0CECAF46028B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PCXRLE.h"
// 类名		: CExportPCX
// 父类名	: 无
// 功能描述	: 保存PCX类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

class CExportPCX  
{
public:
	CExportPCX();
	int iExportPCX(HDIB hInBmp,LPSTR lpszDstFName);
    int iCreatePCX(CFile* m_DstFile,LPBYTE lpSrcImage,LPPCXC_VAR lpPCXCVar,WORD wWidthBytes);

	virtual ~CExportPCX();
private:
	CPCXRLE m_pcxRLE;
};

#endif // !defined(AFX_EXPORTPCX_H__80907A35_3BB8_44AB_B011_0CECAF46028B__INCLUDED_)
