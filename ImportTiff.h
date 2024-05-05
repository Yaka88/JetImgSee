// ImportTIF.h: interface for the ImportTIF class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: Importtiff.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��tiff��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_ImportTIF_H__DBD33B56_B39C_11D3_93FD_0080C8DBBF78__INCLUDED_)
#define AFX_ImportTIF_H__DBD33B56_B39C_11D3_93FD_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TIF_LZW.h"

#include "TIFDCCITT.h"
#include "TIFPackBits.h"

// ����		: CImporTIF
// ������	: ��
// ��������	: ��TIF��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CImportTIF  
{
public:
	CImportTIF();
	virtual ~CImportTIF();

	int iDecodeLineTIF_RawData(CFile* m_SrcFile, LPTIFD_VAR lpTIFDVar, LPSTR lpImage);
	int iCreateBMPFile(CFile* m_SrcFile,LPBYTE pOutTiff, LPTIFD_VAR lpTIFDVar, WORD wWidthBytes);
	int iReadTiffHeader(CFile* m_SrcFile);
	HDIB iImportTIF(LPSTR lpszSrcFName); //

private:
	TIFHEADER			TIFH;
	TIFFTAG				TIFT;
	TIFD_VAR			TIFDVar;

	CTIF_PackBits m_EnPackBits;
	CTIF_LZW	m_LZW;
	CTIF_DCCITT  m_CCITT3;

	WORD m_wEncode ;
	BOOL m_bGray ;
	BOOL m_bB_W ;
};

#endif // !defined(AFX_ImportTIF_H__DBD33B56_B39C_11D3_93FD_0080C8DBBF78__INCLUDED_)
