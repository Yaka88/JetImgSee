// TGARaw.h: interface for the CTGARaw class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// 文件名	: TGARaw.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TGA的非压缩的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_TGARAW_H__1C7DD016_CBCA_4CB7_A28B_134914A1F7DA__INCLUDED_)
#define AFX_TGARAW_H__1C7DD016_CBCA_4CB7_A28B_134914A1F7DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CTGARaw
// 父类名	: 无
// 功能描述	: 处理TGA的非压缩
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CTGARaw  
{
public:
	int iEncodeLineTGA_RawData(CFile *m_DesFile,LPTGAC_VAR lpTGACVar,LPSTR lpImage);
	int iDecodeLineTGA_RawData(CFile *m_SrcFile,LPTGAD_VAR lpTGADVar,LPSTR lpImage);
	CTGARaw();
	virtual ~CTGARaw();

};

#endif // !defined(AFX_TGARAW_H__1C7DD016_CBCA_4CB7_A28B_134914A1F7DA__INCLUDED_)
