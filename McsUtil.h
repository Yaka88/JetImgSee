// McsUtil.h: interface for the CMcsUtil class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: McsUtil.H
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理公共函数的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_MCSUTIL_H__05E4DDB6_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_MCSUTIL_H__05E4DDB6_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 类名		: CMcsUtil
// 父类名	: 无
// 功能描述	: 处理公共函数
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
class CMcsUtil  
{
public:
	WORD wCompute8BitRun(LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes);
	WORD wCompute24BitRun(LPSTR lpImage,LPSTR lpTemp,WORD wLineBytes);
	int iReadSrcData(CFile* m_SrcFile, LPWORD lpwMemLen, LPDWORD lpdwDataLen,LPSTR lpSrcBuff,LPBOOL lpbEOF);
	WORD wRoundUp (WORD wA, WORD wB);
	void vSwapByte(LPSTR lpSrcPtr, WORD wCount);
	void vSwapWord(LPSTR lpSrcPtr, WORD wCount);
	int iReadSrcData(CFile *m_SrcFile, LPDWORD lpdwMemLen, LPDWORD lpdwDataLen, LPSTR lpSrcBuff, LPBOOL lpbEOF);
	int iDeArrangeRGB(WORD wLineBytes, LPSTR lpImage);
	int iEnArrangeRGB(WORD wLineBytes, LPSTR lpImage);
	void vSuperInvertLine(LPSTR lpImage,WORD dwWidth,unsigned short BitCount);

	CMcsUtil();
	virtual ~CMcsUtil();

};
	
extern CMcsUtil  m_utility;


#endif // !defined(AFX_MCSUTIL_H__05E4DDB6_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)


