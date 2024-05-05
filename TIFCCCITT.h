// TIF_CCCITT.h: interface for the CTIF_CCCITT class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: TIFFCCCITT.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIFF��CCITT�����ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#if !defined(AFX_TIF_CCCITT_H__7D3E73E1_8798_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_TIF_CCCITT_H__7D3E73E1_8798_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����		: CTIF_CCCITT
// ������	: ��
// ��������	: ����TIFF��CCITT����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
class CTIF_CCCITT  
{
public:
	int iEncodeLineTiff_Huffman(LPTIFC_VAR lpTIFCVar, LPSTR lpImage);
	CTIF_CCCITT();
	virtual ~CTIF_CCCITT();

	void vEnOneDPartline(LPTIFC_VAR lpTIFCVar,DWORD& dwTempCode,WORD& wLeftBits,int iFromPoint,int iToPoint,BOOL bWhiteRun);
	void vEnGroup3WriteDate(LPTIFC_VAR lpTIFCVar, DWORD& dwTempCode,WORD wCodeWord, WORD wCodeLen,WORD& wLeftBits);
	int iEncodeGroup3TwoDline(LPTIFC_VAR lpTIFCVar, LPSTR lpImage,DWORD dwNumInStrip, int* iLastLineOffset,DWORD& dwLastNumChange);
	int iEncodeGroup3OneDline(LPTIFC_VAR lpTIFCVar, LPSTR lpImage,DWORD dwNumInStrip, int* LastLineOffset,DWORD& dwLastNumChange);
	int iEncodeGroup3(LPTIFC_VAR lpTIFCVar,LPSTR lpImage,DWORD dwStripNums,DWORD dwLineNums);
	void vEnGroup3InEnd1(LPTIFC_VAR lpTIFCVar,DWORD& dwTempCode,WORD& wLeftBits,DWORD dwNumInStrip);
	void vEnGroup3InEnd2(LPTIFC_VAR lpTIFCVar,DWORD& dwTempCode,WORD& wLeftBits,DWORD dwNumInStrip);

private:

	WORD wEol;
	WORD wEolLength;//Group3����ֻ����һά������һά�Ͷ�ά����ķ����������ǵ�EOL���Ȳ�ͬ�����ڵڶ��֣������ڵ�һ�ֵ�EOL���ټ�һ��TAGλ��ָʾ��һ�в���һά���Ƕ�ά�� 
    BOOL bEolInBound;//EOL�Ƿ�ʹ�������ֽڱ߽磻
	BOOL bAddRTC ;//because the RTC(six EOLs) should be added after the last lines in image;
	BOOL bAddHeader;//because a header(000000000001)shoud be add before the first line in every strip when encoded with Group3;
};

#endif // !defined(AFX_TIF_CCCITT_H__7D3E73E1_8798_11D3_93FC_0080C8DBBF78__INCLUDED_)
