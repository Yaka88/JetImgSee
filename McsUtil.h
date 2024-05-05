// McsUtil.h: interface for the CMcsUtil class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: McsUtil.H
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ������������ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_MCSUTIL_H__05E4DDB6_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_MCSUTIL_H__05E4DDB6_88CB_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ����		: CMcsUtil
// ������	: ��
// ��������	: ����������
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
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


