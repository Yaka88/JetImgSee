// ImportWMF.h: interface for the CImportWMF class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ImportWMF.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开WMF的头文件
// 创建日期	: 2001/08/08
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_IMPORTWMF_H__5D135019_E6CF_4958_8818_7867B5F6E62D__INCLUDED_)
#define AFX_IMPORTWMF_H__5D135019_E6CF_4958_8818_7867B5F6E62D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 类名		: CImporWMF
// 父类名	: 无
// 功能描述	: 打开WMF类
// 创建日期	: 2000/08/08
// 作者		: yaka
// 备注 	: 
class CImportWMF  
{
public:
	int iCreateMetaWMF(CFile *m_SrcFile,HENHMETAFILE *hEnhMeta);
	HDIB iImportWMF(LPSTR lpszSrcFName);
	CImportWMF();
	virtual ~CImportWMF();

};

#endif // !defined(AFX_IMPORTWMF_H__5D135019_E6CF_4958_8818_7867B5F6E62D__INCLUDED_)
