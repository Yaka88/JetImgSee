// ExportGIF.h: interface for the ExportGIF class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ExportGif.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����Ϊgif��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_EXPORTGIF_H__5724FAE0_50A3_41B0_840E_B484E0A47571__INCLUDED_)
#define AFX_EXPORTGIF_H__5724FAE0_50A3_41B0_840E_B484E0A47571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GIF_LZW.h"
// ����		: CExportGIF
// ������	: ��
// ��������	: ����GIF��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CExportGIF  
{
public:
	int iExportGIF(HDIB hInBmp,LPSTR lpszDstFName);
	int iCreateGIF(CFile* m_DstFile, LPBYTE lpSrcImage,LPGIFC_VAR lpGIFCVar,WORD wWidthBytes);
	CExportGIF();
	virtual ~CExportGIF();
private:
	CGIF_LZW m_gif_LZW;
};

#endif // !defined(AFX_EXPORTGIF_H__5724FAE0_50A3_41B0_840E_B484E0A47571__INCLUDED_)
