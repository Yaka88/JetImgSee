// JPGDInit.h: interface for the CJPGDInit class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDinit.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码初始化的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGDINIT_H__EA727581_8AC0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDINIT_H__EA727581_8AC0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 类名		: CJPGDInit
// 父类名	: 无
// 功能描述	: 解码初始化
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGDInit  
{
public:
	void vDecode_Interleaved_Init(LPJPGD_VAR lpJDVAR);
	void vDecode_NonInterleaved_Init(LPJPGD_VAR lpJDVAR);
	void vDecode_Generate_HuffTBL(HANDLE hHuffTBL);
	void vDecode_HuffTBL_Init(LPJPGD_VAR lpJDVAR);
	void vDecode_Component_Init(LPJPGD_VAR lpJDVAR);
	int iDecode_Init(LPJPGD_VAR lpJDVAR);
	CJPGDInit();
	virtual ~CJPGDInit();
private:

};

#endif // !defined(AFX_JPGDINIT_H__EA727581_8AC0_11D3_93FC_0080C8DBBF78__INCLUDED_)
