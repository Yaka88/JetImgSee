// GIF_LZW.h: interface for the CGIF_LZW class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010618
// 文件名	: GIF_LZW.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: GIF文件的LZW编码解码的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 	: 
#if !defined(AFX_GIF_LZW_H__0C3AAF83_E4A0_42C5_8B7C_2C5F5E4FEA67__INCLUDED_)
#define AFX_GIF_LZW_H__0C3AAF83_E4A0_42C5_8B7C_2C5F5E4FEA67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mcsgif.h"
// 类名		: CGIF_LZW
// 父类名	: 无
// 功能描述	: 处理GIF的LZW压缩类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

class CGIF_LZW  
{
public:
	int iGIF_LZW_WriteCode(CFile *m_DstFile,WORD wCode,LPSTR lpSubBlock,LPBYTE lpbyCurrentBits,LPGIFC_VAR lpGIFCVar);
	int iEncodeGIF_LZW(LPBYTE& pInBmp,CFile *m_DstFile, LPGIFC_VAR lpGIFCVar, WORD wWidthBytes);
	int iDecodeGIF_LZW(CFile* m_SrcFile, LPBYTE pOutGIF, LPGIFD_VAR lpGIFDVar, WORD wWidthBytes);
	CGIF_LZW();
	virtual ~CGIF_LZW();
	BOOL   m_bInterlace;
	long	i_Imagesize;

};

#endif // !defined(AFX_GIF_LZW_H__0C3AAF83_E4A0_42C5_8B7C_2C5F5E4FEA67__INCLUDED_)
