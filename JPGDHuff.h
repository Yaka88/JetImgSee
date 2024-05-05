// JPGDHuff.h: interface for the CJPGDHuff class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDhuff.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码HUFF算法的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGDHUFF_H__B737EC03_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDHUFF_H__B737EC03_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGDhuff
// 父类名	: 无
// 功能描述	: 解码HUFF算法
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGDHuff  
{
public:
	int HUFF_EXTEND(MYWORD iDC, MYWORD iAC);
	int iRestart_Decode(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR);
	int iHuff_Decode(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, LPJPGD_HUFF_TBL lpHuffPtr, LPMYWORD lpiVal);
	int iEntropy_Decode(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, LPJPG_BLOCKROW MCU_Data);
	CJPGDHuff();
	virtual ~CJPGDHuff();
private:

};

#endif // !defined(AFX_JPGDHUFF_H__B737EC03_8BA0_11D3_93FC_0080C8DBBF78__INCLUDED_)
