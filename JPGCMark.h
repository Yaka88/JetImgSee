// JPGCMark.h: interface for the CJPGCMark class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: JPGCmark.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 编码处理MARK的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_JPGCMARK_H__0B749641_892D_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_JPGCMARK_H__0B749641_892D_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CJPGCMark
// 父类名	: 无
// 功能描述	: 编码处理MARK
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CJPGCMark  
{
public:
	int iWrite_SOS(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_DHT(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_SOF(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_DRI(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_DQT(CFile *m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWrite_APP0(CFile* m_DstFile, LPJPGC_VAR lpJCVAR);
	int iWriteJPGHeader(CFile* m_DstFile, LPJPGC_VAR lpJCVAR);
	CJPGCMark();
	virtual ~CJPGCMark();
private:
};

#endif // !defined(AFX_JPGCMARK_H__0B749641_892D_11D3_93FC_0080C8DBBF78__INCLUDED_)
