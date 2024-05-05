// ImportTIF.h: interface for the ImportTIF class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: Importtiff.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开tiff的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_ImportTIF_H__DBD33B56_B39C_11D3_93FD_0080C8DBBF78__INCLUDED_)
#define AFX_ImportTIF_H__DBD33B56_B39C_11D3_93FD_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TIF_LZW.h"

#include "TIFDCCITT.h"
#include "TIFPackBits.h"

// 类名		: CImporTIF
// 父类名	: 无
// 功能描述	: 打开TIF类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CImportTIF  
{
public:
	CImportTIF();
	virtual ~CImportTIF();

	int iDecodeLineTIF_RawData(CFile* m_SrcFile, LPTIFD_VAR lpTIFDVar, LPSTR lpImage);
	int iCreateBMPFile(CFile* m_SrcFile,LPBYTE pOutTiff, LPTIFD_VAR lpTIFDVar, WORD wWidthBytes);
	int iReadTiffHeader(CFile* m_SrcFile);
	HDIB iImportTIF(LPSTR lpszSrcFName); //

private:
	TIFHEADER			TIFH;
	TIFFTAG				TIFT;
	TIFD_VAR			TIFDVar;

	CTIF_PackBits m_EnPackBits;
	CTIF_LZW	m_LZW;
	CTIF_DCCITT  m_CCITT3;

	WORD m_wEncode ;
	BOOL m_bGray ;
	BOOL m_bB_W ;
};

#endif // !defined(AFX_ImportTIF_H__DBD33B56_B39C_11D3_93FD_0080C8DBBF78__INCLUDED_)
