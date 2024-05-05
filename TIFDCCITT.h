// TIFDCCITT.h: interface for the CTIF_DCCITT class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: TIFFDCCITT.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TIFF的CCITT解码的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
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
// 类名		: CTIF_DCCITT
// 父类名	: 无
// 功能描述	: 处理TIFF的CCITT解码
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 	: 
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
	WORD wEolLength;//Group3可以只采用一维方法或一维和二维交替的方法，则他们的EOL长度不同，对于第二种，则是在第一种的EOL后再加一个TAG位，指示下一行采用一维还是二维； 
	BOOL bIsLastLine;
    BOOL bEolInBound;//EOL是否使他处于字节边界；
};
extern int iWhiteRun[216];
extern int iBlackRun[216];
extern int iReturn ;
#endif // !defined(AFX_TIF_DCCITT_H__72AE3001_B547_11D3_93FE_0080C8DBBF78__INCLUDED_)
