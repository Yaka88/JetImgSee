// TIF_LZW.h: interface for the CTIF_LZW class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: tif_LZW.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TIF的LZW压缩的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_TIF_LZW_H__F02D3D24_863D_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_TIF_LZW_H__F02D3D24_863D_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CTIF_LZW
// 父类名	: 无
// 功能描述	: 处理TIFF的LZW压缩
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 	: 
class CTIF_LZW  
{
public:
	int iDecodeTIF_LZW(CFile* m_SrcFile, LPBYTE pOutTiff, LPTIFD_VAR lpTIFDVar, WORD wWidthBytes);
	int iTIF_LZW_WriteCode(WORD wCode, LPBYTE lpbyCurrentBits, LPTIFC_VAR lpTIFCVar);
	int iEncodeTIF_LZW(LPBYTE& pInBmp, LPTIFC_VAR lpTIFCVar, WORD wWidthBytes);
	void vProcessPrePare(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, WORD wi);

	CTIF_LZW();
	virtual ~CTIF_LZW();


};

#endif // !defined(AFX_TIF_LZW_H__F02D3D24_863D_11D3_93FC_0080C8DBBF78__INCLUDED_)
