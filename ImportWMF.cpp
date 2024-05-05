// ImportWMF.cpp: implementation of the CImportWMF class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ImportWMF.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 打开为WMF的实现文件
// 创建日期	: 2001/08/08
// 作者		: yaka
// 备注 		: 
#include "stdafx.h"
#include "JetImgSee.h"
#include "ImportWMF.h"
#include "mcswmf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int iReturn;
extern int g_ImageCount;
extern BYTE g_ImageFormat;
CImportWMF::CImportWMF()
{

}

CImportWMF::~CImportWMF()
{

}
// 功能描述	: 打开WMF文件
// 参数		: LPSTR lpszSrcFName
// 返回值	: HDIB 图像句柄
// 创建日期	: 2000/08/08
// 作者		: yaka
// 备注 	: 
HDIB CImportWMF::iImportWMF(LPSTR lpszSrcFName)
{
	HENHMETAFILE hEnhMeta;
	ENHMETAHEADER emfHeader;
	BYTE *pby;
	HBITMAP	bmp, oldbmp;
	HDC hComDC;
	HDC	hDC = ::GetDC(NULL);
	LPBITMAPINFOHEADER lpBIH;
	DWORD dwIsAldus;
	ALDUSMFHEADER aldusMFHeader;
	CFile*  m_SrcFile = new CFile();
	if(!m_SrcFile->Open(lpszSrcFName,CFile::modeRead))
	{
		iReturn = READSOURCEIMG_ERR;
		return NULL;
	}

	// 读取文件头的四个字节。（如果是可放置元文件，该DWORD的内容应该是0x9AC6CDD7
	m_SrcFile->Read(&dwIsAldus, 4);
	m_SrcFile->Seek(0, CFile::begin);
	BOOL bIsWMF;
	long lWidth,lHeight;
	switch (dwIsAldus)
	{
	case ALDUSKEY:
		{
			// 表明该文件是一个可放置元文件，读文件头部的结构。
			m_SrcFile->Read(&aldusMFHeader, 22);
			float fx, fy;
			fx = (float)(aldusMFHeader.right-aldusMFHeader.left);
			fy = (float)(aldusMFHeader.bottom-aldusMFHeader.top);
			fx = (fx/(float)aldusMFHeader.inch)*::GetDeviceCaps(hDC, LOGPIXELSX)+(float)0.5; 
			fy = (fy/(float)aldusMFHeader.inch)*::GetDeviceCaps(hDC, LOGPIXELSY)+(float)0.5;	
			lWidth = long(fx);
			lHeight = long(fy);
			bIsWMF = TRUE;
			break;
		}
	case EMR_HEADER:
		{
			m_SrcFile->Read(&emfHeader, sizeof(ENHMETAHEADER));
			lWidth = emfHeader.rclBounds.right - emfHeader.rclBounds.left;
			lHeight = emfHeader.rclBounds.bottom - emfHeader.rclBounds.top;
			bIsWMF = FALSE;
			break;
		}
	case OLDWMF:
		{
			lWidth   = ::GetDeviceCaps(hDC, HORZRES);
			lHeight   = ::GetDeviceCaps(hDC, VERTRES);
			bIsWMF = TRUE;
			break;
		}
	default:
		{
			iReturn = NOWMF;
			m_SrcFile->Close();
			delete m_SrcFile;
			return NULL;
		}
	}

	HANDLE hBI_Struct = GlobalAlloc(GHND,sizeof(BITMAPINFOHEADER));
	lpBIH = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);
	DWORD wWidthBytes      = DWORD_WBYTES(lWidth * 24);
	lpBIH->biSize          = (DWORD)sizeof(BITMAPINFOHEADER);
	lpBIH->biWidth		   = lWidth;
	lpBIH->biHeight		   = lHeight;
    lpBIH->biPlanes        = 1;
    lpBIH->biBitCount      = 24;
    lpBIH->biCompression   = BI_RGB;
    lpBIH->biXPelsPerMeter = 0;
    lpBIH->biYPelsPerMeter = 0;
    lpBIH->biClrUsed       = 0;
    lpBIH->biClrImportant  = 0;
    lpBIH->biSizeImage     = (DWORD)wWidthBytes * lpBIH->biHeight;
	g_ImageCount = 1;
	g_ImageFormat = WMF;

	if (bIsWMF)
	{
		iReturn = iCreateMetaWMF(m_SrcFile,&hEnhMeta);
		m_SrcFile->Close();
		delete m_SrcFile;
	}
	else
	{
		m_SrcFile->Close();
		delete m_SrcFile;
		hEnhMeta = GetEnhMetaFile(lpszSrcFName);
		if (hEnhMeta == NULL)
			iReturn = CREATEWMF_FAILED;
	}
	if (iReturn)
	{
		GlobalUnlock(hBI_Struct);
		GlobalFree(hBI_Struct);
		return NULL;
	}

	// 创建用于存放图像位数据的临时缓冲区
	bmp = ::CreateDIBSection(hDC, (CONST BITMAPINFO*)lpBIH, 
		DIB_RGB_COLORS, (VOID**)&pby, NULL, 0);
	hComDC = ::CreateCompatibleDC(hDC);
	::ReleaseDC(NULL, hDC);
	if ((bmp == NULL)||(hComDC == NULL))	// 错误检查
	{
		::DeleteEnhMetaFile(hEnhMeta);
		if (bmp) ::DeleteObject(bmp);
		if (hComDC) ::DeleteDC(hComDC);
		GlobalUnlock(hBI_Struct);
		GlobalFree(hBI_Struct);
		iReturn = CREATEWMF_FAILED;
		return NULL;
	}

	oldbmp = (HBITMAP)::SelectObject(hComDC, bmp);
	// 设置背景色为缺省窗口色
	::FillRect(hComDC, &CRect(0,0,lpBIH->biWidth, lpBIH->biHeight), (HBRUSH)(COLOR_WINDOW+1));
	// 绘制WMF图像（由Windows元文件格式转为的EMF格式）
	::PlayEnhMetaFile(hComDC, hEnhMeta, CRect(0,0,lpBIH->biWidth, lpBIH->biHeight));
	::SelectObject(hComDC, oldbmp);
	::DeleteDC(hComDC);

	HDIB hOutWMF;
	LPBYTE pOutWMF;
	hOutWMF = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBIH->biSizeImage + lpBIH->biSize);
	pOutWMF = (LPBYTE) ::GlobalLock((HGLOBAL) hOutWMF);	
	if (pOutWMF == NULL)
	{
		AfxMessageBox("申请空间错误!");
		GlobalUnlock(hBI_Struct);
		GlobalFree(hBI_Struct);
		::DeleteObject(bmp);	
		::DeleteEnhMetaFile(hEnhMeta);
		return NULL;
	}
	memcpy(pOutWMF,lpBIH,lpBIH->biSize); 
	pOutWMF += sizeof(BITMAPINFOHEADER);

    memcpy(pOutWMF,pby,lpBIH->biSizeImage);

	GlobalUnlock(hBI_Struct);
    GlobalFree(hBI_Struct);
	GlobalUnlock(hOutWMF);

	::DeleteObject(bmp);	
	::DeleteEnhMetaFile(hEnhMeta);
	return hOutWMF;

}
// 功能描述	: 创建WMF句柄
// 参数		: CFile *m_SrcFile,	HENHMETAFILE *hEnhMeta
//				源文件            WMF图像句柄
// 返回值	: int 错误值
// 创建日期	: 2000/08/08
// 作者		: yaka
// 备注 	: 
int CImportWMF::iCreateMetaWMF(CFile *m_SrcFile, HENHMETAFILE *hEnhMeta)
{
	METAHEADER		mfHeader;
  	DWORD    		dwSize;
	*hEnhMeta = NULL;
	
	// 读Windows元文件头.
	long seekpos = m_SrcFile->Seek(0, CFile::current);
	m_SrcFile->Read(&mfHeader, 18);
	
	// 为元文件数据分配内存.(mtSize的尺寸值单位是WORD型，所以乘2）
	dwSize = mfHeader.mtSize*2;

	BYTE* lpMFBits = (BYTE*)::GlobalAlloc(GPTR, dwSize);
	if (lpMFBits == NULL)
	{
		iReturn = CREATEWMF_FAILED;	
		return iReturn;
	}

	// 定位元文件位数据偏移.
	m_SrcFile->Seek(seekpos, CFile::begin);
	
	if (m_SrcFile->Read(lpMFBits, dwSize) != dwSize)
	{
		GlobalFree(lpMFBits);
		iReturn = READSOURCEIMG_ERR;
		return iReturn;
	}
	

	// 获得元文件句柄
	*hEnhMeta = ::SetWinMetaFileBits(dwSize, lpMFBits, NULL, NULL);

	::GlobalFree(lpMFBits);

	if (!(*hEnhMeta))
	{
		iReturn = NOWMF;
		return iReturn;
	}
	else
		return 0;
}
