// JPGCSAMP.h: interface for the CJPGCSAMP class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCSAMP.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码采样的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGCSAMP_H__8CE30727_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCSAMP_H__8CE30727_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGCSAMP
// 父类名	: 无
// 功能描述	: 编码采样
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGCSAMP  
{
public:
	void vIntSubSample(WORD wDstRows, WORD wDstCols, WORD wFullWidth, WORD wHorExpand, WORD wVerExpand, LPSTR lpSrc, LPSTR lpDst);
	void vNullSubSample(WORD wDstRows, WORD wDstCols, WORD wFullWidth, LPSTR lpSrc, LPSTR lpDst);
	int iSubSample(LPJPGC_VAR lpJCVAR, WORD wFullWidth, LPSTR* lpSrcBuff, LPHANDLE lphCompData);
	CJPGCSAMP();
	virtual ~CJPGCSAMP();

};

#endif // !defined(AFX_JPGCSAMP_H__8CE30727_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
