// ImportICO.cpp: implementation of the CImportICO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JetImgSee.h"
#include "ImportICO.h"
#include "mcsico.h"

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
CImportICO::CImportICO()
{

}

CImportICO::~CImportICO()
{

}
// ��������	: ��ICO,CUR�ļ�
// ����		: LPSTR lpszSrcFName
// ����ֵ	: HDIB ͼ����
// ��������	: 2000/08/13
// ����		: yaka
// ��ע 	: 
HDIB CImportICO::iImportICO(LPSTR lpszSrcFName)
{
	BYTE *pby;
	HBITMAP	bmp, oldbmp;
	HDC hComDC;
	HDC	hDC = ::GetDC(NULL);
	LPBITMAPINFOHEADER lpBIH;
	ICONDIR icondir;
	CFile*  m_SrcFile = new CFile();
	if(!m_SrcFile->Open(lpszSrcFName,CFile::modeRead))
	{
		iReturn = READSOURCEIMG_ERR;
		return NULL;
	}
	m_SrcFile->Read((LPSTR)&icondir, 6);
	m_SrcFile->Read((LPSTR)icondir.idEntries,16);
	m_SrcFile->Close();
	delete m_SrcFile;

	HANDLE hBI_Struct = GlobalAlloc(GHND,sizeof(BITMAPINFOHEADER));
	lpBIH = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);

	lpBIH->biSize          = (DWORD)sizeof(BITMAPINFOHEADER);
	lpBIH->biWidth		   = icondir.idEntries->bWidth;
	lpBIH->biHeight		   = icondir.idEntries->bHeight;
    lpBIH->biPlanes        = 1;
	lpBIH->biBitCount	   = 24;
    lpBIH->biCompression   = BI_RGB;
    lpBIH->biXPelsPerMeter = 0;
    lpBIH->biYPelsPerMeter = 0;
    lpBIH->biClrUsed       = 0;
    lpBIH->biClrImportant  = 0;
	DWORD wWidthBytes      = DWORD_WBYTES(lpBIH->biWidth * lpBIH->biBitCount);
    lpBIH->biSizeImage     = (DWORD)wWidthBytes * lpBIH->biHeight;
	g_ImageCount = 1;
	g_ImageFormat = ICO;
	HICON hicon = (HICON)::LoadImage(NULL,lpszSrcFName,IMAGE_ICON,lpBIH->biWidth,lpBIH->biHeight,LR_LOADFROMFILE);

	if (hicon == NULL)
	{
		iReturn = CREATEICO_FAILED;
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
		DestroyIcon(hicon);
		if (bmp) ::DeleteObject(bmp);
		if (hComDC) ::DeleteDC(hComDC);
		GlobalUnlock(hBI_Struct);
		GlobalFree(hBI_Struct);
		iReturn = CREATEWMF_FAILED;
		return NULL;
	}

	oldbmp = (HBITMAP)::SelectObject(hComDC, bmp);
	::FillRect(hComDC, &CRect(0,0,lpBIH->biWidth, lpBIH->biHeight), (HBRUSH)(COLOR_WINDOW+1));
	::DrawIcon(hComDC,0,0,hicon);
	::SelectObject(hComDC, oldbmp);
	::DeleteDC(hComDC);
	DestroyIcon(hicon);

	HDIB hOutICO;
	LPBYTE pOutICO;
	hOutICO = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBIH->biSizeImage + lpBIH->biSize);
	pOutICO = (LPBYTE) ::GlobalLock((HGLOBAL) hOutICO);	
	if (pOutICO == NULL)
	{
		AfxMessageBox("����ռ����!");
		iReturn = GLOBALLOCKERROR;
		GlobalUnlock(hBI_Struct);
		GlobalFree(hBI_Struct);
		::DeleteObject(bmp);	
		return NULL;
	}
	memcpy(pOutICO,lpBIH,lpBIH->biSize); 
	pOutICO += sizeof(BITMAPINFOHEADER);

    memcpy(pOutICO,pby,lpBIH->biSizeImage);

	GlobalUnlock(hBI_Struct);
    GlobalFree(hBI_Struct);
	GlobalUnlock(hOutICO);

	::DeleteObject(bmp);	
	return hOutICO;

}
