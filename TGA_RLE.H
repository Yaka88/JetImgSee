// TGA_RLE.h: interface for the CTGA_RLE class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// 文件名	: tga_RLE.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理tga的RLE压缩的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_TGA_RLE_H__CDBD96DE_67E4_46EB_9960_0A3A4081924A__INCLUDED_)
#define AFX_TGA_RLE_H__CDBD96DE_67E4_46EB_9960_0A3A4081924A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CTGA_RLE
// 父类名	: 无
// 功能描述	: 处理TGA的RLE压缩
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CTGA_RLE  
{
public:
	int iEncodeLineTGA_RLE(CFile *m_DesFile,LPTGAC_VAR lpTGACVar,LPSTR lpImage,LPSTR lpTemp,WORD wRunCnt);
	int iDecodeLineTGA_RLE(CFile *m_SrcFile,LPTGAD_VAR lpTGADVar,LPSTR lpImage);
	CTGA_RLE();
	virtual ~CTGA_RLE();

};

#endif // !defined(AFX_TGA_RLE_H__CDBD96DE_67E4_46EB_9960_0A3A4081924A__INCLUDED_)
