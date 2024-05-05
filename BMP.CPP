// BMP.cpp: implementation of the CBMP class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: BMP.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理BMP的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "BMP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern int g_ImageCount;
extern BYTE g_ImageFormat;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMP::CBMP()
{

}

CBMP::~CBMP()
{

}
// 功能描述	: 计算调色板尺寸
// 参数		: LPSTR lpbi
// 返回值	: WORD 调色板尺寸
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
WORD CBMP::PaletteSize(LPSTR lpbi)
{
		DWORD DIBNumColors;
		DIBNumColors = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		WORD Bits = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
		if (DIBNumColors == 0 && Bits <= 8)
			DIBNumColors = 0x1L << Bits;
	  return (WORD)(DIBNumColors * sizeof(RGBQUAD));
}
// 功能描述	: 保存bmp文件
// 参数		: HDIB hDib, CFile *p_DstFName
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
int CBMP::iExportBMP(HDIB hDib, CFile *p_DstFName)
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	DWORD dwDIBSize;

	if (hDib == NULL)
		return FALSE;

	/*
	 * Get a pointer to the DIB memory, the first of which contains
	 * a BITMAPINFO structure
	 */
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	if (lpBI == NULL)
		return FALSE;

	if (!IS_WIN30_DIB(lpBI))
	{
		::GlobalUnlock((HGLOBAL) hDib);
		return FALSE;       // It's an other-style DIB (save not supported)
	}

	/*
	 * Fill in the fields of the file header
	 */

	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHdr.bfType = 'MB';//DIB_HEADER_MARKER;  // "BM"

	dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = DWORD_WBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).

		lpBI->biSizeImage = dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	/*
	 * Now, calculate the offset the actual bitmap bits will be in
	 * the file -- It's the Bitmap file header plus the DIB header,
	 * plus the size of the color table.
	 */
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);

	TRY
	{
		// Write the file header
		p_DstFName->Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		//
		// Write the DIB header and the bits
		//
		p_DstFName->Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		::GlobalUnlock((HGLOBAL) hDib);
		THROW_LAST();
	}
	END_CATCH


	::GlobalUnlock((HGLOBAL) hDib);
	return TRUE;
}
// 功能描述	: 打开bmp文件
// 参数		: CFile *p_SrcFName
// 返回值	: HDIB 图像句柄
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 
HDIB CBMP::iImportBMP(CFile *p_SrcFName)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	/*
	 * get length of DIB in bytes for use when reading
	 */

	dwBitsSize = p_SrcFName->GetLength();

	/*
	 * Go read the DIB file header and check if it's valid.
	 */
	if (p_SrcFName->Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

	if (bmfHeader.bfType != BMP_MARK)
		return NULL;

	/*
	 * Allocate memory for DIB
	 */
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/*
	 * Go read the bits.
	 */
	if (p_SrcFName->Read(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		return NULL;
	}
	g_ImageCount = 1;
	g_ImageFormat = BMP;
	::GlobalUnlock((HGLOBAL) hDIB);
	return hDIB;

}

// 功能描述	: 是否彩色
// 参数		: LPSTR lpbi
// 返回值	: TRUE 是彩色,FALSE 不是
// 创建日期	: 2001/08/03
// 作者		: yaka
// 备注 	: 
BOOL CBMP::IsColor(LPSTR lpbi)
{
	return (((LPBITMAPINFOHEADER)lpbi)->biBitCount > 1);
}

// 功能描述	: 获取图像DPI
// 参数		: LPSTR lpbi
// 返回值	: CSize DPI大小
// 创建日期	: 2001/08/03
// 作者		: yaka
// 备注 	: 
CSize CBMP::GetDPI(LPSTR lpbi)
{
	CSize DpiSize;
	DpiSize.cx = long(((LPBITMAPINFOHEADER)lpbi)->biXPelsPerMeter * 25.4 / 1000 + 0.5);
	DpiSize.cy = long(((LPBITMAPINFOHEADER)lpbi)->biYPelsPerMeter * 25.4 / 1000 + 0.5);
	return DpiSize;
}
// 功能描述	: 获取图像大小
// 参数		: LPSTR lpbi
// 返回值	: CSize 图像大小
// 创建日期	: 2001/08/03
// 作者		: yaka
// 备注 	: 
CSize CBMP::GetSize(LPSTR lpbi)
{
	CSize	ImgSize;
	ImgSize.cx = ((LPBITMAPINFOHEADER)lpbi)->biWidth;
	ImgSize.cy = abs(((LPBITMAPINFOHEADER)lpbi)->biHeight);
	return ImgSize;
}
