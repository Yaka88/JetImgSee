// ImportTGA.h: interface for the CImportTGA class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// 文件名	: Importtga.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开tga的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_IMPORTTGA_H__DD5916FA_8FEC_4FC3_ADC0_34BF0A9A0EAE__INCLUDED_)
#define AFX_IMPORTTGA_H__DD5916FA_8FEC_4FC3_ADC0_34BF0A9A0EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TGA_RLE.h"
#include "TGARaw.h"
// 类名		: CImportTGA
// 父类名	: 无
// 功能描述	: 打开TGA类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CImportTGA  
{
public:
	int iCreateMetaTGA(CFile* m_SrcFile,LPBYTE pOutTGA,LPTGAD_VAR lpTGADVar,WORD wWidthBytes);
	HDIB iImportTGA(LPSTR lpszSrcFName);
	CImportTGA();
	virtual ~CImportTGA();
private:
	CTGA_RLE m_tgaRLE;
	CTGARaw m_tgaRaw;
};

#endif // !defined(AFX_IMPORTTGA_H__DD5916FA_8FEC_4FC3_ADC0_34BF0A9A0EAE__INCLUDED_)
