// JPGDExpa.h: interface for the CJPGDExpa class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDExpa.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码反采样的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGDEXPA_H__B737EC02_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDEXPA_H__B737EC02_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGDExpa
// 父类名	: 无
// 功能描述	: 解码反采样
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGDExpa  
{
public:
	void vIntExpand(WORD wSrcRows, WORD wSrcCols, WORD wFullWidth, WORD wHorExpand, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst);
	void vHor2Expand(WORD wSrcRows,WORD wSrcCols, WORD wFullWidth, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst);
	void vNullExpand(WORD wSrcRows, WORD wSrcCols, WORD wFullWidth, LPSTR lpSrc, LPSTR lpDst);
	int iExpand(LPJPGD_VAR lpJDVAR, WORD wFullWidth, LPSTR* lpRealData, LPSTR* lpDstBuff);
	CJPGDExpa();
	virtual ~CJPGDExpa();

};

#endif // !defined(AFX_JPGDEXPA_H__B737EC02_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
