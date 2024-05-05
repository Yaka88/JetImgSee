// ImportTIF.cpp: implementation of the ImportTIF class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: ImportTIFF.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ��ΪTiFF��ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "ImportTiff.h"
#include "ImportJPG.h" //add by yaka 20010710
#include "io.h"

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

CImportTIF::CImportTIF()
{
	// ** Initial TIFDVar variable
	TIFDVar.hStripOffsets = NULL;
	TIFDVar.hStripByteCount = NULL;
}

CImportTIF::~CImportTIF()
{

}
// ��������	: ��TIF�ļ�
// ����		: LPSTR lpszSrcFName
// ����ֵ	: HDIB ͼ����
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
HDIB CImportTIF::iImportTIF(LPSTR lpszSrcFName) 
{
	PALETTEENTRY		SysPal256[256];
	HANDLE				hBI_Struct;
	HANDLE				hTemp, hTagBuff; // ���ڶ���ʶ��Ϣ����
	HGLOBAL hcopyright = NULL;
	HDC					hDC;
	LPBITMAPINFOHEADER	lpBIH;
	LPBITMAPINFO		lpBI;
	LPDWORD				lpdwTemp0, lpdwTemp1 ;
	LPWORD				lpwTemp0, lpwTemp1, lpwTemp2;
	LPSTR				lpTemp, lpTagBuff;//���ڶ���ʶ��Ϣ����
	DWORD				dwTemp;
	WORD				wDTN_Len[13] = {0,1,1,2,4,8,1,1,2,4,8,4,8};//��Ӧ12�����͵ĳ���(���ֽ�Ϊ��λ);
	WORD				wBI_Size, wPalSize, wColors, wDirTagCnt ;//��ʶ��ʾ��Ϣ��ĸ�����
	WORD				wWidthBytes, wTagSize;//��ʶ��Ϣ���Ĵ�С(��������ǰ�����ֽ�);
	WORD				wTemp, wi, wj;
	BYTE				byTIF_Pal[768],	byTemp;
	BYTE				bySysPal16[48] ={ 0,  0,  0,  0,  0,  128,0,128,  0,  0,  128,128,
										  128,0,  0,  128,0,  128,128,128,0,   0, 128,128,
										  192,192,192,0,  0,  255,0,  255,0,  0,  255,255,
										  255,0,  0,  255,0,  255,255,0,  0,  255,255,255
										}; 
	CFile*				m_SrcFile = new CFile();
	CFileException m_error;
	long JPEGIFOffset = 0 ;
	TIFDVar.IsJetSoft = FALSE;
	TIFDVar.wSrcBits = 1;
	TIFDVar.wSamples = 1;
	TIFDVar.wPhotoMet = 0;
	TIFDVar.wPlannarConfig = 1;
	TIFDVar.wPredictor = 1;
	TIFDVar.bPalette = FALSE;
	TIFDVar.bHorMirror = FALSE;
	TIFDVar.bVerMirror = FALSE;
	TIFDVar.FillOrder = 1;
	TIFDVar.dwGroupOption = (DWORD)0x00;

	if(!m_SrcFile->Open(lpszSrcFName,CFile::modeRead ,&m_error))
	{
		m_error.ReportError();
		return NULL;
	}
	// ** ��ȡTIFF�ļ�ͷ.
	m_SrcFile->SeekToBegin();
	if(iReadTiffHeader(m_SrcFile))
		return NULL;

	// ** ��ȡ��TIFF�ļ��ж��ٸ���ʶ��Ϣ��,�����ñ�ʶ��Ϣ�������ڴ�.
	m_SrcFile->Seek(TIFH.lDirOffset,CFile::begin);
	m_SrcFile->Read(&wDirTagCnt,2);
	if(!TIFDVar.bINTEL)
		m_utility.vSwapByte((LPSTR) & wDirTagCnt,1);
	wTagSize = wDirTagCnt * sizeof(TIFFTAG) + 4;
	hTagBuff = GlobalAlloc(GHND,(DWORD)wTagSize);
	lpTagBuff =(char*) GlobalLock(hTagBuff);
	m_SrcFile->Read(lpTagBuff,wTagSize);

	// ** Initial TIFDVar variable. Have inset into the constructor function;

// ***************************************************************************************************************
// ** ��ʼ��TIFF�ļ���ʶ��Ϣ���Ķ�ȡ���ж�.
	for(wi=0;wi<wDirTagCnt;wi++)
	{
		TIFT = *((LPTIFTAG)lpTagBuff);
		lpTagBuff += sizeof(TIFT);
		if(!TIFDVar.bINTEL)
		{
			m_utility.vSwapByte((LPSTR) & TIFT.wTag,1);
			m_utility.vSwapByte((LPSTR) & TIFT.wType,1);
			m_utility.vSwapWord((LPSTR) & TIFT.dwCount,1);
			if(((DWORD)wDTN_Len[TIFT.wType] * TIFT.dwCount) > 4UL)
				m_utility.vSwapWord((LPSTR)&TIFT.ValOff.dwOffset,1);
			else
			{
				if((TIFT.wType == DTN_LONG) || (TIFT.wType == DTN_SLONG))
					m_utility.vSwapWord((LPSTR)&TIFT.ValOff.dwValue,1);
				else
				{
					m_utility.vSwapByte((LPSTR)&TIFT.ValOff.Value.wVal1,1);
					m_utility.vSwapByte((LPSTR)&TIFT.ValOff.Value.wVal2,1);
				}
			}
		}
		switch(TIFT.wTag)
		{
		case TN_NewSubfileType:
			{
				DWORD dwTempfiletype = TIFT.ValOff.dwValue;
				break;
			}
		case TN_SubfileType:
			break;
		case TN_ImageWidth:
			TIFDVar.dwWidth = TIFT.ValOff.dwValue;
			break;
		case TN_ImageLength :
			TIFDVar.dwDepth = TIFT.ValOff.dwValue;
			break;
		case TN_BitsPerSample:
			{
				wTemp = wDTN_Len[TIFT.wType]*(WORD)TIFT.dwCount;
				if(wTemp>4)
				{
					hTemp = GlobalAlloc(GHND,(DWORD)wTemp);
					lpTemp = (char*)GlobalLock(hTemp);
					m_SrcFile->Seek(TIFT.ValOff.dwOffset,CFile::begin);
					m_SrcFile->Read(lpTemp,wTemp);
					if(!TIFDVar.bINTEL)
						m_utility.vSwapByte(lpTemp,(WORD)TIFT.dwCount);
					TIFDVar.wSrcBits = *(LPWORD)lpTemp;
					GlobalUnlock(hTemp);
					GlobalFree(hTemp);
				}
				else TIFDVar.wSrcBits = TIFT.ValOff.Value.wVal1;//TIFDVar.wSrcBits��ʾBMPÿ�����ص��λ�����3(�����ɫΪ8).
				break;
			}
		case TN_Compression:
			TIFDVar.wCompress = TIFT.ValOff.Value.wVal1;
			break;
		case TN_PhotometricInterpretation:
			TIFDVar.wPhotoMet = TIFT.ValOff.Value.wVal1;
			break;
		case TN_Thresholding:
			break;
		case TN_CellWidth:
			break;
		case TN_CellHeight:
			break;
		case TN_FillOrder:
			TIFDVar.FillOrder = TIFT.ValOff.Value.wVal1;
			break;
		case TN_DocumentName:
			break;
		case TN_ImageDescription:
			break;
		case TN_Make:
			break;
		case TN_Model:
			break;
		case TN_StripOffsets:
			{
				TIFDVar.wOffsetsLen = (WORD)TIFT.dwCount;//TIFDVar.wOffsetsLen��ʾ�ж��ٸ����ݴ�.
				wTemp = wDTN_Len[TIFT.wType] * TIFDVar.wOffsetsLen;
				TIFDVar.hStripOffsets = GlobalAlloc(GHND,(DWORD)wTemp);
				lpTemp = (char*)GlobalLock(TIFDVar.hStripOffsets);
				if(wTemp>4)
				{
					m_SrcFile->Seek(TIFT.ValOff.dwOffset,CFile::begin);
					m_SrcFile->Read(lpTemp,wTemp);
					if(!TIFDVar.bINTEL)
					{
						if((TIFT.wType == DTN_LONG) || (TIFT.wType == DTN_SLONG))
							m_utility.vSwapWord(lpTemp,TIFDVar.wOffsetsLen);
						else
							m_utility.vSwapByte(lpTemp,TIFDVar.wOffsetsLen);
					}
				}
				else
				{	/* ..... */
					lpdwTemp0 = (LPDWORD)lpTemp;
					*lpdwTemp0 = TIFT.ValOff.dwValue;
				}
				GlobalUnlock(TIFDVar.hStripOffsets);
				TIFDVar.wOffsetsType = TIFT.wType;
				break;
			}
		case TN_Orientation:
			{
				switch(TIFT.ValOff.Value.wVal1)
				{
				case 1:
					break;
				case 2:
					TIFDVar.bHorMirror = TRUE;
					break;
				case 3:
					TIFDVar.bHorMirror = TRUE;
					TIFDVar.bVerMirror = TRUE;
					break;
				case 4:
					TIFDVar.bVerMirror = TRUE;
					break;
				default:
					AfxMessageBox("��֧������ͼ����ת��ʽ!");
					break;
				}
				break;
			}
		case TN_SamplePerPixel:
			TIFDVar.wSamples = TIFT.ValOff.Value.wVal1;
			break;
		case TN_RowsPerStrip:
			TIFDVar.dwRowsPerStrip = TIFT.ValOff.dwValue;
			break;
		case TN_StripByteCounts:
			{
				TIFDVar.wByteCntLen = (WORD)TIFT.dwCount;
				wTemp = wDTN_Len[TIFT.wType] * TIFDVar.wByteCntLen;
				TIFDVar.hStripByteCount = GlobalAlloc(GHND,(DWORD)wTemp);
				lpTemp =(char*) GlobalLock(TIFDVar.hStripByteCount);
				if(wTemp>4)
				{
					m_SrcFile->Seek(TIFT.ValOff.dwOffset,CFile::begin);
					m_SrcFile->Read(lpTemp,wTemp);
					if(!TIFDVar.bINTEL)
					{
						if((TIFT.wType == DTN_LONG) || (TIFT.wType == DTN_SLONG))
							m_utility.vSwapWord(lpTemp,TIFDVar.wOffsetsLen);
						else
							m_utility.vSwapByte(lpTemp,TIFDVar.wOffsetsLen);
					}
				}
				else
				{	/* ..... */
					lpdwTemp0 = (LPDWORD) lpTemp;
					*lpdwTemp0 = TIFT.ValOff.dwValue;
				}
				GlobalUnlock(TIFDVar.hStripByteCount);
				TIFDVar.wByteCntType  = TIFT.wType;
				break;
			}
		case TN_MinsampleValue:
			break;
		case TN_MaxsampleValue:
			break;
		case TN_XResolution:{
			TIFDVar.wByteCntLen = (WORD)TIFT.dwCount;
			wTemp = wDTN_Len[TIFT.wType] * TIFDVar.wByteCntLen;
			DWORD dwTemp  ;
			m_SrcFile->Seek(TIFT.ValOff.dwOffset,CFile::begin);
			m_SrcFile->Read(&(TIFDVar.dwxResolution),4);
			m_SrcFile->Read(&dwTemp,4);
			TIFDVar.dwxResolution = TIFDVar.dwxResolution/dwTemp;
			break;}
		case TN_YResolution:{
			TIFDVar.wByteCntLen = (WORD)TIFT.dwCount;
			wTemp = wDTN_Len[TIFT.wType] * TIFDVar.wByteCntLen;
			DWORD dwTemp  ;
			m_SrcFile->Seek(TIFT.ValOff.dwOffset,CFile::begin);
			m_SrcFile->Read(&(TIFDVar.dwyResolution),4);
			m_SrcFile->Read(&dwTemp,4);
			TIFDVar.dwyResolution = TIFDVar.dwyResolution/dwTemp;
			break;}
		case TN_PlanarConfiguration:
			TIFDVar.wPlannarConfig = TIFT.ValOff.Value.wVal1;
			break;
		case TN_PageName:
			break;
		case TN_Xposition:
			break;
		case TN_Yposition:
			break;
		case TN_FreeOffsets:
			break;
		case TN_FreeByteCounts:
			break;
		case TN_GreyResponseCurveUnit:
			break;
		case TN_GreyResponseCurve:
			break;
		case TN_Group3Option:
			TIFDVar.dwGroupOption = TIFT.ValOff.dwValue;
			break;
		case TN_Group4Option:
			TIFDVar.dwGroupOption = TIFT.ValOff.dwValue;
			break;
		case TN_ResolutionUnit:
			TIFDVar.wResolutionUnit = TIFT.ValOff.Value.wVal1;
			break;
		case TN_PageNumber:
			break;
		case TN_TransferFunction:
			break;
		case TN_SoftWare:
			break;
		case TN_DateTime:
			break;
		case TN_Artist:
			break;
		case TN_HostComputer:
			break;
		case TN_Predictor:
			TIFDVar.wPredictor = TIFT.ValOff.Value.wVal1;
			break;
		case TN_WhitePoint:
			break;
		case TN_PrimaryChromaticities:
			break;
		case TN_Colormap:
			{
				TIFDVar.bPalette = TRUE;
				wColors = (WORD)TIFT.dwCount / 3;
				wTemp = wDTN_Len[TIFT.wType] * (WORD)TIFT.dwCount;
				//hTemp = GlobalAlloc(GHND,(DWORD)wTemp);//lpTemp = (char*)GlobalLock(hTemp);
				lpTemp = (char*)malloc(sizeof(char)*wTemp);
				m_SrcFile->Seek(TIFT.ValOff.dwOffset,CFile::begin);
				m_SrcFile->Read(lpTemp,wTemp);

				if(!TIFDVar.bINTEL)
					m_utility.vSwapByte(lpTemp,(WORD)TIFT.dwCount);
				memset((LPSTR)byTIF_Pal,0,768);
				lpwTemp0 = (LPWORD)lpTemp;
				lpwTemp1 = lpwTemp0 + wColors;
				lpwTemp2 = lpwTemp1 + wColors;
				wTemp = 0;
				for(wj=0;wj<wColors;wj++)
				{
					byTIF_Pal[wTemp++] = (BYTE)(*lpwTemp0++ >> 8);
					byTIF_Pal[wTemp++] = (BYTE)(*lpwTemp1++ >> 8);
					byTIF_Pal[wTemp++] = (BYTE)(*lpwTemp2++ >> 8);
				}
				//GlobalUnlock(hTemp);//GlobalFree(hTemp);
				if(lpTemp != NULL)
					free(lpTemp);
				break;
			}
		case TN_HalftoneHints:
			break;
		case TN_TileWidth:
			break;
		case TN_TileHeight:
			break;
		case TN_TileOffsets:
			break;
		case TN_TileByteCounts:
			break;
		case TN_Inkset:
			break;
		case TN_InkNames:
			break;
		case TN_NumberOfInks:
			break;
		case TN_DotRange:
			break;
		case TN_TargetPrinter:
			break;
		case TN_ExtraSamples:
			break;
		case TN_SampleFormat:
			break;
		case TN_SMinSampleValue:
			break;
		case TN_SMaxSampleValue:
			break;
		case TN_JPEGProc:
			break;
		case TN_JPEGInterchangeFormat:
			JPEGIFOffset = TIFT.ValOff.dwValue; //add by yaka 20010524
			break;
		case TN_JPEGInterchangeFormatLngth:
			break;
		case TN_JPEGRestartInterval:
			break;
		case TN_JPEGLosslessPredictors:
			break;
		case TN_JPEGPointTransforms:
			break;
		case TN_JPEGQTables:
			break;
		case TN_JPEGDCTables:
			break;
		case TN_JPEGACTables:
			break;
		case TN_YCbCrCoefficients:
			break;
		case TN_YCbCrSubSampling:
			break;
		case TN_YCbCrPositioning:
			break;
		case TN_ReferenceBlackWhite:
			break;
		case TN_Copyright:
			{
				HGLOBAL hcopyright = GlobalAlloc(GHND,(DWORD)CopyRightSize);
				LPSTR lpcopyright = (LPSTR)GlobalLock(hcopyright);
				m_SrcFile->Seek(TIFT.ValOff.dwOffset,CFile::begin);
				m_SrcFile->Read(lpcopyright,CopyRightSize);
				TIFDVar.lpCopyRight = lpcopyright;
				if(!strcmp(lpcopyright,"SUNYARD_ZYZ") || !strcmp(lpcopyright,"JetSoft_Yaka"))
				{
					TIFDVar.IsJetSoft = TRUE;
					TIFDVar.wResolutionUnit = 2; //add by yaka 20010710
				}
				break;
			}
		default:
			;//AfxMessageBox("����ʶ���ֱ���ֶ�!");
			break;
		}
	}
	/*if(!TIFDVar.IsJetSoft)
		TIFDVar.bVerMirror = TRUE;*/
	TIFH.lDirOffset = *(LPDWORD)lpTagBuff;
	GlobalUnlock(hTagBuff);
	GlobalFree(hTagBuff);

	if(TIFH.lDirOffset)
		AfxMessageBox("���TIFF�ļ�������������ͼ��,�ݲ�֧��!");
	//add by yaka 20010524
	if (TIFDVar.wCompress == CN_JPEG) 
	{
		if(TIFDVar.hStripOffsets)
			GlobalFree(TIFDVar.hStripOffsets);
		if(TIFDVar.hStripByteCount)
			GlobalFree(TIFDVar.hStripByteCount);
		m_SrcFile->Close();

		CImportJPG m_ImportJpg;
		HDIB hOutTiff = m_ImportJpg.iTmportJPGDecode((char*)lpszSrcFName,JPEGIFOffset);
		LPSTR lpInImage = (LPSTR)::GlobalLock(hOutTiff);
		if (lpInImage == NULL)
		return NULL;
		LPBITMAPINFO m_lpBitMapInfo = (LPBITMAPINFO)lpInImage;
		switch(TIFDVar.wResolutionUnit)
		{
		case 2:	m_lpBitMapInfo->bmiHeader.biXPelsPerMeter = (LONG)((TIFDVar.dwxResolution/25.4)*1000 + 0.5);
				m_lpBitMapInfo->bmiHeader.biYPelsPerMeter = (LONG)((TIFDVar.dwyResolution/25.4)*1000 + 0.5);break;
		case 3: m_lpBitMapInfo->bmiHeader.biXPelsPerMeter = (LONG)TIFDVar.dwxResolution*100;
				m_lpBitMapInfo->bmiHeader.biYPelsPerMeter = (LONG)TIFDVar.dwyResolution*100;break;
		default:m_lpBitMapInfo->bmiHeader.biXPelsPerMeter = 0;
				m_lpBitMapInfo->bmiHeader.biYPelsPerMeter = 0;
		}
		::GlobalUnlock(hOutTiff);
		return hOutTiff;
	}
	//add end

	if((TIFDVar.wPlannarConfig != 1) || (TIFDVar.wPredictor != 1) )//|| (TIFDVar.wCompress == CN_Group3Fax)
	{
		if(TIFDVar.hStripOffsets)
			GlobalFree(TIFDVar.hStripOffsets);
		if(TIFDVar.hStripByteCount)
			GlobalFree(TIFDVar.hStripByteCount);
		m_SrcFile->Close();
		AfxMessageBox("�ݲ�֧������TIFF�ļ���ʽ!");
		iReturn = NoSupport_TIF_FORMAT;
		return NULL;
	}
	if((TIFDVar.wCompress == CN_Group3Fax) || (TIFDVar.wCompress == CN_Group4Fax))
	{
		if((TIFDVar.dwGroupOption & (DWORD)0x02) != 0){
			AfxMessageBox("This TIFF format is not supported!",NULL,MB_OK);
			iReturn = NoSupport_TIF_FORMAT;
			return NULL;
		}
	}
	if(!TIFDVar.hStripByteCount && (TIFDVar.wCompress != CN_Uncompressed))
	{
		if(TIFDVar.hStripOffsets)
			GlobalFree(TIFDVar.hStripOffsets);
		m_SrcFile->Close();
		AfxMessageBox("�ݲ�֧������TIFF�ļ���ʽ!");
		iReturn = NoSupport_TIF_FORMAT;
		return NULL;
	}

	m_bGray = ((TIFDVar.wPhotoMet < 2) ? TRUE:FALSE);//��gbGray��gbB_W�����ֺڰס��ҶȺͲ�ɫ.
	m_bB_W = ((TIFDVar.wSrcBits == 1) ? TRUE:FALSE);
	TIFDVar.wNewBits = TIFDVar.wSrcBits * TIFDVar.wSamples;
	if( TIFDVar.wNewBits == 15 )
		TIFDVar.wNewBits = 16;
	TIFDVar.wLineBytes = (WORD)BYTE_WBYTES(TIFDVar.dwWidth*(DWORD)TIFDVar.wNewBits);
	
	// ** ��ֹTIFF�ļ���ʶ�˸����ݴ����ļ��е�λ��,����û�б�ʶ�����ݴ��Ĵ�С,���������Լ�����������ݴ��Ĵ�С,���ŵ�TIFDVar��.
	if(TIFDVar.hStripOffsets && !TIFDVar.hStripByteCount)
	{
		TIFDVar.wByteCntLen = TIFDVar.wOffsetsLen;
		TIFDVar.wByteCntType = TIFDVar.wOffsetsType;
		wTemp = wDTN_Len[TIFDVar.wByteCntType] * TIFDVar.wByteCntLen;
		TIFDVar.hStripByteCount = GlobalAlloc(GHND,(DWORD)wTemp);
		lpTemp = (char*)GlobalLock(TIFDVar.hStripByteCount);

		if((TIFDVar.wByteCntType == DTN_LONG) || (TIFDVar.wByteCntType == DTN_SLONG))
		{
			dwTemp = (DWORD)TIFDVar.wLineBytes * TIFDVar.dwDepth;
			lpdwTemp0 = (LPDWORD)GlobalLock(TIFDVar.hStripOffsets);
			lpdwTemp1 = (LPDWORD)GlobalLock(TIFDVar.hStripByteCount);
			for(wi=1;wi<TIFDVar.wByteCntLen;wi++)
			{
				*lpdwTemp1 = *(lpdwTemp0 + 1) - *lpdwTemp0;
				dwTemp -= *lpdwTemp1;
				lpdwTemp0++;
				lpdwTemp1++;
			}
			*lpdwTemp1 = dwTemp;
		}
		else
		{
			dwTemp = (DWORD)TIFDVar.wLineBytes * TIFDVar.dwDepth;
			lpwTemp0 = (LPWORD)GlobalLock(TIFDVar.hStripOffsets);
			lpwTemp1 = (LPWORD)GlobalLock(TIFDVar.hStripByteCount);
			for(wi=1;wi<TIFDVar.wByteCntLen;wi++)
			{
				*lpwTemp1 = *(lpwTemp0 + 1) - *lpwTemp0;
				dwTemp -= (DWORD)(*lpwTemp1 & 0XFFFF);
				lpwTemp0++;
				lpwTemp1++;
			}
			*lpwTemp1 = (WORD)dwTemp;
		}
		GlobalUnlock(TIFDVar.hStripOffsets);
		GlobalUnlock(TIFDVar.hStripByteCount);
	}

// ������TIFF�ļ���ʶ��Ϣ���Ķ�ȡ�ͷ�֧�ֱ���ֶε��жϵ�,��������ȷ��TIFF�ļ�����������(������TIFDVar�ṹ��).
//************************************************************************************************************

	wWidthBytes = (WORD)DWORD_WBYTES(TIFDVar.dwWidth * (DWORD)TIFDVar.wNewBits);
	wColors = IMAGE_COLORS(TIFDVar.wNewBits,1);
	wPalSize = wColors *sizeof(RGBQUAD);
	wBI_Size = sizeof(BITMAPINFOHEADER) + wPalSize;
	hBI_Struct = GlobalAlloc(GHND,(DWORD)wBI_Size);// ��ָ��BITMAPINFOHEADR������Color Table(40���ֽڼ�����ɫ���С);
	lpBIH = (LPBITMAPINFOHEADER)GlobalLock(hBI_Struct);
	lpBI = (LPBITMAPINFO)lpBIH;

	// ** Copy bitmap information header structure;
	lpBIH->biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	lpBIH->biWidth = TIFDVar.dwWidth;
	lpBIH->biHeight = TIFDVar.dwDepth;
	lpBIH->biPlanes = 1;
	lpBIH->biBitCount = TIFDVar.wNewBits;
	lpBIH->biCompression = BI_RGB;
	switch(TIFDVar.wResolutionUnit)
	{
	case 2:	lpBIH->biXPelsPerMeter = (LONG)((TIFDVar.dwxResolution/25.4)*1000 + 0.5);
			lpBIH->biYPelsPerMeter = (LONG)((TIFDVar.dwyResolution/25.4)*1000 + 0.5);break;
	case 3: lpBIH->biXPelsPerMeter = (LONG)TIFDVar.dwxResolution*100;
		    lpBIH->biYPelsPerMeter = (LONG)TIFDVar.dwyResolution*100;break;
	default:lpBIH->biXPelsPerMeter = 0;
			lpBIH->biYPelsPerMeter = 0;
	}
	lpBIH->biClrUsed = wColors;
	lpBIH->biClrImportant = 0;
	lpBIH->biSizeImage = (DWORD)wWidthBytes * lpBIH->biHeight;
	g_ImageCount = 1;
	g_ImageFormat = TIF;
	// ** Set palette;(�����16λλ���24λλ��!)
	if(TIFDVar.bPalette) //��԰�����ɫ��Ĳ�ɫͼ��!
	{
		wTemp = 0;
		for(wi=0;wi<wColors;wi++)
		{
			lpBI->bmiColors[wi].rgbRed = byTIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbGreen = byTIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbBlue = byTIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbReserved = 0X00;
		}
	}
	else //��԰�����ɫ��ĺڰ׺ͻҶ�ͼ��(BMP�ļ����ںڰ׺ͻҶ�ͼ��ҲҪ��ɫ��)!
	{
		switch(wColors)
		{
		case 2:
			lpBI->bmiColors[0].rgbReserved  = 0X00;
			lpBI->bmiColors[1].rgbReserved  = 0X00;
			if(TIFDVar.wPhotoMet == 0){
				lpBI->bmiColors[0].rgbRed=lpBI->bmiColors[0].rgbBlue=lpBI->bmiColors[0].rgbGreen=0XFF;
				lpBI->bmiColors[1].rgbRed=lpBI->bmiColors[1].rgbBlue=lpBI->bmiColors[1].rgbGreen=0X00;
			}
			else {//BlackIsZero����ϰ�ߡ�
				lpBI->bmiColors[0].rgbRed=lpBI->bmiColors[0].rgbBlue=lpBI->bmiColors[0].rgbGreen=0X00;
				lpBI->bmiColors[1].rgbRed=lpBI->bmiColors[1].rgbBlue=lpBI->bmiColors[1].rgbGreen=0XFF;
			}
			break;
		case 16:
			{
				if(m_bGray)
				{
					// ** Gray Palette
					for(wi=0;wi<wColors;wi++)
					{
						byTemp = (BYTE)wi;
						lpBI->bmiColors[wi].rgbRed = byTemp;
						lpBI->bmiColors[wi].rgbGreen = byTemp;
						lpBI->bmiColors[wi].rgbBlue = byTemp;
						lpBI->bmiColors[wi].rgbReserved = 0X00;
					}
				}
				else
				{
					// ** Color palette
					wTemp = 0;
					for(wi=0;wi<wColors;wi++)
					{
						lpBI->bmiColors[wi].rgbRed = bySysPal16[wTemp++];
						lpBI->bmiColors[wi].rgbGreen = bySysPal16[wTemp++];
						lpBI->bmiColors[wi].rgbBlue = bySysPal16[wTemp++];
						lpBI->bmiColors[wi].rgbReserved= 0X00;
					}
				}
				break;
			}
		case 256:
			{
				if(m_bGray)
				{
					for(wi=0;wi<wColors;wi++)
					{
						byTemp = (BYTE)wi;
						lpBI->bmiColors[wi].rgbRed = byTemp;
						lpBI->bmiColors[wi].rgbGreen = byTemp;
						lpBI->bmiColors[wi].rgbBlue = byTemp;
						lpBI->bmiColors[wi].rgbReserved = 0X00;
					}
				}
				else
				{
					hDC = GetDC(NULL);//ghParentWnd,::GetActiveWindow()
					GetPaletteEntries((HPALETTE)hDC,0,256,(LPPALETTEENTRY)SysPal256);
					for(wi=0;wi<wColors;wi++)
					{
						lpBI->bmiColors[wi].rgbRed = SysPal256[wi].peRed;
						lpBI->bmiColors[wi].rgbGreen= SysPal256[wi].peGreen;
						lpBI->bmiColors[wi].rgbBlue = SysPal256[wi].peBlue;
						lpBI->bmiColors[wi].rgbReserved = 0X00;
					}
					ReleaseDC(NULL,hDC);
				}
				break;
			}
		default: 
			break;
		}// end switch
	}//  end else

	/*ע�⣺�ش��޸ģ�������(wPhotoMet == 1)�ó�(wPhotoMet == 0)��
	�Ӷ��޸��˶�WHITE_IS_ZERO��BLACK_IS_ZERO����⡣*/

// ************************************************************************************************************
	/*if(!m_TempFile->Open(lpszDstFName,CFile::modeCreate | CFile::modeWrite,&m_error))
	{
		m_error.ReportError();	
		m_SrcFile->Close();
		return NULL;
	}*/

	HDIB hOutTiff;LPBYTE pOutTiff;
	hOutTiff = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lpBI->bmiHeader.biSizeImage+wBI_Size);
	if (hOutTiff== NULL)
	{
		iReturn = GLOBALLOCKERROR;
		return NULL;
	}
	pOutTiff = (LPBYTE) ::GlobalLock((HGLOBAL) hOutTiff);	
	memcpy(pOutTiff,lpBI,wBI_Size);
	GlobalUnlock(hBI_Struct);
	GlobalFree(hBI_Struct);

    if(iReturn = iCreateBMPFile(m_SrcFile,pOutTiff+wBI_Size,&TIFDVar, wWidthBytes))
	{
		m_SrcFile->Close();
		::GlobalUnlock((HGLOBAL) hOutTiff);
		::GlobalFree((HGLOBAL) hOutTiff);
		return NULL;
	}
	if(hcopyright !=NULL)
	{	GlobalUnlock(hcopyright);
		GlobalFree(hcopyright);
	}
	if(TIFDVar.hStripOffsets)
		GlobalFree(TIFDVar.hStripOffsets);
	if(TIFDVar.hStripByteCount)
		GlobalFree(TIFDVar.hStripByteCount);

	// ** ��ѹ�Ǳ����ѹ����TIFFͼ���轫�õ���BMPͼ��ֱ�ߵ���
	/*if(!TIFDVar.IsJetSoft)
	{
		m_TempFile->Close();
		m_TempFile->Open(lpszDstFName,CFile::modeReadWrite);
		if(m_utility.Ver_Reverse(m_TempFile,lpBIH, BFH))
			AfxMessageBox("�ڴ治����ͼ��ֱ��תʧ��!");
	}*/
	m_SrcFile->Close();
	delete m_SrcFile;
	::GlobalUnlock((HGLOBAL) hOutTiff);
	return hOutTiff;
}
// ************************************************************************************************************



// ��������	: ��ȡTIFFͷ��
// ����		: CFile *m_SrcFile
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 

int CImportTIF::iReadTiffHeader(CFile* m_SrcFile)
{
	m_SrcFile->Read(&TIFH,sizeof(TIFH));
	if(!ISTIF_INTEL(TIFH.wImageType) && !ISTIF_MOTOROLA(TIFH.wImageType))
	{
		m_SrcFile->Close();
		AfxMessageBox("���ļ�����TIFF�ļ�!");
		iReturn = NOT_TIFF;
		return 1;
	}
	TIFDVar.bINTEL = (ISTIF_INTEL(TIFH.wImageType) ? TRUE : FALSE);
	if(!TIFDVar.bINTEL)
	{
		m_utility.vSwapByte((LPSTR)&TIFH.wVersion,1);
		m_utility.vSwapWord((LPSTR) & TIFH.lDirOffset,1);
	}
	if(TIFH.wVersion != TIF_VERSION)
	{
		m_SrcFile->Close();
		AfxMessageBox("��֧������TIFF�ļ���ʽ�汾!");
		iReturn = NoSupport_TIF_VERSION;
		return 2;
	}
	return SUCCESS;
}

// ��������	: ����HDIB�ļ�
// ����		: CFile *m_SrcFile, LPBYTE lpOUTTiff, LPTIFD_VAR lpTIFDVar, WORD wWidthBytes
//				Դ�ļ�            ͼ����			GIF�ṹ					���ֽ���
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CImportTIF::iCreateBMPFile(CFile *m_SrcFile, LPBYTE pOutTiff, LPTIFD_VAR lpTIFDVar, WORD wWidthBytes)
{
	HANDLE hSrcBuff , hImage;
	LPSTR lpImage;
	DWORD  dwDataNdx, dwRowNum, dwRowCnt,dwi;
	WORD wi;
	if((lpTIFDVar->wCompress != CN_Group4Fax) && (lpTIFDVar->wCompress != CN_CCITT1D) && (lpTIFDVar->wCompress != CN_Group3Fax))
	{
		hSrcBuff = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
		lpTIFDVar->lpDataBuff = (char*)GlobalLock(hSrcBuff);
	}
	if(lpTIFDVar->wCompress == CN_LZW)
		iReturn = m_LZW.iDecodeTIF_LZW(m_SrcFile,pOutTiff,lpTIFDVar,wWidthBytes);
	else
	{
		//dwDataBgn = (DWORD)m_DstFile->Seek(0L,CFile::current);
		hImage = GlobalAlloc(GHND,(DWORD)wWidthBytes);
		lpImage = (char*)GlobalLock(hImage);
		
		// ** ��ͼ���һ������,����Ҫ��ֱ��ת,��Ӧ�ڴ�BMP�ļ�β����ʼ�洢ͼ������,
		// ** dwRowNum���涨λ�ļ�ָ�뽫�õ�.
		/*�������ã����������ļ��ķ���
		lpTIFDVar->bVerMirror = ((lpTIFDVar->bVerMirror == TRUE) ? FALSE : TRUE);*/
		dwRowNum = (lpTIFDVar->bVerMirror ? 0UL : (lpTIFDVar->dwDepth - 1UL));

		dwRowCnt = 1UL;//dwRowCnt = 1UL;
		for(wi=0;wi<lpTIFDVar->wOffsetsLen;wi++)
		{
			if(lpTIFDVar->dwRowsPerStrip > (lpTIFDVar->dwDepth - dwRowCnt))
				lpTIFDVar->dwRowsPerStrip = lpTIFDVar->dwDepth - dwRowCnt + 1;
			// ** �����潫Ҫ��ȡ��һ��TIFF���ݴ���С����lpTIFDVar��dwDataLen(�����ж���̫������MAX_BUFF_SIZEΪ����),
			// ** �����ļ��Ƶ������ݴ�λ��.
			m_LZW.vProcessPrePare(m_SrcFile,lpTIFDVar,wi);

			lpTIFDVar->bEOF = FALSE;
			//��һ�����ݴ�������,�������ݴ�����MAX_BUFF_SIZE,�������(��lpTIFDVar->bEOFΪ��־);
			if((lpTIFDVar->wCompress != CN_Group4Fax) && (lpTIFDVar->wCompress != CN_CCITT1D) && (lpTIFDVar->wCompress != CN_Group3Fax))
			{
				iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF);
				lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
				lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
			}

			if(lpTIFDVar->wCompress == CN_Group4Fax || (lpTIFDVar->wCompress == CN_CCITT1D) || (lpTIFDVar->wCompress == CN_Group3Fax))
			{
				hSrcBuff = GlobalAlloc(GHND,(DWORD)lpTIFDVar->dwDataLen);
				lpTIFDVar->lpDataBuff = (char*)GlobalLock(hSrcBuff);
				iReturn = m_utility.iReadSrcData(m_SrcFile,&lpTIFDVar->dwDataLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF);
				lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
				lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->dwDataLen;
				iReturn = m_CCITT3.iDecodeCCITTNGroup(m_SrcFile,lpTIFDVar,pOutTiff,wi,dwRowCnt,dwRowNum);
				//�������ã�Group4�Ľ����ںϵ���CCITT3���С�
				//iReturn = m_CCITT4.iDecodeGroup4(m_SrcFile,lpTIFDVar,pOutTiff,dwRowCnt,dwRowNum);
				GlobalUnlock(hSrcBuff);
				GlobalFree(hSrcBuff);
			}
			else
			{
				for(dwi=0;dwi<lpTIFDVar->dwRowsPerStrip;dwi++)
				{
					switch(lpTIFDVar->wCompress)
					{
					case CN_Uncompressed:
						iReturn = iDecodeLineTIF_RawData(m_SrcFile,lpTIFDVar,lpImage);
						break;
					case CN_PackBits:
						iReturn = m_EnPackBits.iDecodeLineTIF_PackBits(m_SrcFile,lpTIFDVar,lpImage);
						if(lpTIFDVar->wNewBits == 24 && 
							(!strcmp((const char*)lpTIFDVar->lpCopyRight,"SUNYARD_ZYZ")||!strcmp((const char*)lpTIFDVar->lpCopyRight,"JetSoft_Yaka")))
							iReturn = m_utility.iDeArrangeRGB(lpTIFDVar->wLineBytes, lpImage);
						break;
					case CN_JPEG:
						;//
						break;
					default:
						AfxMessageBox("�ݲ�֧������TIFF�ļ���ʽ!");
						iReturn = NoSupport_TIF_FORMAT;
					}
					if(iReturn)
					{
						GlobalUnlock(hImage);
						GlobalFree(hImage);
						GlobalUnlock(hSrcBuff);
						GlobalFree(hSrcBuff);
						return iReturn;
					}
					// **��ͼ���һ������,����ˮƽ��ת;
					if(lpTIFDVar->bHorMirror)
						m_utility.vSuperInvertLine(lpImage,(WORD)lpTIFDVar->wLineBytes,lpTIFDVar->wNewBits);
					
					dwDataNdx = dwRowNum * (DWORD)wWidthBytes;
					memcpy(pOutTiff+dwDataNdx,lpImage,wWidthBytes);
					dwRowCnt++;
					// ** ��ͼ���һ������,���BMP�ļ�β�����Ŵ洢.
					(lpTIFDVar->bVerMirror ? dwRowNum++ : dwRowNum--);
				}//end of for
			}// end of else
			if(iReturn)
				break;
		}// end of for
		GlobalUnlock(hImage);
		GlobalFree(hImage);
	}
	if((lpTIFDVar->wCompress != CN_Group4Fax) && (lpTIFDVar->wCompress != CN_CCITT1D) && (lpTIFDVar->wCompress != CN_Group3Fax))
	{
		GlobalUnlock(hSrcBuff);
		GlobalFree(hSrcBuff);
	}
	return iReturn;
}


// ��������	: ����δѹ����TIFFͼ������;
// ����		: CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, LPSTR lpImage
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CImportTIF::iDecodeLineTIF_RawData(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, LPSTR lpImage)
{
	WORD wBack;
	WORD wi;
	if((lpTIFDVar->lpBgnBuff + lpTIFDVar->wLineBytes) > lpTIFDVar->lpEndBuff)
	{
		wBack = (WORD)(lpTIFDVar->lpEndBuff - lpTIFDVar->lpBgnBuff);
		m_SrcFile->Seek(-(LONG)wBack,CFile::current);
		lpTIFDVar->dwDataLen += (DWORD)wBack;
		if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
			return iReturn;
		lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
		lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
	}

	memcpy(lpImage,lpTIFDVar->lpBgnBuff,lpTIFDVar->wLineBytes);
	lpTIFDVar->lpBgnBuff += lpTIFDVar->wLineBytes;
	if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
	{
		if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen,&lpTIFDVar->dwDataLen,lpTIFDVar->lpDataBuff,&lpTIFDVar->bEOF))
			return iReturn;
		lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
		lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
	}
	if(lpTIFDVar->wNewBits == 24 )//|| lpTIFDVar->wNewBits == 16
	{
		for(wi=0;wi<lpTIFDVar->wLineBytes;wi+=3)
		{
			SWAP(*lpImage, *(lpImage + 2));
			lpImage += 3;
		}
	}
	return SUCCESS;
}


