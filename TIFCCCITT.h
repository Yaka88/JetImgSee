// TIF_CCCITT.h: interface for the CTIF_CCCITT class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: TIFFCCCITT.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TIFF的CCITT编码的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#if !defined(AFX_TIF_CCCITT_H__7D3E73E1_8798_11D3_93FC_0080C8DBBF78__INCLUDED_)
#define AFX_TIF_CCCITT_H__7D3E73E1_8798_11D3_93FC_0080C8DBBF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 类名		: CTIF_CCCITT
// 父类名	: 无
// 功能描述	: 处理TIFF的CCITT编码
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
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
	WORD wEolLength;//Group3可以只采用一维方法或一维和二维交替的方法，则他们的EOL长度不同，对于第二种，则是在第一种的EOL后再加一个TAG位，指示下一行采用一维还是二维； 
    BOOL bEolInBound;//EOL是否使他处于字节边界；
	BOOL bAddRTC ;//because the RTC(six EOLs) should be added after the last lines in image;
	BOOL bAddHeader;//because a header(000000000001)shoud be add before the first line in every strip when encoded with Group3;
};

#endif // !defined(AFX_TIF_CCCITT_H__7D3E73E1_8798_11D3_93FC_0080C8DBBF78__INCLUDED_)
