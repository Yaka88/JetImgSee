// ImportGIF.h: interface for the CImportGIF class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010618
// �ļ���	: Importgif.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��gif��ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_IMPORTGIF_H__3E73A795_149A_45AE_86BA_61358197279C__INCLUDED_)
#define AFX_IMPORTGIF_H__3E73A795_149A_45AE_86BA_61358197279C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GIF_LZW.h"
// ����		: CImportGIF
// ������	: ��
// ��������	: ��GIF��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

class CImportGIF  
{
public:
	int iCreateMetaGIF(CFile* m_SrcFile,LPBYTE pOutGIF,LPGIFD_VAR lpGIFDVar,WORD wWidthBytes);
	HDIB iImportGIF(LPSTR lpszSrcFName);
	CImportGIF();
	virtual ~CImportGIF();
private:
	CGIF_LZW m_gif_LZW;
};

#endif // !defined(AFX_IMPORTGIF_H__3E73A795_149A_45AE_86BA_61358197279C__INCLUDED_)
