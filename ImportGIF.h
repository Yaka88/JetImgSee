// ImportGIF.h: interface for the CImportGIF class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010618
// 文件名	: Importgif.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开gif的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_IMPORTGIF_H__3E73A795_149A_45AE_86BA_61358197279C__INCLUDED_)
#define AFX_IMPORTGIF_H__3E73A795_149A_45AE_86BA_61358197279C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GIF_LZW.h"
// 类名		: CImportGIF
// 父类名	: 无
// 功能描述	: 打开GIF类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

class CImportGIF  
{
public:
	int iCreateMetaGIF(CFile* m_SrcFile,LPBYTE pOutGIF,LPGIFD_VAR lpGIFDVar,WORD wWidthBytes);
	HDIB iImportGIF(LPSTR lpszSrcFName);
	CImportGIF();
	virtual ~CImportGIF();
private:
	CGIF_LZW m_gif_LZW;
};

#endif // !defined(AFX_IMPORTGIF_H__3E73A795_149A_45AE_86BA_61358197279C__INCLUDED_)
