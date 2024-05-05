// JPGCFdct.h: interface for the CJPGCFdct class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCFdct.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码DCT变换的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGCFDCT_H__8CE30722_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCFDCT_H__8CE30722_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGCFdct
// 父类名	: 无
// 功能描述	: 编码DCT变换
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGCFdct  
{
public:
	void vDCT_Forward(LPJPG_BLOCK Block);
	CJPGCFdct();
	virtual ~CJPGCFdct();

};

#endif // !defined(AFX_JPGCFDCT_H__8CE30722_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
