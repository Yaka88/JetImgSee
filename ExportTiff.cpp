// CExportTIF.cpp: implementation of the CExportTIF class.
//
//////////////////////////////////////////////////////////////////////
// 文件名	: ExportTIFF.cpp
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 保存为TIFF的实现文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
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
// 功能描述	: 保存HDIB为TIFF文件
// 参数		: HDIB hInBmp,图像句柄 LPSTR lpszDstFName目标文件,long wEncodeNumber压缩方式
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:
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
	WORD			 wi;//用于for循环记数;

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
	{	AfxMessageBox("该BMP文件已经压缩过!");
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
		AfxMessageBox("这是32为真彩色BMP文件，暂不支持!");
		::GlobalUnlock(hInBmp);
		//::GlobalFree(hInBmp);//rem by yaka 20010525
		iReturn = NOSUPPORT_BMP;
		return iReturn;
	}
	switch (wEncodeNumber)
	{
	case 0 : gwEncode = CN_Uncompressed; break;
	case 1 : if(TIFCVar.wBits != 1) {
				 AfxMessageBox("非黑白图象，CCITT 1D 不支持!");
				 return (iReturn = NOSUPPORT_BMP);
			 }
			 gwEncode = CN_CCITT1D;
			 break;
	case 4 : if(TIFCVar.wBits != 1) {
				 AfxMessageBox("非黑白图象，Group3Fax T4不支持!");
				 return (iReturn = NOSUPPORT_BMP);
			 }
			 gwEncode= CN_Group3Fax ; break;
	case 5 : if(TIFCVar.wBits != 1) {
				 AfxMessageBox("非黑白图象，Group4Fax T6不支持!");
				 return (iReturn = NOSUPPORT_BMP);
			 }
			 gwEncode = CN_Group4Fax; break;
	case 2 : gwEncode = CN_LZW; break;
	case 6 : gwEncode = CN_JPEG;break;  //add by yaka 20010524
	case 3 : gwEncode = CN_PackBits; break;
	default :gwEncode = CN_JPEG; break;
	}
	TIFCVar.wEncodeMethod = gwEncode;

	//得到Tiff文件中一行数据应占的字节数；
	TIFCVar.wLineBytes = (WORD)BYTE_WBYTES(	TIFCVar.dwWidth *(DWORD)TIFCVar.wBits);
	// ** 确保一行数据的字节长度是4的倍数，因为BMP文件的一行长度总是4的倍数。
	wWidthBytes = (WORD) DWORD_WBYTES(TIFCVar.dwWidth * (DWORD)TIFCVar.wBits);
	wColors = IMAGE_COLORS(TIFCVar.wBits,1);

	if(wColors)
	{
		// ** read palette from pInBmp
		//m_SrcFile->Read(TmpPal,(wColors << 2)); //BMP color 描述每种颜色要四个字节，一个为空；
		TmpPal = (RGBQUAD*)pInBmp;
		pInBmp += (sizeof(RGBQUAD)*wColors);
		lpwTemp0 = wTIF_Pal;
		lpwTemp1 = lpwTemp0 + wColors;
		lpwTemp2 = lpwTemp1+ wColors;
		wTemp = 0;
		*lpwTemp0 = 0;
		*lpwTemp1 = 0;
		*lpwTemp2 = 0;
		for(wi=0;wi<wColors;wi++) // TIFF调色板的R、G、B值分别连续存放；
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


	// ** 调用iCreateTIF创建TIFF文件
	if(iReturn = iCreateTIF(pInBmp,m_TempFile,&TIFCVar,wWidthBytes))
	{
		//m_SrcFile->Close();
		::GlobalUnlock(hInBmp);
		//::GlobalFree(hInBmp);//rem by yaka 20010525
		m_TempFile->Close();
		return iReturn;
	}

	::GlobalUnlock(hInBmp);
	//::GlobalFree(hInBmp);//由外面释放
	m_TempFile->Close();
	if(m_TempFile != NULL)
	{
		delete m_TempFile;
		m_TempFile = NULL;
	}
	return SUCCESS;
}
// 功能描述	: 创建TIF文件
// 参数		: CFile *m_DstFile, LPBYTE lpSrcImage, LPTIFC_VAR lpTIFCVar, WORD wWidthBytes
//				目标文件            源图像句柄			GIF结构					行字节数
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	: 

int CExportTIF::iCreateTIF(LPBYTE pInBmp, CFile *m_TempFile, LPTIFC_VAR lpTIFCVar, WORD wWidthBytes)
{
	TIFHEADER TIFH;//MAX_TAG_COUNT
	TIFFTAG TIFT[15] = //其中dwCount字段和ValOff字段都被初始化为1和0；下面需要对它们进行赋值。
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
	/* 0x00:缺省模式，图象数据仅采用一维编码，EOL不在字节边界结束；（一行编码结束后，加上填充位，凑成整字节，然后跟上EOL，EOL不再凑成整字节；）
	   0x01:图象数据采用二维编码方式，即一行一维编码后，再连续跟着若干行（即PARAMETER宏所指）二维编码；，EOL不在字节边界结束；
	   0x04:EOL在字节边界结束，即一行编码结束后，根据具体的情况，使得添加的bit个数加上EOL长度正好凑成整个字节；图象数据仅采用一维编码；
	   0x05:EOL在字节边界结束；图象数据仅采用二维编码；*/
	if(gwEncode == CN_Group3Fax)
	{   //使EOL是否处于字节边界，是否采用二维编码；
		lpTIFCVar->dwGroupOption = (DWORD)0x05;//0x01;//0x00//0x04;//modify by yaka 20010613
		TifAddedTag.wTag = TN_Group3Option;
		TifAddedTag.wType = DTN_LONG;
		TifAddedTag.dwCount = 1;
		TifAddedTag.ValOff.dwValue = lpTIFCVar->dwGroupOption;
	}
	HANDLE	hSrcBuff;
	HANDLE	hImage;
	HANDLE	hTemp;
	LPDWORD	lpdwOffsets;//指向保存每个数据带地址的地方；
	LPDWORD	lpdwByteCount;//指向保存每个数据带大小的地方；
	LPSTR	lpImage;//指向暂存一行图象数据的地方
	LPSTR	lpTemp;
	DWORD	dwTemp;
	DWORD	dwi;//用于for循环
	DWORD	dwj;//用于for循环
	WORD	wDirTagCnt;
	WORD	wRunCount;
	WORD	wTemp;

	WORD wTempRowsPerStrip = MAX_STRIP_SIZE/lpTIFCVar->wLineBytes;// 每个数据带有几行
	wTempRowsPerStrip = (wTempRowsPerStrip/PAREMETERK) * PAREMETERK;//倘若使用GROUP3编码，则每个strip的行数应该是PAREMETERK整数倍；
	//modify by yaka 20010524
	if (gwEncode == CN_JPEG)
		lpTIFCVar->dwRowsPerStrip = lpTIFCVar->dwDepth;
	else
		lpTIFCVar->dwRowsPerStrip = (DWORD)wTempRowsPerStrip;
	//modify end

	lpTIFCVar->dwStripLen =  lpTIFCVar->dwDepth / lpTIFCVar->dwRowsPerStrip; // 表明有几个数据带
	if(lpTIFCVar->dwDepth % lpTIFCVar->dwRowsPerStrip)
		lpTIFCVar->dwStripLen ++; // 则最后一个数据带的行数例外（小于上面的dwRowsPerStrip);
	
	TIFT[ImageWidth].ValOff.dwValue = lpTIFCVar->dwWidth;
	TIFT[ImageHeight].ValOff.dwValue = lpTIFCVar->dwDepth;
	TIFT[Compression].ValOff.dwValue = (DWORD)gwEncode;

	if((lpTIFCVar->wBits == 1) || m_bGray)// 黑白或灰度图象；
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
		AfxMessageBox("创建TIFF文件时，申请内存错误!");
		iReturn = CREATETIF_AllOCMEMERR;
		return iReturn;
	}

		hSrcBuff = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);//32k,其实只用来保存一个编好码的数据带;

	//modify end
	lpTIFCVar->lpDataBuff = (char*)GlobalLock(hSrcBuff);
	lpTIFCVar->lpEndBuff = lpTIFCVar->lpDataBuff;

	
	lpTIFCVar->dwTempCode = 0UL;
	lpTIFCVar->dwRowCount = 1UL;
	lpTIFCVar->wByteCount = 0;// 用于在下面累计每个数据带的字节数(一次加上一行字节数);
	lpTIFCVar->byLeftBits = 0x00;

	hImage = GlobalAlloc(GHND,(DWORD)wWidthBytes);
	lpImage =(char*) GlobalLock(hImage);
	if((lpImage == NULL) || (lpTIFCVar->lpDataBuff == NULL))
	{
		AfxMessageBox("创建TIFF文件时，申请内存错误!");
		iReturn = CREATETIF_AllOCMEMERR;
		return iReturn;
	}
	if(gwEncode == CN_PackBits)
	{
		hTemp = GlobalAlloc(GHND,(DWORD)(wWidthBytes + 1));
		lpTemp = (char*)GlobalLock(hTemp);
		if (lpTemp == NULL) 
		{
			AfxMessageBox("创建TIFF文件时，申请内存错误!");
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
			AfxMessageBox("创建TIFF文件时，申请内存错误!");
			iReturn = CREATETIF_AllOCMEMERR; 
			return iReturn;
		}
		lpTIFCVar->iLastLineOffset = (int*)lpTemp;
	}
	// ** 开始从BMP读图象数据，向TIFF写图象数据（不包括TAG部分），无论是源文件
	// ** 还是目标文件，都已一行数据为单位(LZW例外,已一个数据带为单位).
	pInBmp = pInBmp+(lpTIFCVar->dwDepth - 1)*wWidthBytes;
	for(dwi=0; dwi<lpTIFCVar->dwStripLen;dwi++)// 外层循环（次数与数据带个数相同）
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
			for(dwj=0;dwj<lpTIFCVar->dwRowsPerStrip;dwj++)//内层循环（次数与每个数据带行数相同）
			{   
				// ** 从BMP文件读一行数据
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
				if(lpTIFCVar->wLeftBits)//此时可能没有8位
				{
					*lpTIFCVar->lpEndBuff++ = (BYTE)(lpTIFCVar->dwTempCode << (8 - lpTIFCVar->wLeftBits));//结尾可能补零；
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
			dwTemp = (DWORD)m_TempFile->Seek(0L,CFile::current); //保存当时在写的TIFF文件指针（上面读完的数据带数据尚在内存lpData中）
			*(lpdwOffsets + dwi) = (DWORD)m_TempFile->Seek(0L,CFile::current);// 保存将指向该条数据带的文件指针到lpTIFCVar->hStripOffsets所指的地方；
			*(lpdwByteCount + dwi) = (DWORD)lpTIFCVar->wByteCount; //保存该条数据带的大小到lpTIFCVar->hStripByteCount所指的地方
			m_TempFile->Write(lpTIFCVar->lpDataBuff,lpTIFCVar->wByteCount);//真正将从BMP中读到的该条数据带数据存入文件；
		}
		lpTIFCVar->wByteCount = 0;//置0，准备下一次循环；
		lpTIFCVar->lpEndBuff = lpTIFCVar->lpDataBuff;
	} // 外层循环结束

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
		TIFT[BitsPerSample].ValOff.dwValue = (DWORD)lpTIFCVar->wBits; //此时dwCount为1，在主程序中已经初始化；




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

	// ** 对于BMP文件虽然第一行数据在图象下方(4UL),但一般的TIFF解压软件会自动考虑到;
	// ** 所以这里不必用4L.若要用4L,反而要多加一层工序,即将内存中的数据垂直反转后再写入TIF文件.
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
		m_TempFile->Write(wTIF_Pal,(wcolors << 1 ));// 共写入wcolors*6个字节；
	}

	if(lpTIFCVar->dwStripLen > 1)
	{
		wTemp = (WORD)(lpTIFCVar->dwStripLen << 2);
		TIFT[StripOffsets].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
		m_TempFile->Write(lpdwOffsets,wTemp);//将保存在lpdwOffsets中的各个数据带地址最终保存到TIF文件中；

		TIFT[StripByteCount].ValOff.dwOffset = (DWORD)m_TempFile->Seek(0L,CFile::current);
		m_TempFile->Write(lpdwByteCount,wTemp);//将保存在lpdwByteCount中的各个数据带大小最终保存到TIF文件中；
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

	// ** Write the IFD，注意：IFD部分其实写在TIFF文件的最后部分，所以可以安全添加IFD中的字段，但是若该
	// **字段的.ValOff.dwOffset有效，则要在次之前，现在文件的dwOffset处写好数据；
	wDirTagCnt = sizeof(TIFT) / sizeof(TIFFTAG);

	//add by yaka 20010621
	
	if((dwPhotoMetric!=(DWORD)CS_Palette))
		wDirTagCnt--;//TIFT[ColorMap]无效，这也是ColorMap放在TIFT最后的原因。

	//add end
	if(gwEncode == CN_Group3Fax || gwEncode == CN_JPEG)
		wDirTagCnt++;//多了一个TifAddedTag。
	 
		
	m_TempFile->Write(&wDirTagCnt,2);//写入有多少个标识信息组；
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
	// **只有一个信息区
	dwTemp = 0UL;
	m_TempFile->Write(&dwTemp,4);

	// ** Rewrite the TIFF file header
	m_TempFile->SeekToBegin();
	m_TempFile->Write(&TIFH,sizeof(TIFH));

	return iReturn;
}
// 功能描述	: 非压缩编码
// 参数		: LPTIFC_VAR lpTIFCVar,TIFF结构 LPSTR lpImage句柄
// 返回值	: int 错误值
// 创建日期	: 2000/02/01
// 作者		: yaka
// 备注 	:
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
