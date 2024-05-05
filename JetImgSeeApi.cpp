// �ļ���	: JetimgSeeApi.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: API������ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "StdAfx.h"
#include "ExportTiff.h"
#include "ExportJPG.h"
#include "ExportTGA.h"
#include "ExportPCX.h"
#include "ExportGIF.h"
#include "ImportTiff.h"
#include "ImportJPG.h"
#include "ImportTGA.h"
#include "ImportGIF.h"
#include "ImportPCX.h"	
#include "ImportWMF.h"
#include "ImportICO.h"
#include "BMP.h"
#include <math.h>
#include <IO.h>

int g_ImageCount = 1;
int iReturn = 0;
BYTE g_ImageFormat = 0;
// ��������	: ����0
// ����		: HDIB pa_hDib
// ����ֵ	: 
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
void vBlackIsZero(HDIB pa_hDib)
{
	LPBYTE pInBmp = (LPBYTE)::GlobalLock(pa_hDib);
	if (pInBmp == NULL)
		return;
	LPBITMAPINFOHEADER lpBIH = (LPBITMAPINFOHEADER)pInBmp;
	pInBmp += sizeof(BITMAPINFOHEADER);
	if (lpBIH->biBitCount == 1) {
		WORD wPaletteSize = 2 * sizeof(RGBQUAD);
		RGBQUAD* TmpPal = (RGBQUAD*)pInBmp;
		if((TmpPal[0].rgbRed==255)  && (TmpPal[1].rgbRed==0))
		{
			TmpPal[0].rgbRed = TmpPal[0].rgbGreen = TmpPal[0].rgbBlue = (BYTE)0;
			TmpPal[1].rgbRed = TmpPal[1].rgbGreen = TmpPal[1].rgbBlue = (BYTE)255;
			pInBmp += wPaletteSize;
			
			DWORD dwWidthBytes = DWORD_WBYTES(lpBIH->biWidth*lpBIH->biBitCount);
			DWORD dwBytesLeft = dwWidthBytes*lpBIH->biHeight;
			long lwi,lwj;
			BYTE byTemp=0x80;
			for(lwi=0; lwi<lpBIH->biHeight; lwi++){
				for(lwj=0; lwj<(lpBIH->biWidth/8); lwj++)//ǰ����󲿷ְ��ֽ��󷴡�
					pInBmp[lwj] ^= 0xFF;
				for(lwj=(lpBIH->biWidth/8)*8; lwj<lpBIH->biWidth; lwj++){//��󼸸�bitλ�󷴡����λ�����ǡ�
					byTemp = 0x80 >> (lwj % 8);
					pInBmp[lwj/8] ^= byTemp;
				}
				pInBmp += dwWidthBytes;
			}
		}
	}
	::GlobalUnlock(pa_hDib);
}
// ��������	: ����HDIBΪBMP
// ����		: HDIB hDIB,ͼ���� LPCTSTR p_lpszFileName�ļ���
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveBmpFile(HDIB hDIB, LPCTSTR p_lpszFileName)
{
	CFile *m_File = new CFile();
	CFileException m_error;
	if (hDIB == NULL)   //add by yaka 20010601
		return FALSE;
	if(!m_File->Open(p_lpszFileName,CFile::modeCreate | CFile::modeWrite,&m_error))
	{
		m_error.ReportError();
		return FALSE;
	}	
	CBMP bmpfile;
	if(bmpfile.iExportBMP(hDIB, m_File)==FALSE)
	{
		m_File->Close();
		return FALSE;
	}
	m_File->Close();
	return TRUE;
}

// ��������	: ��bmp�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB WINAPI OpenBmpFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	CFile *m_File = new CFile();
	CFileException m_error;
	if(!m_File->Open(p_lpszFileName,CFile::modeRead,&m_error))
	{
		m_error.ReportError();
		return FALSE;
	}
	CBMP bmpfile;
	HDIB hDib = bmpfile.iImportBMP(m_File);
	m_File->Close();

	/*����Ϊ��������*/
	vBlackIsZero(hDib);

	if(p_lpszDesFileName != NULL)
		SaveFileFromDib(hDib, p_lpszDesFileName);
	return hDib;
}

// ��������	: ��tiff�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB WINAPI OpenTiffFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	if(p_lpszFileName==NULL)
		return NULL;
	CImportTIF m_TiffDecode;
	HDIB m_hDIB;
	m_hDIB = m_TiffDecode.iImportTIF((char*)p_lpszFileName);

	if(m_hDIB == NULL)
		return NULL;
	/*����Ϊ��������*/
	vBlackIsZero(m_hDIB);

	if(p_lpszDesFileName != NULL)
		SaveFileFromDib(m_hDIB, p_lpszDesFileName);
	return m_hDIB;
}
// ��������	: ����HDIBΪtiff
// ����		: HDIB hDIB,ͼ���� LPCTSTR p_lpszFileName�ļ���,int p_nFormat����ѹ����ʽ
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveTiffFromDib(HDIB hDIB, LPCTSTR p_lpszFileName,int p_nFormat)
{
	if (hDIB == NULL)   //add by yaka 20010601
		return FALSE;
	//modify by yaka 20010524
	if (p_nFormat == 6)
	{
		CExportJPG m_ExportJpg;
		if(m_ExportJpg.iExportJPGEncode(hDIB,(char *)p_lpszFileName,50,8))
			return FALSE;
	}

	CExportTIF m_TiffEncode;
	if(m_TiffEncode.iExportTIF(hDIB,p_lpszFileName,p_nFormat))
		return FALSE;
	return TRUE;
	//modify end 
}
// ��������	: ����BMPΪTIFF
// ����		: LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName,int p_nFormat����ѹ����ʽ
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveTiffFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName,int p_nFormat)
{
	//modify by yaka 20010601
	HDIB hdib = OpenBmpFile(p_lpszSrcName,NULL);
	if (hdib == NULL)
	{
		AfxMessageBox("���ļ�����BMP�ļ�!");
		return FALSE;
	}
	//modify end

	if(SaveTiffFromDib(hdib,p_lpszDesName,p_nFormat) == FALSE)
	{
		::GlobalFree(hdib);
		return FALSE;
	}
	::GlobalFree(hdib);
	return TRUE;
}

// ��������	: ��jpeg�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB WINAPI OpenJpegFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	if(p_lpszFileName==NULL)
		return NULL;
	CImportJPG m_ImportJpg;
	HDIB m_hDIB;
	m_hDIB = m_ImportJpg.iTmportJPGDecode((char*)p_lpszFileName,0);
	if(m_hDIB == NULL)
		return NULL;

	if(p_lpszDesFileName != NULL)
		SaveFileFromDib(m_hDIB, p_lpszDesFileName);
	return m_hDIB;
}
// ��������	: ��pcx�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
//add by yaka 20010530
HDIB WINAPI OpenPcxFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	if(p_lpszFileName==NULL)
		return NULL;
	CImportPCX m_ImportPcx;
	HDIB m_hDIB;
	m_hDIB = m_ImportPcx.iImportPCX((char*)p_lpszFileName);
	if(m_hDIB == NULL)
		return NULL;

	if(p_lpszDesFileName != NULL)
		SaveFileFromDib(m_hDIB, p_lpszDesFileName);
	return m_hDIB;
}
//add end
// ��������	: ����HDIBΪjpeg
// ����		: HDIB hDIB,ͼ���� LPCTSTR p_lpszFileName�ļ���,int p_nCompFactoѹ����
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveJpegFromDib(HDIB hDIB,LPCTSTR p_lpszFileName,int p_nCompFactor)
{
	if (hDIB == NULL)   //add by yaka 20010601
		return FALSE;
	CExportJPG m_ExportJpg;
	if(m_ExportJpg.iExportJPGEncode(hDIB,(char*)p_lpszFileName,p_nCompFactor,0))
		return FALSE;
	return TRUE;
}

//add by yaka 20010531
// ��������	: ����HDIBΪpcx
// ����		: HDIB hDIB,ͼ���� LPCTSTR p_lpszFileName�ļ���
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SavePcxFromDib(HDIB hDIB,LPCTSTR p_lpszFileName)
{
	if (hDIB == NULL)   //add by yaka 20010601
		return FALSE;
	CExportPCX m_ExportPcx;
	if(m_ExportPcx.iExportPCX(hDIB,(char*)p_lpszFileName))
		return FALSE;
	return TRUE;
}
// ��������	: ����bmpΪpcx
// ����		: LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesNam
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SavePcxFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName)
{
	//modify by yaka 20010601
	HDIB hdib = OpenBmpFile(p_lpszSrcName,NULL);
	if (hdib == NULL)
	{
		AfxMessageBox("���ļ�����BMP�ļ�!");
		return FALSE;
	}
	//modify end

	if(SavePcxFromDib(hdib,p_lpszDesName) == FALSE)
	{
		::GlobalFree(hdib);
		return FALSE;
	}
	::GlobalFree(hdib);
	return TRUE;
}
//add end
// ��������	: ����bmpΪjpeg
// ����		: LPCTSTR p_lpszSrcName,LPCTSTR p_lpszDesName,int p_nCompFactoѹ����
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveJpegFromBmp(LPCTSTR p_lpszSrcName,LPCTSTR p_lpszDesName,int p_nCompFactor)
{
	//modify by yaka 20010601
	HDIB hInBmp = OpenBmpFile(p_lpszSrcName,NULL);
	if (hInBmp == NULL)
	{
		AfxMessageBox("���ļ�����BMP�ļ�!");
		return FALSE;
	}
	//modify end

	if(SaveJpegFromDib(hInBmp,p_lpszDesName,p_nCompFactor)==FALSE)
	{
		::GlobalFree(hInBmp);
		return FALSE;
	}
	::GlobalFree(hInBmp);
	return TRUE;
}
// ��������	: ����bmpΪtga
// ����		: LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName,BOOL p_bRLE�Ƿ�RLEѹ��
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
//add by yaka 20010601
BOOL WINAPI SaveTgaFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName,BOOL p_bRLE)
{
	HDIB hdib = OpenBmpFile(p_lpszSrcName,NULL);
	if (hdib == NULL)
	{
		AfxMessageBox("���ļ�����BMP�ļ�!");
		return FALSE;
	}
	if(SaveTgaFromDib(hdib,p_lpszDesName,p_bRLE) == FALSE)
	{
		::GlobalFree(hdib);
		return FALSE;
	}
	::GlobalFree(hdib);
	return TRUE;
}
// ��������	: ����HDIBΪtga
// ����		: HDIB hDIB,ͼ���� LPCTSTR p_lpszFileName�ļ���,BOOL p_bRLE�Ƿ�RLEѹ��
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
//add by yaka 20010608
BOOL WINAPI SaveTgaFromDib(HDIB hDIB,LPCTSTR p_lpszFileName,BOOL p_bRLE)
{
	if (hDIB == NULL)   
		return FALSE;
	CExportTGA m_ExportTga;
	if(m_ExportTga.iExportTGA(hDIB,(char*)p_lpszFileName,p_bRLE))
		return FALSE;
	return TRUE;
}
// ��������	: ��tga�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
//add by yaka 20010607
HDIB WINAPI OpenTgaFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	if(p_lpszFileName==NULL)
		return NULL;
	CImportTGA m_ImportTga;
	HDIB m_hDIB;
	m_hDIB = m_ImportTga.iImportTGA((char*)p_lpszFileName);
	if(m_hDIB == NULL)
		return NULL;

	if(p_lpszDesFileName != NULL)
		SaveFileFromDib(m_hDIB, p_lpszDesFileName);
	return m_hDIB;
}
//add end
// ��������	: ����bmpΪgif
// ����		: LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesNam
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveGifFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName)
{
	HDIB hdib = OpenBmpFile(p_lpszSrcName,NULL);
	if (hdib == NULL)
	{
		AfxMessageBox("���ļ�����BMP�ļ�!");
		return FALSE;
	}
	if(SaveGifFromDib(hdib,p_lpszDesName) == FALSE)
	{
		::GlobalFree(hdib);
		return FALSE;
	}
	::GlobalFree(hdib);
	return TRUE;
}
//add by yaka 20010619
// ��������	: ����HDIBΪgif
// ����		: HDIB hDIB,ͼ���� LPCTSTR p_lpszFileName�ļ���
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveGifFromDib(HDIB hDIB,LPCTSTR p_lpszFileName)
{
	if (hDIB == NULL)   
		return FALSE;
	CExportGIF m_ExportGif;
	if(m_ExportGif.iExportGIF(hDIB,(char*)p_lpszFileName))
		return FALSE;
	return TRUE;
}
//add end

// ��������	: ��WMF�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/08/08
// ����		: yaka
// ��ע 	: 
HDIB WINAPI OpenWmfFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	if(p_lpszFileName==NULL)
		return NULL;
	CImportWMF m_ImportWmf;
	HDIB m_hDIB;
	m_hDIB = m_ImportWmf.iImportWMF((char*)p_lpszFileName);

	if(m_hDIB == NULL)
		return NULL;

	if(p_lpszDesFileName != NULL)
		SaveFileFromDib(m_hDIB, p_lpszDesFileName);
	return m_hDIB;
}
// ��������	: ��ICO,CUR�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/08/13
// ����		: yaka
// ��ע 	: 
HDIB WINAPI OpenIcoFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	if(p_lpszFileName==NULL)
		return NULL;
	CImportICO m_ImportIco;
	HDIB m_hDIB;
	m_hDIB = m_ImportIco.iImportICO((char*)p_lpszFileName);

	if(m_hDIB == NULL)
		return NULL;

	if(p_lpszDesFileName != NULL)
		SaveFileFromDib(m_hDIB, p_lpszDesFileName);
	return m_hDIB;
}
// ��������	: ��gif�ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
//add by yaka 20010618
HDIB WINAPI OpenGifFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName)
{
	if(p_lpszFileName==NULL)
		return NULL;
	CImportGIF m_ImportGif;
	HDIB m_hDIB;
	m_hDIB = m_ImportGif.iImportGIF((char*)p_lpszFileName);
	if(m_hDIB == NULL)
		return NULL;

	if(p_lpszDesFileName != NULL)
		SaveGifFromDib(m_hDIB, p_lpszDesFileName);
	return m_hDIB;
}

//add end

//add by yaka 20010601
// ��������	: ����BMPΪ������ʽ
// ����		: LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveFileFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName)
{
	HDIB hInBmp = OpenBmpFile(p_lpszSrcName,NULL);
	if (hInBmp == NULL)
	{
		AfxMessageBox("���ļ�����BMP�ļ�!");
		return FALSE;
	}

	if(SaveFileFromDib(hInBmp,p_lpszDesName)==FALSE)
	{
		::GlobalFree(hInBmp);
		return FALSE;
	}
	::GlobalFree(hInBmp);
	return TRUE;

}


// ��������	: ����HDIBΪ����ͼ���ʽ
// ����		: HDIB hDIB,ͼ���� LPCTSTR p_lpszFileName�ļ���
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
BOOL WINAPI SaveFileFromDib(HDIB hDIB,LPCTSTR p_lpszFileName)
{
	if (hDIB == NULL || p_lpszFileName == NULL)
		return FALSE;
	CString strFilename(p_lpszFileName);
	CString strRight;
	int idotpos = strFilename.ReverseFind('.');
	if (idotpos == -1)
		strRight = "BMP";
	else
		strRight = strFilename.Mid(idotpos + 1);
	if (!strRight.CompareNoCase("DIB") || !strRight.CompareNoCase("BMP"))
		return SaveBmpFile(hDIB,p_lpszFileName);
	else if (!strRight.CompareNoCase("TIF") || !strRight.CompareNoCase("TIFF"))
		return SaveTiffFromDib(hDIB,p_lpszFileName,0);
	else if (!strRight.CompareNoCase("JPEG") || !strRight.CompareNoCase("JPG"))
		return SaveJpegFromDib(hDIB,p_lpszFileName,50);
	else if (!strRight.CompareNoCase("PCX") || !strRight.CompareNoCase("DCX"))
		return SavePcxFromDib(hDIB,p_lpszFileName);
	else if (!strRight.CompareNoCase("TGA"))
		return SaveTgaFromDib(hDIB,p_lpszFileName,TRUE);
	else if (!strRight.CompareNoCase("GIF"))
		return SaveGifFromDib(hDIB,p_lpszFileName);
	else
		return SaveBmpFile(hDIB,p_lpszFileName);

}
//add end
// ��������	: ��ͼ���ļ�
// ����		: LPCTSTR p_lpszFileName,ԭ�ļ�LPCTSTR p_lpszDesFileName������ļ�
// ����ֵ	: HDIB�򿪵�ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB WINAPI OpenImgFile(LPCTSTR p_lpszSrcName,LPCTSTR p_lpszDesName)
{
	CFile m_SrcFile;
	CFileException m_error;
	if(!m_SrcFile.Open(p_lpszSrcName,CFile::modeRead ,&m_error))
	{
		m_error.ReportError();
		iReturn = OPENFILE_ERR;
		return NULL;
	}
    DWORD dwMarker;
	m_SrcFile.Read((LPSTR)&dwMarker,4);
	m_SrcFile.Close();
	
	if (dwMarker == CUR_MARK || dwMarker == ICO_MARK)
		return(OpenIcoFile(p_lpszSrcName,p_lpszDesName));

	WORD wMarker = (WORD)dwMarker;
	switch(wMarker)
	{
	case JPG_SOI_MARK:
		return(OpenJpegFile(p_lpszSrcName,p_lpszDesName));
		break;
	case GIF_MARK:
		return(OpenGifFile(p_lpszSrcName,p_lpszDesName));
		break;
	case TIF_INTEL:
	case TIF_MOTOROLA:	
		return(OpenTiffFile(p_lpszSrcName,p_lpszDesName));
		break;
	case BMP_MARK:	
		return(OpenBmpFile(p_lpszSrcName,p_lpszDesName));
		break;
	case WMF_MARK:
	case EMF_MARK:
		return(OpenWmfFile(p_lpszSrcName,p_lpszDesName));
		break;
	default:
		{//add by yaka 20010530
			if ((BYTE)wMarker == PCX_MARK)
				return(OpenPcxFile(p_lpszSrcName,p_lpszDesName));
			else if ((BYTE)wMarker == TGA_MARK_0 || !CString(p_lpszSrcName).Right(4).CompareNoCase(".TGA")) //add by yaka 20010608
				return(OpenTgaFile(p_lpszSrcName,p_lpszDesName));
			else
			{
				AfxMessageBox("�ݲ�֧������ͼ���ļ���ʽ!");
				return NULL;
			}
		}
	}
}
// ��������	: ��HDIB��ץȡָ����
// ����		: HDIB hDIB,RECT rect
// ����ֵ	: HDIB����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB WINAPI hGetImageBlock(HDIB hDIB,RECT rect)
{
	if (rect.right <= rect.left || rect.bottom <= rect.top )
		return NULL;
	LPBITMAPINFO lpbi = (LPBITMAPINFO)::GlobalLock(hDIB);
	if (lpbi == NULL)
		return NULL;
	CBMP bmp;
	DWORD wBiSize = bmp.PaletteSize((LPSTR)lpbi) + *(LPDWORD)lpbi;
	LPBYTE lpSrcBytes = (LPBYTE)lpbi + wBiSize;
	int sw = lpbi->bmiHeader.biWidth;
	int sh = abs(lpbi->bmiHeader.biHeight);
	WORD wbits = lpbi->bmiHeader.biBitCount;

	if (rect.left < 0 || rect.right >= sw || rect.top  < 0 || rect.bottom >= sh)
		return NULL;
	else
	{
		long temptop = sh - rect.bottom ;
		rect.bottom = sh - rect.top ;
		rect.top = temptop;
	}

	int dw = rect.right - rect.left + 1;
	int dh = rect.bottom - rect.top + 1;
	DWORD nDestWidthBytes = DWORD_WBYTES(dw * wbits);

	HDIB hDes = (HDIB)GlobalAlloc(GHND,nDestWidthBytes * dh + wBiSize);
	LPBYTE lpDestBytes = (LPBYTE)::GlobalLock(hDes);
	if (lpDestBytes == NULL)		//add by yaka 20010528
	{
		return NULL;
	}  //add end
	memcpy(lpDestBytes, lpbi, wBiSize);
	LPBITMAPINFOHEADER lpTempInfo = (LPBITMAPINFOHEADER)lpDestBytes;
	lpTempInfo->biWidth = dw;
	lpTempInfo->biHeight = dh;
	lpTempInfo->biSizeImage = nDestWidthBytes * dh;
	lpDestBytes += wBiSize;

	DWORD nSrcWidthBytes = DWORD_WBYTES(sw * wbits);
	int nSrcPos, nSrcPos1, nDestPos, nDestPos1;
	int i,j;

	BYTE nSrcBit0, nSrcBit, nDestBit;
	nSrcBit0 = (BYTE)128 >> rect.left % 8;
	//��ʼλ�á�
	nSrcPos = (rect.top * nSrcWidthBytes) + rect.left * wbits / 8;
	nDestPos = 0;

	for (j = rect.top; j <= rect.bottom; j++) {
		nSrcPos1 = nSrcPos;
		nSrcBit = nSrcBit0;
		nDestBit = 128;
		nDestPos1 = nDestPos;
		if (wbits == 1)
		{
			for (i=rect.left; i<=rect.right; i++) 
			{
				if (nSrcBit & lpSrcBytes[nSrcPos1])
					lpDestBytes[nDestPos1] |= nDestBit;
				nSrcBit >>= 1;
				if (nSrcBit == 0) 
				{
					nSrcBit = 128;
					nSrcPos1 ++;
				}
				nDestBit >>= 1;
				if (nDestBit == 0)
				{
					nDestBit = 128;
					nDestPos1 ++;
				}
			}
		}
		else
		{
			memcpy(lpDestBytes + nDestPos,lpSrcBytes + nSrcPos,nDestWidthBytes);
		}
		nSrcPos += nSrcWidthBytes;
		nDestPos += nDestWidthBytes;
	}
	::GlobalUnlock(hDes);
	::GlobalUnlock(hDIB);
	return hDes;
}

// ��������	: ���ڽ�HDIBͼ�����ݴ�ֱ��ת��
// ����		: HDIB pHdib
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
//add by yaka 20010601
BOOL WINAPI ReverseImage(HDIB pHdib)
{
	LPBYTE pOut = (LPBYTE)GlobalLock(pHdib);
	int i;
	LPBITMAPINFOHEADER lpBIH = (LPBITMAPINFOHEADER)pOut;
	WORD wSize = IMAGE_COLORS(lpBIH->biBitCount,1) * sizeof(RGBQUAD);
	pOut += sizeof(BITMAPINFOHEADER) + wSize;
	LPBYTE pTempOut = pOut;
	WORD wWidthBytes = (WORD)DWORD_WBYTES(lpBIH->biWidth * lpBIH->biBitCount);
	WORD LoopCount = lpBIH->biHeight / 2;
	HGLOBAL hTempTrans = GlobalAlloc(GHND,wWidthBytes);
	HGLOBAL hTempSrc = GlobalAlloc(GHND,wWidthBytes);
	LPSTR lpTempTrans = (LPSTR)GlobalLock(hTempTrans);
	LPSTR lpTempSrc = (LPSTR)GlobalLock(hTempSrc);
	if((lpTempTrans == NULL) || (lpTempSrc == NULL))
	{
		GlobalUnlock(pHdib);
		return FALSE;
	}
	for(i=1; i<=LoopCount; i++)
	{
		memcpy(lpTempSrc,pTempOut,wWidthBytes);
		memcpy(lpTempTrans,lpTempSrc,wWidthBytes);
		pTempOut = pOut+((LoopCount-i)*2+1)*wWidthBytes;
		memcpy(lpTempSrc,pTempOut,wWidthBytes);
		memcpy(pTempOut,lpTempTrans,wWidthBytes);
		pTempOut = pTempOut - ((LoopCount-i)*2+1)*wWidthBytes;
		memcpy(pTempOut,lpTempSrc,wWidthBytes);
		pOut = (pTempOut += wWidthBytes);
	}
	GlobalUnlock(hTempTrans);
	GlobalUnlock(hTempSrc);
	GlobalFree(hTempTrans);
	GlobalFree(hTempSrc);
	return TRUE;
}
//add end
//����		:���ڽ�DIBͼ��������ת
//����ֵ	:��ת���HDIB
HDIB WINAPI RotateImage(HDIB hInImage,			//	I ͼ����
						double dAngle)			//	I �Ƕ�
{
	long lAngle = ((long)dAngle % 360) ;
	if (lAngle < 0)
		lAngle +=360;	
	if (lAngle == 0)
		return hInImage;
	double dAngle2 = lAngle * (double)3.1415927/(double)180.0;
	double dCosAngle = cos(dAngle2);
	double dSinAngle = sin(dAngle2);

	WORD wCol = 0; //���ھ�����ת��������Ͻ�ˮƽ�������ꡣ
	WORD wRow = 0; //���ھ�����ת��������ϽǴ�ֱ�������ꡣ
	WORD wWidth; //���ھ�����ת����Ŀ�ȣ�
	WORD wHeight; //���ھ�����ת����ĸ߶ȣ�

	LPSTR lpInImage = (LPSTR)::GlobalLock(hInImage);
	if (lpInImage == NULL)
		return NULL;
	LPBITMAPINFO m_lpBitMapInfo = (LPBITMAPINFO)lpInImage;
	WORD BitsPerPixel = m_lpBitMapInfo->bmiHeader.biBitCount;
	WORD wOldWidth = (WORD)m_lpBitMapInfo->bmiHeader.biWidth;
	WORD wOldHeight = (WORD)m_lpBitMapInfo->bmiHeader.biHeight;

	WORD wOffsetx;	
	WORD wOffsety;
	wWidth = wOldWidth;  
	wHeight = wOldHeight;
	DWORD dwBmpWidth = DWORD_WBYTES(wWidth*BitsPerPixel);

	switch (lAngle)
	{
	case 90:
	case 270:
		wOffsety = (wWidth - wHeight ) / 2 ;
		wOffsetx = (wHeight - wWidth ) / 2 ;
		break;
	case 180:
		wOffsetx = wOffsety = 0;
		break;
	default:
		{
			double dAngle3;
			if (lAngle > 180.0)
				dAngle3 = (lAngle - 180.0) * (double)3.1415927/(double)180.0;
			else
				dAngle3 = lAngle * (double)3.1415927/(double)180.0;
			if (lAngle < 90.0 || (lAngle > 180.0 && lAngle < 270.0)) 
			{
				 wOffsetx = WORD(sqrt((double)(wWidth * wWidth + wHeight * wHeight)) / 2.0 * sin(dAngle3 + atan((double)wWidth / (double)wHeight)) - wWidth / 2.0) ;
				 wOffsety = WORD(sqrt((double)(wWidth * wWidth + wHeight * wHeight)) / 2.0 * sin(dAngle3 + atan((double)wHeight / (double)wWidth)) - wHeight / 2.0) ;
			}
			else
			{
				 wOffsetx = WORD(sqrt((double)(wWidth * wWidth + wHeight * wHeight)) / 2.0 * sin(dAngle3 - atan((double)wWidth / (double)wHeight)) - wWidth / 2.0) ;
				 wOffsety = WORD(sqrt((double)(wWidth * wWidth + wHeight * wHeight)) / 2.0 * sin(dAngle3 - atan((double)wHeight / (double)wWidth)) - wHeight / 2.0) ;
			}
		}
	}
	wWidth = wWidth + wOffsetx * 2;
	wHeight = wHeight + wOffsety * 2;

	register unsigned short ImageCol, ImageRow;
    WORD wCenterCol, wCenterRow, wSPixelColNum, wSPixelRowNum;
	WORD wColExtent, wRowExtent;
	WORD wPtA ;
	double dPixelRelativeColNum, dPixelRelativeRowNum;
	double dSPixelColAddr, dSPixelRowAddr; //dCosAngle, dSinAngle,
	BYTE ByMask, ByTemp = 0x80;
	WORD wSizeOfHeader;
	WORD wi ;

	wSizeOfHeader = (WORD)*(LPDWORD)lpInImage + GetPaletteSize(hInImage);
	lpInImage += wSizeOfHeader;

	wCenterCol = wCol + (wWidth - 1 )/2 ; //���� 
	wCenterRow = wRow + (wHeight - 1 )/2 ;		

	DWORD dwBmpSonWidth = DWORD_WBYTES(wWidth*BitsPerPixel);
	HDIB hOutImage = (HDIB)::GlobalAlloc(GHND,dwBmpSonWidth*wHeight+wSizeOfHeader);
	if(hOutImage == NULL)
		return NULL;
	LPBYTE lpOutImage = (LPBYTE)::GlobalLock(hOutImage);
	if(lpOutImage == NULL)
		return NULL;

	memcpy(lpOutImage,m_lpBitMapInfo,wSizeOfHeader);
	LPBITMAPINFO m_lpOutBitMapInfo = (LPBITMAPINFO)lpOutImage;
	m_lpOutBitMapInfo->bmiHeader.biSizeImage = wHeight*dwBmpSonWidth;
	m_lpOutBitMapInfo->bmiHeader.biWidth = wWidth;
	m_lpOutBitMapInfo->bmiHeader.biHeight = wHeight;

	lpOutImage += wSizeOfHeader;
	memset(lpOutImage,0xFF,dwBmpSonWidth*wHeight);

	LPBYTE lpTempOut = lpOutImage;
	int pos=0, pos1,pos2=0;

	wColExtent = wCol + wWidth;
	wRowExtent = wRow + wHeight;

	for (ImageRow=wRow; ImageRow < wRowExtent; ImageRow++)
	{
		if (BitsPerPixel == 1)
			ByMask = 0x80;
		else
			ByMask = 0xF0;

		pos1 = pos;
		pos2 = pos;//֮����ÿ�ζ���ֵ������ΪwColExtent(��ѭ������)���ܲ�ͬ��dwBmpSonWidth��
		dPixelRelativeRowNum = (double)ImageRow - wCenterRow;
		for(ImageCol=wCol; ImageCol< wColExtent; ImageCol++)
		{
			//modify by yaka 20010412
			switch ((int)(dAngle))
			{
			case 90:
				wSPixelColNum = wCenterCol - wOffsetx - (ImageRow - wCenterRow);
				wSPixelRowNum = wCenterRow - wOffsety + (ImageCol - wCenterCol);
				break;
			case 180:
				wSPixelColNum = wCenterCol  - (ImageCol - wCenterCol);
				wSPixelRowNum = wCenterRow  - (ImageRow - wCenterRow);
				break;
			case 270:
				wSPixelColNum = wCenterCol - wOffsetx + (ImageRow - wCenterRow);
				wSPixelRowNum = wCenterRow - wOffsety - (ImageCol - wCenterCol);
				break;
			default:
				{
					dPixelRelativeColNum = (double)ImageCol - wCenterCol;
					dSPixelColAddr = wCenterCol - wOffsetx +(dPixelRelativeColNum * dCosAngle - dPixelRelativeRowNum * dSinAngle);//u=x*CosAngle - y*SinAngle
					dSPixelRowAddr = wCenterRow - wOffsety +(dPixelRelativeColNum * dSinAngle + dPixelRelativeRowNum * dCosAngle);//v=x*SinAngle + y*CosAngle		
					wSPixelColNum = (WORD)(dSPixelColAddr+0.5);
					wSPixelRowNum = (WORD)(dSPixelRowAddr+0.5);
				}
			}
				//modify end 

			// ��Խ���߽磬��ȱʡֵ����Ϊ1����
			wi = 0;
			do
			{		//modify by yaka 20010411
				if((wSPixelColNum>=wCol) && (wSPixelRowNum>=wRow) && (wSPixelColNum< wOldWidth) && (wSPixelRowNum< wOldHeight))
				{
					if(BitsPerPixel <= 4)//modify by yaka 20010705
						wPtA = (WORD)(lpInImage[wSPixelRowNum*dwBmpWidth + wSPixelColNum * BitsPerPixel/8]);
					else
						wPtA = (WORD)(lpInImage[wSPixelRowNum*dwBmpWidth + wSPixelColNum * BitsPerPixel/8 + wi]);	

					if(wPtA==0xFF)
						goto HERE;
					if(BitsPerPixel == 1)
					{
						ByTemp = 0x80 >> (wSPixelColNum % 8 );
						if(!(ByTemp & wPtA))
							lpTempOut[pos1] &= (~ByMask);//����ByMask��ΪByTemp�����!
					}
					else if (BitsPerPixel == 4)
						lpTempOut[pos1] &= (~ByMask | (BYTE)wPtA );
					else
						lpTempOut[pos2] = (BYTE)wPtA;
			
				}
	HERE:		if(BitsPerPixel == 1)
				{
					ByMask >>= 1;
					if (ByMask == 0) 
					{
						pos1 ++;
						ByMask = 0x80;
					}
				}
				else if(BitsPerPixel == 4)
				{
					ByMask >>= 4;
					if (ByMask == 0) 
					{
						pos1 ++;
						ByMask = 0xF0;
					}
				}
				else
					pos2++;
				wi++;
			}while(wi<(BitsPerPixel/8));
		}
		pos += dwBmpSonWidth;
	}
	::GlobalUnlock(hInImage);
	::GlobalUnlock(hOutImage);
	return hOutImage;

}

// ��������	: �������Ĵ������ֵ
// ����ֵ	: int �������ֵ
// ��������	: 2001/06/01
// ����		: yaka
// ��ע 	: 
int WINAPI GetLastErrorCode()
{
	return iReturn;
}

// ��������	: ��ȡ��ͼ������
// ����ֵ	: int ͼ������
// ��������	: 2001/07/03
// ����		: yaka
// ��ע 	: 
int WINAPI GetSubImageCount()
{
	return g_ImageCount;
}
// ��������	: ��ȡͼ���ɫ���С
// ����ֵ	: WORD ��ɫ���С
// ��������	: 2001/07/23
// ����		: yaka
// ��ע 	: 
WORD WINAPI GetPaletteSize(HDIB hDIB)
{
	CBMP Bmp;
	WORD wReturn;
	LPSTR lpbi = (LPSTR)GlobalLock(hDIB);
	if (lpbi == NULL)
		wReturn = 0;
	else
		wReturn = Bmp.PaletteSize(lpbi);
	GlobalUnlock(hDIB);
	return wReturn;
}

BYTE WINAPI GetImageFormat()
{
	return g_ImageFormat;
}

// ��������	: ��ȡͼ���С
// ����ֵ	: BOOL �Ƿ�ɹ�
// ��������	: 2001/08/03
// ����		: yaka
// ��ע 	: 
BOOL WINAPI GetImageSize(HDIB p_hDIB,			//   I  ͼ����
						 DWORD *p_pdwWidth,		//	 O	ͼ����
						 DWORD *p_pdwHeight)	//	 O	ͼ��߶�
{
	LPSTR lpdib = (LPSTR)GlobalLock(p_hDIB);
	if (lpdib== NULL)
		return FALSE;
	CBMP bitmap;
	CSize bmpSize(bitmap.GetSize(lpdib));
	*p_pdwWidth = bmpSize.cx;
	*p_pdwHeight = bmpSize.cy;
	GlobalUnlock(p_hDIB);
	return TRUE;
}
//����		:��ȡͼ��DPI
//����ֵ	:BOOL �Ƿ�ɹ�
// ��������	: 2001/08/03
// ����		: yaka
// ��ע 	: 
BOOL WINAPI GetImageDPI(HDIB p_hDIB,			//	I	ͼ����
						DWORD *p_pdwDpiX,		//	O	X����DPI
						DWORD *p_pdwDpiY)		//	O	Y����DPI
{
	LPSTR lpdib = (LPSTR)GlobalLock(p_hDIB);
	if (lpdib== NULL)
		return FALSE;
	CBMP bitmap;
	CSize DpiSize(bitmap.GetDPI(lpdib));
	*p_pdwDpiX = DpiSize.cx;
	*p_pdwDpiY = DpiSize.cy;
	GlobalUnlock(p_hDIB);
	return TRUE;
}

//����		:�ж��Ƿ��ɫ
//����ֵ	:BOOL �Ƿ��ɫ
BOOL WINAPI ImageIsColor(HDIB p_hDIB)			//	I	ͼ����
{
	LPSTR lpdib = (LPSTR)GlobalLock(p_hDIB);
	if (lpdib == NULL)
		return FALSE;
	CBMP bitmap;
	BOOL bReturn = bitmap.IsColor(lpdib);
	GlobalUnlock(p_hDIB);
	return bReturn;
}
