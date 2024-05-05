// TIF_PackBits.h: interface for the CTIF_PackBits class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: TIFFPackbits.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TIFF的Packbits压缩的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_TIF_PackBits_H__54C60401_8608_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_TIF_PackBits_H__54C60401_8608_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 类名		: CTIF_PackBits
// 父类名	: 无
// 功能描述	: 处理TIFF的Packbits压缩
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 	: 
class CTIF_PackBits  
{
public:

	int iDecodeLineTIF_PackBits(CFile* m_SrcFile, LPTIFD_VAR lpTIFDVar, LPSTR lpImage);
	int iEncodeLineTIF_PackBits(LPTIFC_VAR lpTIFCVar, LPSTR lpImage, LPSTR lpTemp,WORD wRunCnt);

	CTIF_PackBits();
	virtual ~CTIF_PackBits();
private:

};

#endif // !defined(AFX_TIF_PackBits_H__54C60401_8608_11D3_93FC_0080C8DBBF78__INCLUDED_)
