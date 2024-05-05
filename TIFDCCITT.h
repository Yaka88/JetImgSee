// TIFDCCITT.h: interface for the CTIF_DCCITT class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: TIFFDCCITT.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIFF��CCITT�����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_TIF_DCCITT_H__72AE3001_B547_11D3_93FE_0080C8DBBF78__INCLUDED_)
#define AFX_TIF_DCCITT_H__72AE3001_B547_11D3_93FE_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TIFFEOL 65
#define KEYTABSIZE 197
#define	MAXKEYSIZE 25*2
#define MODETABSIZE 11
#define MAXMODESIZE 9
// ����		: CTIF_DCCITT
// ������	: ��
// ��������	: ����TIFF��CCITT����
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 	: 
class CTIF_DCCITT  
{
public:
	CTIF_DCCITT();
	virtual ~CTIF_DCCITT();
	int iDecodeCCITTNGroup(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar,LPBYTE pOutTiff, WORD wStripNum,DWORD& dwRowCnt,DWORD& dwRowNum);
	void vReverseByte(BYTE* byChar);
	int iSearchModeTab(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, BYTE& byMask);
	int iSearchKeyTab(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, BOOL& bWhiteRun,BYTE& byMask);
	int iSearchLineHuffman(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, int* LineOffset1,BYTE& byMask);
	void WriteAline(LPSTR lpImage, int* LineOffset1,int iLineWidth,WORD wPhotoMet);
	int inline iReadDataToBuff(LPTIFD_VAR lpTIFDVar, CFile *m_SrcFile);

private:
	BOOL bIsTwoD;
	BOOL bIsGroup3;
	WORD wEol;
	WORD wEolLength;//Group3����ֻ����һά������һά�Ͷ�ά����ķ����������ǵ�EOL���Ȳ�ͬ�����ڵڶ��֣������ڵ�һ�ֵ�EOL���ټ�һ��TAGλ��ָʾ��һ�в���һά���Ƕ�ά�� 
	BOOL bIsLastLine;
    BOOL bEolInBound;//EOL�Ƿ�ʹ�������ֽڱ߽磻
};
extern int iWhiteRun[216];
extern int iBlackRun[216];
extern int iReturn ;
#endif // !defined(AFX_TIF_DCCITT_H__72AE3001_B547_11D3_93FE_0080C8DBBF78__INCLUDED_)
