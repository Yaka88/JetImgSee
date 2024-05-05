// BMP.h: interface for the CBMP class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: BMP.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理BMP的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_BMP_H__B17BFA99_AE42_434B_BFF1_CEABFC6333B4__INCLUDED_)
#define AFX_BMP_H__B17BFA99_AE42_434B_BFF1_CEABFC6333B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
// 类名		: CBMP
// 父类名	: 无
// 功能描述	: 处理BMP类
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

class CBMP  
{
public:
	CSize GetSize(LPSTR lpbi);
	CSize GetDPI(LPSTR lpbi);
	BOOL IsColor(LPSTR lpbi);
	WORD PaletteSize(LPSTR lpbi);
	HDIB iImportBMP(CFile *p_SrcFName);
	int iExportBMP(HDIB hDib, CFile *p_DstFName);
	CBMP();
	virtual ~CBMP();

};

#endif // !defined(AFX_BMP_H__B17BFA99_AE42_434B_BFF1_CEABFC6333B4__INCLUDED_)
