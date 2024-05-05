// TIF_LZW.h: interface for the CTIF_LZW class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: tif_LZW.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIF��LZWѹ����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_TIF_LZW_H__F02D3D24_863D_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_TIF_LZW_H__F02D3D24_863D_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CTIF_LZW
// ������	: ��
// ��������	: ����TIFF��LZWѹ��
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 	: 
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