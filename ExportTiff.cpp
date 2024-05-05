// CExportTIF.cpp: implementation of the CExportTIF class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ExportTIFF.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����ΪTIFF��ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "ExportTiff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExportTIF::CExportTIF()
{
	//gwEncode = CN_LZW;// CN_PackBits;//CN_CCITT1D; //; //CN_Uncompressed;
	m_bGray = FALSE;
	m_dwBlackOrWhite = CS_BlackIsZero;
}

CExportTIF::~CExportTIF()
{

}
extern int iReturn ;
// ��������	: ����HDIBΪTIFF�ļ�
// ����		: HDIB hInBmp,ͼ���� LPSTR lpszDstFNameĿ���ļ�,long wEncodeNumberѹ����ʽ
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:
int CExportTIF::iExportTIF(HDIB hInBmp,const char *lpszDstFName, long wEncodeNumber)
{
	BITMAPINFOHEADER BIH;
	LPBYTE pInBmp;
	TIFHEADER		 TIFH;
	TIFC_VAR		 TIFCVar;
	//RGBQUAD			 TmpPal[256];
	RGBQUAD*		 TmpPal = NULL;
	LPWORD			 lpwTemp0;
	LPWORD			 lpwTemp1;
	LPWORD			 lpwTemp2;
	WORD			 wColors;
	WORD			 wWidthBytes;
	WORD			 wTemp;
	WORD			 wi;//����forѭ������;

	CFile*			 m_TempFile = new CFile();
	CFileException m_error;
	pInBmp = (LPBYTE)::GlobalLock(hInBmp);

	if (pInBmp == NULL)		//add by yaka 20010528
	{
		iReturn = GLOBALLOCKERROR;	
		return iReturn;
	}  //add end
	BIH = *((LPBITMAPINFOHEADER)pInBmp);
	pInBmp += sizeof(BITMAPINFOHEADER);

	if(BIH.biCompression != BI_RGB)
	{	AfxMessageBox("��BMP�ļ��Ѿ�ѹ����!");
		::GlobalUnlock(hInBmp);
		//::GlobalFree(hInBmp);//rem by yaka 20010525
		iReturn = BMP_HAVECOMPRESS;	
		if(m_TempFile != NULL)
		{
			delete m_TempFile;
			m_TempFile = NULL;
		}
		return iReturn;
	}
	TIFCVar.dwWidth = BIH.biWidth;
	TIFCVar.dwDepth = BIH.biHeight;
	TIFCVar.wBits = BIH.biBitCount;
	TIFCVar.xResolution = (DWORD)((BIH.biXPelsPerMeter * 25.4) / 1000);
	TIFCVar.yResolution = (DWORD)((BIH.biYPelsPerMeter * 25.4) / 1000);
	if(TIFCVar.wBits == 32)
	{
		AfxMessageBox("����32Ϊ���ɫBMP�ļ����ݲ�֧��!");
		::GlobalUnlock(hInBmp);
		//::GlobalFree(hInBmp);//rem by yaka 20010525
		iReturn = NOSUPPORT_BMP;
		return iReturn;
	}
	switch (wEncodeNumber)
	{
	case 0 : gwEncode = CN_Uncompressed; break;
	case 1 : if(TIFCVar.wBits != 1) {
				 AfxMessageBox("�Ǻڰ�ͼ��CCITT 1D ��֧��!");
				 return (iReturn = NOSUPPORT_BMP);
			 }
			 gwEncode = CN_CCITT1D;
			 break;
	case 4 : if(TIFCVar.wBits != 1) {
				 AfxMessageBox("�Ǻڰ�ͼ��Group3Fax T4��֧��!");
				 return (iReturn = NOSUPPORT_BMP);
			 }
			 gwEncode= CN_Group3Fax ; break;
	case 5 : if(TIFCVar.wBits != 1) {
				 AfxMessageBox("�Ǻڰ�ͼ��Group4Fax T6��֧��!");
				 return (iReturn = NOSUPPORT_BMP);
			 }
			 gwEncode = CN_Group4Fax; break;
	case 2 : gwEncode = CN_LZW; break;
	case 6 : gwEncode = CN_JPEG;break;  //add by yaka 20010524
	case 3 : gwEncode = CN_PackBits; break;
	default :gwEncode = CN_JPEG; break;
	}
	TIFCVar.wEncodeMethod = gwEncode;

	//�õ�Tiff�ļ���һ������Ӧռ���ֽ�����
	TIFCVar.wLineBytes = (WORD)BYTE_WBYTES(	TIFCVar.dwWidth *(DWORD)TIFCVar.wBits);
	// ** ȷ��һ�����ݵ��ֽڳ�����4�ı�������ΪBMP�ļ���һ�г�������4�ı�����
	wWidthBytes = (WORD) DWORD_WBYTES(TIFCVar.dwWidth * (DWORD)TIFCVar.wBits);
	wColors = IMAGE_COLORS(TIFCVar.wBits,1);

	if(wColors)
	{
		// ** read palette from pInBmp
		//m_SrcFile->Read(TmpPal,(wColors << 2)); //BMP color ����ÿ����ɫҪ�ĸ��ֽڣ�һ��Ϊ�գ�
		TmpPal = (RGBQUAD*)pInBmp;
		pInBmp += (sizeof(RGBQUAD)*wColors);
		lpwTemp0 = wTIF_Pal;
		lpwTemp1 = lpwTemp0 + wColors;
		lpwTemp2 = lpwTemp1+ wColors;
		wTemp = 0;
		*lpwTemp0 = 0;
		*lpwTemp1 = 0;
		*lpwTemp2 = 0;
		for(wi=0;wi<wColors;wi++) // TIFF��ɫ���R��G��Bֵ�ֱ�������ţ�
		{
			*lpwTemp0 ++ = (WORD)TmpPal[wi].rgbRed << 8;
			*lpwTemp1 ++ = (WORD)TmpPal[wi].rgbGreen << 8;
			*lpwTemp2 ++ = (WORD)TmpPal[wi].rgbBlue << 8;
		}
	}
	if(TIFCVar.wBits == 1){  //modify by yaka 200100601    
		if((TmpPal[0].rgbRed>TmpPal[1].rgbRed) == (gwEncode == CN_CCITT1D || gwEncode == CN_Group3Fax || gwEncode == CN_Group4Fax))
			m_dwBlackOrWhite = (DWORD)CS_BlackIsZero;
		else
			m_dwBlackOrWhite = (DWORD)CS_WhiteIsZero;

	}
	//add by yaka 20010621
	int tempreturn;
	if (gwEncode == CN_JPEG)
		tempreturn = m_TempFile->Open(lpszDstFName, CFile::modeWrite,&m_error);
	else
		tempreturn = m_TempFile->Open(lpszDstFName,CFile::modeCreate | CFile::modeWrite,&m_error);
	//add end
	if (tempreturn ==0)
	{
		m_error.ReportError();
		//m_SrcFile->Close();
		::GlobalUnlock(hInBmp);
		//::GlobalFree(hInBmp);//rem by yaka 20010525
		iReturn = CREATETIF_FAILED;
		if(m_TempFile != NULL)
		{
			delete m_TempFile;
			m_TempFile = NULL;
		}
		return CREATETIF_FAILED;
	}

	// ** write TIFF file header
	TIFH.wImageType = TIF_INTEL;
	TIFH.wVersion = TIF_VERSION;
	TIFH.lDirOffset = 0L;
	m_TempFile->Write(&TIFH,sizeof(TIFH));


	// ** ����iCreateTIF����TIFF�ļ�
	if(iReturn = iCreateTIF(pInBmp,m_TempFile,&TIFCVar,wWidthBytes))
	{
		//m_SrcFile->Close();
		::GlobalUnlock(hInBmp);
		//::GlobalFree(hInBmp);//rem by yaka 20010525
		m_TempFile->Close();
		return iReturn;
	}

	::GlobalUnlock(hInBmp);
	//::GlobalFree(hInBmp);//�������ͷ�
	m_TempFile->Close();
	if(m_TempFile != NULL)
	{
		delete m_TempFile;
		m_TempFile = NULL;
	}
	return SUCCESS;
}
// ��������	: ����TIF�ļ�
// ����		: CFile *m_DstFile, LPBYTE lpSrcImage, LPTIFC_VAR lpTIFCVar, WORD wWidthBytes
//				Ŀ���ļ�            Դͼ����			GIF�ṹ					���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

int CExportTIF::iCreateTIF(LPBYTE pInBmp, CFile *m_TempFile, LPTIFC_VAR lpTIFCVar, WORD wWidthBytes)
{
	TIFHEADER TIFH;//MAX_TAG_COUNT
	TIFFTAG TIFT[15] = //����dwCount�ֶκ�ValOff�ֶζ�����ʼ��Ϊ1��0��������Ҫ�����ǽ��и�ֵ��
	{
		{	TN_ImageWidth, DTN_LONG, 1UL, 0UL},
		{	TN_ImageLength, DTN_LONG, 1UL, 0UL},   
		{	TN_BitsPerSample, DTN_SHORT,1UL,0UL}, 
		{	TN_Compression, DTN_SHORT,1UL,0UL},   
		{	TN_PhotometricInterpretation, DTN_SHORT,1UL,1UL},
		{	TN_StripOffsets, DTN_LONG, 1UL, 0UL},  
		{	TN_SamplePerPixel, DTN_SHORT,1UL,0UL}, 
		{	TN_RowsPerStrip, DTN_LONG, 1UL, 0UL}, 
		{	TN_StripByteCounts, DTN_LONG, 1UL, 0UL}, 
		{	TN_XResolution, DTN_RATIONAL,1UL,0UL},
		{	TN_YResolution, DTN_RATIONAL,1UL,0UL}, 
		{	TN_ResolutionUnit, DTN_SHORT,1UL,2UL}, //modify by yaka 20010709
		{	TN_Copyright, DTN_ASCII,1UL,NULL},
		{	TN_Orientation,DTN_SHORT,1UL,1UL} ,
		{	TN_Colormap, DTN_SHORT,1UL,0UL} 
	};
	TIFFTAG TifAddedTag;
	/* 0x00:ȱʡģʽ��ͼ�����ݽ�����һά���룬EOL�����ֽڱ߽��������һ�б�������󣬼������λ���ճ����ֽڣ�Ȼ�����EOL��EOL���ٴճ����ֽڣ���
	   0x01:ͼ�����ݲ��ö�ά���뷽ʽ����һ��һά��������������������У���PARAMETER����ָ����ά���룻��EOL�����ֽڱ߽������
	   0x04:EOL���ֽڱ߽��������һ�б�������󣬸��ݾ���������ʹ����ӵ�bit��������EOL�������ôճ������ֽڣ�ͼ�����ݽ�����һά���룻
	   0x05:EOL���ֽڱ߽������ͼ�����ݽ����ö�ά���룻*/
	if(gwEncode == CN_Group3Fax)
	{   //ʹEOL�Ƿ����ֽڱ߽磬�Ƿ���ö�ά���룻
		lpTIFCVar->dwGroupOption = (DWORD)0x05;//0x01;//0x00//0x04;//modify by yaka 20010613
		TifAddedTag.wTag = TN_Group3Option;
		TifAddedTag.wType = DTN_LONG;
		TifAddedTag.dwCount = 1;
		TifAddedTag.ValOff.dwValue = lpTIFCVar->dwGroupOption;
	}
	HANDLE	hSrcBuff;
	HANDLE	hImage;
	HANDLE	hTemp;
	LPDWORD	lpdwOffsets;//ָ�򱣴�ÿ�����ݴ���ַ�ĵط���
	LPDWORD	lpdwByteCount;//ָ�򱣴�ÿ�����ݴ���С�ĵط���
	LPSTR	lpImage;//ָ���ݴ�һ��ͼ�����ݵĵط�
	LPSTR	lpTemp;
	DWORD	dwTemp;
	DWORD	dwi;//����forѭ��
	DWORD	dwj;//����forѭ��
	WORD	wDirTagCnt;
	WORD	wRunCount;
	WORD	wTemp;

	WORD wTempRowsPerStrip = MAX_STRIP_SIZE/lpTIFCVar->wLineBytes;// ÿ�����ݴ��м���
	wTempRowsPerStrip = (wTempRowsPerStrip/PAREMETERK) * PAREMETERK;//����ʹ��GROUP3���룬��ÿ��strip������Ӧ����PAREMETERK��������
	//modify by yaka 20010524
	if (gwEncode == CN_JPEG)
		lpTIFCVar->dwRowsPerStrip = lpTIFCVar->dwDepth;
	else
		lpTIFCVar->dwRowsPerStrip = (DWORD)wTempRowsPerStrip;
	//modify end

	lpTIFCVar->dwStripLen =  lpTIFCVar->dwDepth / lpTIFCVar->dwRowsPerStrip; // �����м������ݴ�
	if(lpTIFCVar->dwDepth % lpTIFCVar->dwRowsPerStrip)
		lpTIFCVar->dwStripLen ++; // �����һ�����ݴ����������⣨С�������dwRowsPerStrip);
	
	TIFT[ImageWidth].ValOff.dwValue = lpTIFCVar->dwWidth;
	TIFT[ImageHeight].ValOff.dwValue = lpTIFCVar->dwDepth;
	TIFT[Compression].ValOff.dwValue = (DWORD)gwEncode;

	if((lpTIFCVar->wBits == 1) || m_bGray)// �ڰ׻�Ҷ�ͼ��
	{
		/*if(gwEncode == CN_CCITT1D)//  || gwEncode == CN_Group3Fax || gwEncode == CN_Group4Fax
			TIFT[Photometric].ValOff.dwValue = (DWORD)CS_WhiteIsZero;
		else
			TIFT[Photometric].ValOff.dwValue = (DWORD)CS_BlackIsZero;*/
		TIFT[Photometric].ValOff.dwValue = (DWORD)m_dwBlackOrWhite;
	}
	else
		TIFT[Photometric].ValOff.dwValue = (DWORD)((lpTIFCVar->wBits == 24 || lpTIFCVar->wBits == 16) ? CS_RGB : CS_Palette);
	
	if(lpTIFCVar->wBits == 24 || lpTIFCVar->wBits == 16)
		TIFT[SamplePerPixel].ValOff.dwValue = 3 ;
	else	
		TIFT[SamplePerPixel].ValOff.dwValue = 1;

	TIFT[RowsPerStrip].ValOff.dwValue = lpTIFCVar->dwRowsPerStrip;
	TIFT[StripOffsets].dwCount = lpTIFCVar->dwStripLen;
	TIFT[StripByteCount].dwCount = lpTIFCVar->dwStripLen;


	
	lpTIFCVar->hStripOffsets = GlobalAlloc(GHND,(lpTIFCVar->dwStripLen << 2));
	lpTIFCVar->hStripByteCount = GlobalAlloc(GHND,(lpTIFCVar->dwStripLen << 2));
	lpdwOffsets = (LPDWORD)GlobalLock(lpTIFCVar->hStripOffsets);
	lpdwByteCount = (LPDWORD)GlobalLock(lpTIFCVar->hStripByteCount);
	if((lpdwOffsets == NULL) || (lpdwByteCount == NULL))
	{
		AfxMessageBox("����TIFF�ļ�ʱ�������ڴ����!");
		iReturn = CREATETIF_AllOCMEMERR;
		return iReturn;
	}

		hSrcBuff = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);//32k,��ʵֻ��������һ�����������ݴ�;

	//modify end
	lpTIFCVar->lpDataBuff = (char*)GlobalLock(hSrcBuff);
	lpTIFCVar->lpEndBuff = lpTIFCVar->lpDataBuff;

	
	lpTIFCVar->dwTempCode = 0UL;
	lpTIFCVar->dwRowCount = 1UL;
	lpTIFCVar->wByteCount = 0;// �����������ۼ�ÿ�����ݴ����ֽ���(һ�μ���һ���ֽ���);
	lpTIFCVar->byLeftBits = 0x00;

	hImage = GlobalAlloc(GHND,(DWORD)wWidthBytes);
	lpImage =(char*) GlobalLock(hImage);
	if((lpImage == NULL) || (lpTIFCVar->lpDataBuff == NULL))
	{
		AfxMessageBox("����TIFF�ļ�ʱ�������ڴ����!");
		iReturn = CREATETIF_AllOCMEMERR;
		return iReturn;
	}
	if(gwEncode == CN_PackBits)
	{
		hTemp = GlobalAlloc(GHND,(DWORD)(wWidthBytes + 1));
		lpTemp = (char*)GlobalLock(hTemp);
		if (lpTemp == NULL) 
		{
			AfxMessageBox("����TIFF�ļ�ʱ�������ڴ����!");
			iReturn = CREATETIF_AllOCMEMERR; 
			return iReturn;
		}
	}
	if((gwEncode == CN_Group3Fax) || (gwEncode == CN_Group4Fax))
	{
		hTemp = GlobalAlloc(GHND,(DWORD)(sizeof(unsigned int)*MAXCHANGE));
		lpTemp = (char*)GlobalLock(hTemp);
		if (lpTemp == NULL) 
		{
			AfxMessageBox("����TIFF�ļ�ʱ�������ڴ����!");
			iReturn = CREATETIF_AllOCMEMERR; 
			return iReturn;
		}
		lpTIFCVar->iLastLineOffset = (int*)lpTemp;
	}
	// ** ��ʼ��BMP��ͼ�����ݣ���TIFFдͼ�����ݣ�������TAG���֣���������Դ�ļ�
	// ** ����Ŀ���ļ�������һ������Ϊ��λ(LZW����,��һ�����ݴ�Ϊ��λ).
	pInBmp = pInBmp+(lpTIFCVar->dwDepth - 1)*wWidthBytes;
	for(dwi=0; dwi<lpTIFCVar->dwStripLen;dwi++)// ���ѭ�������������ݴ�������ͬ��
	{
		if(gwEncode == CN_Group4Fax || gwEncode == CN_Group3Fax) {
			lpTIFCVar->wLeftBits = 0;
			lpTIFCVar->dwTempCode = 0;
		}
		if(lpTIFCVar->dwRowsPerStrip > (lpTIFCVar->dwDepth - lpTIFCVar->dwRowCount))
			lpTIFCVar->dwRowsPerStrip = lpTIFCVar->dwDepth - lpTIFCVar->dwRowCount + 1;
		if(gwEncode == CN_LZW)
			iReturn = m_LZW.iEncodeTIF_LZW(pInBmp,lpTIFCVar,wWidthBytes);
		else if(gwEncode == CN_JPEG)//add by yaka 20010621
		{
			lpTIFCVar->wByteCount = m_TempFile->GetLength() - 8;
		}
		else
		{
			for(dwj=0;dwj<lpTIFCVar->dwRowsPerStrip;dwj++)//�ڲ�ѭ����������ÿ�����ݴ�������ͬ��
			{   
				// ** ��BMP�ļ���һ������
				//m_SrcFile->Read(lpImage,wWidthBytes);
				memcpy(lpImage,pInBmp,wWidthBytes);
				pInBmp -= wWidthBytes;
				switch(gwEncode)
				{
				case CN_Uncompressed:
					iReturn = iEncodeLineTIF_RawData(lpTIFCVar,lpImage);
					break;
				case CN_PackBits:
					if(lpTIFCVar->wBits == 24)
						iReturn = m_utility.iEnArrangeRGB(lpTIFCVar->wLineBytes,lpImage);
					wRunCount = m_utility.wCompute8BitRun(lpImage,lpTemp,lpTIFCVar->wLineBytes);
					iReturn = m_EnPackBits.iEncodeLineTIF_PackBits(lpTIFCVar,lpImage,lpTemp,wRunCount);
					break;
				case CN_Group4Fax:
				case CN_Group3Fax:
				case CN_CCITT1D:
					iReturn = m_CCCITT.iEncodeGroup3(lpTIFCVar,lpImage,dwi,dwj);
					break;
				}
				if(iReturn)
					return iReturn;
				lpTIFCVar->dwRowCount ++ ;//iRatio = (int)((100L * (LONG)lpTIFCVar->dwRowCount ++) / (LONG)lpTIFCVar->dwDepth);
				//vStatusProcess(iRatio);
			}// end of for;
			if(gwEncode == CN_Group4Fax){
				if(lpTIFCVar->wLeftBits)//��ʱ����û��8λ
				{
					*lpTIFCVar->lpEndBuff++ = (BYTE)(lpTIFCVar->dwTempCode << (8 - lpTIFCVar->wLeftBits));//��β���ܲ��㣻
					lpTIFCVar->wByteCount++;
				}
			}
		} // end of else;
		if(gwEncode == CN_JPEG)//add by yaka 20010621
		{
			*(lpdwOffsets + dwi) = 8;
			*(lpdwByteCount + dwi) = (DWORD)lpTIFCVar->wByteCount; 
			m_TempFile->SeekToEnd();
		}		//add end
		else
		{
			dwTemp = (DWORD)m_TempFile->Seek(0L,CFile::current); //���浱ʱ��д��TIFF�ļ�ָ�루�����������ݴ����������ڴ�lpData�У�
			*(lpdwOffsets + dwi) = (DWORD)m_TempFile->Seek(0L,CFile::current);// ���潫ָ��������ݴ����ļ�ָ�뵽lpTIFCVar->hStripOffsets��ָ�ĵط���
			*(lpdwByteCount + dwi) = (DWORD)lpTIFCVar->wByteCount; //����������ݴ��Ĵ�С��lpTIFCVar->hStripByteCount��ָ�ĵط�
			m_TempFile->Write(lpTIFCVar->lpDataBuff,lpTIFCVar->wByteCount);//��������BMP�ж����ĸ������ݴ����ݴ����ļ���
		}
		lpTIFCVar->wByteCount = 0;//��0��׼����һ��ѭ����
		lpTIFCVar->lpEndBuff = lpTIFCVar->lpDataBuff;
	} // ���ѭ������

	if((gwEncode == CN_PackBits) || (gwEncode == CN_Group3Fax)|| (gwEncode == CN_Group4Fax))
	{
		GlobalUnlock(hTemp);
		GlobalFree(hTemp);
	}
	GlobalUnlock(hSrcBuff);
	GlobalUnlock(hImage);
	GlobalFree(hSrcBuff);
	GlobalFree(hImage);

	if(TIFT[Photometric].ValOff.dwValue == (DWORD)CS_RGB)
	{
		TIFT[BitsPerSample].dwCount = 3UL;
		TIFT[BitsPerSample].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
		wTemp = ((lpTIFCVar->wBits == 16) ? 5 : 8);
		m_TempFile->Write(&wTemp,2);
		m_TempFile->Write(&wTemp,2);
		m_TempFile->Write(&wTemp,2);
	}
	else
		TIFT[BitsPerSample].ValOff.dwValue = (DWORD)lpTIFCVar->wBits; //��ʱdwCountΪ1�������������Ѿ���ʼ����




	TIFT[Xresolution].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
	dwTemp = lpTIFCVar->xResolution;
	m_TempFile->Write(&dwTemp,4);
	dwTemp = 1UL;
	m_TempFile->Write(&dwTemp,4);

	TIFT[Yresolution].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
	dwTemp = lpTIFCVar->yResolution;
	m_TempFile->Write(&dwTemp,4);
	dwTemp = 1UL;
	m_TempFile->Write(&dwTemp,4);

	// ** ����BMP�ļ���Ȼ��һ��������ͼ���·�(4UL),��һ���TIFF��ѹ������Զ����ǵ�;
	// ** �������ﲻ����4L.��Ҫ��4L,����Ҫ���һ�㹤��,�����ڴ��е����ݴ�ֱ��ת����д��TIF�ļ�.
	TIFT[Orientation].ValOff.dwValue = 1UL; 

	char lpCopyright[CopyRightSize] = "JetSoft_Yaka\0";
	TIFT[CopyRight].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
	TIFT[CopyRight].dwCount = CopyRightSize;
	m_TempFile->Write(lpCopyright,CopyRightSize);

	DWORD dwPhotoMetric = TIFT[Photometric].ValOff.dwValue;
	if((dwPhotoMetric==(DWORD)CS_Palette))
	{	DWORD wcolors = IMAGE_COLORS(lpTIFCVar->wBits,1) * 3UL;
		TIFT[ColorMap].dwCount = wcolors;
		TIFT[ColorMap].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
		m_TempFile->Write(wTIF_Pal,(wcolors << 1 ));// ��д��wcolors*6���ֽڣ�
	}

	if(lpTIFCVar->dwStripLen > 1)
	{
		wTemp = (WORD)(lpTIFCVar->dwStripLen << 2);
		TIFT[StripOffsets].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
		m_TempFile->Write(lpdwOffsets,wTemp);//��������lpdwOffsets�еĸ������ݴ���ַ���ձ��浽TIF�ļ��У�

		TIFT[StripByteCount].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
		m_TempFile->Write(lpdwByteCount,wTemp);//��������lpdwByteCount�еĸ������ݴ���С���ձ��浽TIF�ļ��У�
	}
	else
	{
		TIFT[StripOffsets].ValOff.dwValue = *lpdwOffsets;
		TIFT[StripByteCount].ValOff.dwValue = *lpdwByteCount;
	}
	GlobalUnlock(lpTIFCVar->hStripByteCount);
	GlobalUnlock(lpTIFCVar->hStripOffsets);
	GlobalFree(lpTIFCVar->hStripByteCount);
	GlobalFree(lpTIFCVar->hStripOffsets);

	// ** Reset the TIF file header
	TIFH.wImageType = TIF_INTEL;
	TIFH.wVersion = TIF_VERSION;
	TIFH.lDirOffset = m_TempFile->Seek(0L,CFile::current);

	// ** Write the IFD��ע�⣺IFD������ʵд��TIFF�ļ�����󲿷֣����Կ��԰�ȫ���IFD�е��ֶΣ���������
	// **�ֶε�.ValOff.dwOffset��Ч����Ҫ�ڴ�֮ǰ�������ļ���dwOffset��д�����ݣ�
	wDirTagCnt = sizeof(TIFT) / sizeof(TIFFTAG);

	//add by yaka 20010621
	
	if((dwPhotoMetric!=(DWORD)CS_Palette))
		wDirTagCnt--;//TIFT[ColorMap]��Ч����Ҳ��ColorMap����TIFT����ԭ��

	//add end
	if(gwEncode == CN_Group3Fax || gwEncode == CN_JPEG)
		wDirTagCnt++;//����һ��TifAddedTag��
	 
		
	m_TempFile->Write(&wDirTagCnt,2);//д���ж��ٸ���ʶ��Ϣ�飻
	if(gwEncode == CN_Group3Fax || gwEncode == CN_JPEG)
		m_TempFile->Write(TIFT, ((wDirTagCnt-1)*sizeof(TIFFTAG)));
	else
		m_TempFile->Write(TIFT, (wDirTagCnt*sizeof(TIFFTAG)));
	if(gwEncode == CN_Group3Fax)
		m_TempFile->Write(&TifAddedTag, sizeof(TIFFTAG));
	else if (gwEncode == CN_JPEG) //add by yaka 20010524
	{
		TIFFTAG JPEGIFOffsetTag ;
		JPEGIFOffsetTag.wTag = TN_JPEGInterchangeFormat;
		JPEGIFOffsetTag.wType = DTN_LONG;
		JPEGIFOffsetTag.dwCount = 1;
		JPEGIFOffsetTag.ValOff.dwValue = TIFT[StripOffsets].ValOff.dwValue;
		m_TempFile->Write(&JPEGIFOffsetTag, sizeof(TIFFTAG));
	}
	//add end
	// **ֻ��һ����Ϣ��
	dwTemp = 0UL;
	m_TempFile->Write(&dwTemp,4);

	// ** Rewrite the TIFF file header
	m_TempFile->SeekToBegin();
	m_TempFile->Write(&TIFH,sizeof(TIFH));

	return iReturn;
}
// ��������	: ��ѹ������
// ����		: LPTIFC_VAR lpTIFCVar,TIFF�ṹ LPSTR lpImage���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	:
int CExportTIF::iEncodeLineTIF_RawData(LPTIFC_VAR lpTIFCVar, LPSTR lpImage)
{
	LPSTR lpImageBgn;
	WORD wi;

	lpImageBgn = lpImage;
	if(lpTIFCVar->wBits == 24 )//|| lpTIFCVar->wBits == 16
	{
		for(wi=0;wi<lpTIFCVar->wLineBytes;wi+=3)
		{
			SWAP(*lpImage,*(lpImage + 2));
			lpImage += 3;
		}
	}
	memcpy(lpTIFCVar->lpEndBuff,lpImageBgn,lpTIFCVar->wLineBytes);
	lpTIFCVar->lpEndBuff += lpTIFCVar->wLineBytes;
	lpTIFCVar->wByteCount += lpTIFCVar->wLineBytes;
	return SUCCESS;
}
