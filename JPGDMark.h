// JPGDMark.h: interface for the CJPGDMark class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGDmark.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 解码处理MARK的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGDMARK_H__D6D47261_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGDMARK_H__D6D47261_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGDmark
// 父类名	: 无
// 功能描述	: 解码处理MARK
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGDMark  
{
public:
	int iReadJPGHeader(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR);
	int iRead_SOS(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_DHT(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_SOF(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_DRI(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_DQT(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_App0(CFile* m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_App1(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	int iRead_App2(CFile *m_SrcFile, LPJPGD_VAR lpJDVAR, HANDLE hTemp);
	CJPGDMark();
	virtual ~CJPGDMark();
	BOOL gbGray;
private:

};

#endif // !defined(AFX_JPGDMARK_H__D6D47261_8A45_11D3_93FC_0080C8DBBF78__INCLUDED_)
