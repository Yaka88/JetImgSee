// JPGInit.h: interface for the CJPGInit class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCinit.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码初始化的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGINIT_H__05E4DDB7_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGINIT_H__05E4DDB7_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGinit
// 父类名	: 无
// 功能描述	: 解码初始化
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGInit  
{
public:
	void vEncode_Interleaved_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_NonInterleaved_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_Generate_HuffTBL(HANDLE hHuffTBL);
	void vEncode_HuffTBL_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_QuantTBL_Init(LPJPGC_VAR lpJCVAR);
	void vEncode_Component_Init(LPJPGC_VAR lpJCVAR);
	int iEncode_Init(LPJPGC_VAR lpJCVAR);
	CJPGInit();
	virtual ~CJPGInit();
private:
	YUV YUV_Sample ;
public:
	WORD gwQuality ;
	BOOL gbGray ;
};

#endif // !defined(AFX_JPGINIT_H__05E4DDB7_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
