// TIF_PackBits.h: interface for the CTIF_PackBits class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: TIFFPackbits.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIFF��Packbitsѹ����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_TIF_PackBits_H__54C60401_8608_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_TIF_PackBits_H__54C60401_8608_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����		: CTIF_PackBits
// ������	: ��
// ��������	: ����TIFF��Packbitsѹ��
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 	: 
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
