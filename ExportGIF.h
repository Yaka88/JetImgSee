// ExportGIF.h: interface for the ExportGIF class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ExportGif.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为gif的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_EXPORTGIF_H__5724FAE0_50A3_41B0_840E_B484E0A47571__INCLUDED_)
#define AFX_EXPORTGIF_H__5724FAE0_50A3_41B0_840E_B484E0A47571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GIF_LZW.h"
// 类名		: CExportGIF
// 父类名	: 无
// 功能描述	: 保存GIF类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

class CExportGIF  
{
public:
	int iExportGIF(HDIB hInBmp,LPSTR lpszDstFName);
	int iCreateGIF(CFile* m_DstFile, LPBYTE lpSrcImage,LPGIFC_VAR lpGIFCVar,WORD wWidthBytes);
	CExportGIF();
	virtual ~CExportGIF();
private:
	CGIF_LZW m_gif_LZW;
};

#endif // !defined(AFX_EXPORTGIF_H__5724FAE0_50A3_41B0_840E_B484E0A47571__INCLUDED_)
