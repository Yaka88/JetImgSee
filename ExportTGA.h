// ExportTGA.h: interface for the CExportTGA class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: Exporttga.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为tga的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_EXPORTTGA_H__1F622D44_C861_42D4_B9C4_4BCA72B4CD31__INCLUDED_)
#define AFX_EXPORTTGA_H__1F622D44_C861_42D4_B9C4_4BCA72B4CD31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TGA_RLE.h"
#include "TGARaw.h"
// 类名		: CExportTGA
// 父类名	: 无
// 功能描述	: 保存TGA类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

class CExportTGA  
{
public:
	int iCreateTGA(CFile* m_DstFile,LPBYTE lpSrcImage,LPTGAC_VAR lpTGACVar,WORD wWidthBytes);
	int iExportTGA(HDIB hInBmp,LPSTR lpszDstFName,BOOL p_gbRLE);
	CExportTGA();
	virtual ~CExportTGA();
private:
	CTGA_RLE m_tgaRLE;
	CTGARaw m_tgaRaw;
	BOOL gbGray;
	BOOL gbRLE;

};

#endif // !defined(AFX_EXPORTTGA_H__1F622D44_C861_42D4_B9C4_4BCA72B4CD31__INCLUDED_)
