// JPGDIDct.h: interface for the CJPGDIDct class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDidct.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码DCT变换的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGDIDCT_H__B737EC04_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDIDCT_H__B737EC04_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGDidct
// 父类名	: 无
// 功能描述	: 解码DCT变换
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGDIDct  
{
public:
	void vDCT_Inverse(LPJPG_BLOCK Block);
	CJPGDIDct();
	virtual ~CJPGDIDct();

};

#endif // !defined(AFX_JPGDIDCT_H__B737EC04_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
