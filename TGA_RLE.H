// TGA_RLE.h: interface for the CTGA_RLE class.
//
//////////////////////////////////////////////////////////////////////
//create by yaka 20010607
// �ļ���	: tga_RLE.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����tga��RLEѹ����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_TGA_RLE_H__CDBD96DE_67E4_46EB_9960_0A3A4081924A__INCLUDED_)
#define AFX_TGA_RLE_H__CDBD96DE_67E4_46EB_9960_0A3A4081924A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CTGA_RLE
// ������	: ��
// ��������	: ����TGA��RLEѹ��
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CTGA_RLE  
{
public:
	int iEncodeLineTGA_RLE(CFile *m_DesFile,LPTGAC_VAR lpTGACVar,LPSTR lpImage,LPSTR lpTemp,WORD wRunCnt);
	int iDecodeLineTGA_RLE(CFile *m_SrcFile,LPTGAD_VAR lpTGADVar,LPSTR lpImage);
	CTGA_RLE();
	virtual ~CTGA_RLE();

};

#endif // !defined(AFX_TGA_RLE_H__CDBD96DE_67E4_46EB_9960_0A3A4081924A__INCLUDED_)