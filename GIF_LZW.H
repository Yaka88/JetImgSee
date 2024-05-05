// GIF_LZW.h: interface for the CGIF_LZW class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010618
// �ļ���	: GIF_LZW.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: GIF�ļ���LZW��������ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 	: 
#if !defined(AFX_GIF_LZW_H__0C3AAF83_E4A0_42C5_8B7C_2C5F5E4FEA67__INCLUDED_)
#define AFX_GIF_LZW_H__0C3AAF83_E4A0_42C5_8B7C_2C5F5E4FEA67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mcsgif.h"
// ����		: CGIF_LZW
// ������	: ��
// ��������	: ����GIF��LZWѹ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CGIF_LZW  
{
public:
	int iGIF_LZW_WriteCode(CFile *m_DstFile,WORD wCode,LPSTR lpSubBlock,LPBYTE lpbyCurrentBits,LPGIFC_VAR lpGIFCVar);
	int iEncodeGIF_LZW(LPBYTE& pInBmp,CFile *m_DstFile, LPGIFC_VAR lpGIFCVar, WORD wWidthBytes);
	int iDecodeGIF_LZW(CFile* m_SrcFile, LPBYTE pOutGIF, LPGIFD_VAR lpGIFDVar, WORD wWidthBytes);
	CGIF_LZW();
	virtual ~CGIF_LZW();
	BOOL   m_bInterlace;
	long	i_Imagesize;

};

#endif // !defined(AFX_GIF_LZW_H__0C3AAF83_E4A0_42C5_8B7C_2C5F5E4FEA67__INCLUDED_)