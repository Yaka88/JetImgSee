// ImportICO.h: interface for the CImportICO class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ImportICO.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开ICO,CUR的头文件
// 创建日期	: 2001/08/13
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_IMPORTICO_H__2BFCE536_2919_499B_8E7C_6AABF10B1ECF__INCLUDED_)
#define AFX_IMPORTICO_H__2BFCE536_2919_499B_8E7C_6AABF10B1ECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CImporICO
// 父类名	: 无
// 功能描述	: 打开ICO,CUR类
// 创建日期	: 2000/08/13
// 作者		: yaka
// 备注 	: 
class CImportICO  
{
public:
	HDIB iImportICO(LPSTR lpszSrcFName);
	CImportICO();
	virtual ~CImportICO();

};

#endif // !defined(AFX_IMPORTICO_H__2BFCE536_2919_499B_8E7C_6AABF10B1ECF__INCLUDED_)
