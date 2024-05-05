// JPGCHuff.h: interface for the CJPGCHuff class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGChuff.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码HUFF算法的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGCHUFF_H__8CE30723_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCHUFF_H__8CE30723_8964_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGCHUFF
// 父类名	: 无
// 功能描述	: 编码HUFF算法
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGCHuff  
{
public:
	int iRestart_Encode(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iBits_Encode(CFile* m_DstFile, LPJPGC_VAR lpJCVAR, WORD wCode, WORD wSize);
	int iEntropy_Encode(CFile* m_DstFile, LPJPGC_VAR lpJCVAR, LPJPG_BLOCKROW MCU_Data);
	CJPGCHuff();
	virtual ~CJPGCHuff();

};

#endif // !defined(AFX_JPGCHUFF_H__8CE30723_8964_11D3_93FC_0080C8DBBF78__INCLUDED_)
