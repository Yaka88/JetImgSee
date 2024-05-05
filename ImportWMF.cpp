// ImportWMF.cpp: implementation of the CImportWMF class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ImportWMF.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��ΪWMF��ʵ���ļ�
// ��������	: 2001/08/08
// ����		: yaka
// ��ע 		: 
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
// ��������	: ��WMF�ļ�
// ����		: LPSTR lpszSrcFName
// ����ֵ	: HDIB ͼ����
// ��������	: 2000/08/08
// ����		: yaka
// ��ע 	: 
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

	// ��ȡ�ļ�ͷ���ĸ��ֽڡ�������ǿɷ���Ԫ�ļ�����DWORD������Ӧ����0x9AC6CDD7
	m_SrcFile->Read(&dwIsAldus, 4);
	m_SrcFile->Seek(0, CFile::begin);
	BOOL bIsWMF;
	long lWidth,lHeight;
	switch (dwIsAldus)
	{
	case ALDUSKEY:
		{
			// �������ļ���һ���ɷ���Ԫ�ļ������ļ�ͷ���Ľṹ��
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

	// �������ڴ��ͼ��λ���ݵ���ʱ������
	bmp = ::CreateDIBSection(hDC, (CONST BITMAPINFO*)lpBIH, 
		DIB_RGB_COLORS, (VOID**)&pby, NULL, 0);
	hComDC = ::CreateCompatibleDC(hDC);
	::ReleaseDC(NULL, hDC);
	if ((bmp == NULL)||(hComDC == NULL))	// ������
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
	// ���ñ���ɫΪȱʡ����ɫ
	::FillRect(hComDC, &CRect(0,0,lpBIH->biWidth, lpBIH->biHeight), (HBRUSH)(COLOR_WINDOW+1));
	// ����WMFͼ����WindowsԪ�ļ���ʽתΪ��EMF��ʽ��
	::PlayEnhMetaFile(hComDC, hEnhMeta, CRect(0,0,lpBIH->biWidth, lpBIH->biHeight));
	::SelectObject(hComDC, oldbmp);
	::DeleteDC(hComDC);

	HDIB hOutWMF;
	LPBYTE pOutWMF;
	hOutWMF = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBIH->biSizeImage + lpBIH->biSize);
	pOutWMF = (LPBYTE) ::GlobalLock((HGLOBAL) hOutWMF);	
	if (pOutWMF == NULL)
	{
		AfxMessageBox("����ռ����!");
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
// ��������	: ����WMF���
// ����		: CFile *m_SrcFile,	HENHMETAFILE *hEnhMeta
//				Դ�ļ�            WMFͼ����
// ����ֵ	: int ����ֵ
// ��������	: 2000/08/08
// ����		: yaka
// ��ע 	: 
int CImportWMF::iCreateMetaWMF(CFile *m_SrcFile, HENHMETAFILE *hEnhMeta)
{
	METAHEADER		mfHeader;
  	DWORD    		dwSize;
	*hEnhMeta = NULL;
	
	// ��WindowsԪ�ļ�ͷ.
	long seekpos = m_SrcFile->Seek(0, CFile::current);
	m_SrcFile->Read(&mfHeader, 18);
	
	// ΪԪ�ļ����ݷ����ڴ�.(mtSize�ĳߴ�ֵ��λ��WORD�ͣ����Գ�2��
	dwSize = mfHeader.mtSize*2;

	BYTE* lpMFBits = (BYTE*)::GlobalAlloc(GPTR, dwSize);
	if (lpMFBits == NULL)
	{
		iReturn = CREATEWMF_FAILED;	
		return iReturn;
	}

	// ��λԪ�ļ�λ����ƫ��.
	m_SrcFile->Seek(seekpos, CFile::begin);
	
	if (m_SrcFile->Read(lpMFBits, dwSize) != dwSize)
	{
		GlobalFree(lpMFBits);
		iReturn = READSOURCEIMG_ERR;
		return iReturn;
	}
	

	// ���Ԫ�ļ����
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
