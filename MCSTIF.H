// mcstif.h
// �ļ���	: MCSTIF.H
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIF��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 

enum writetagname
{
	ImageWidth,
	ImageHeight,
	BitsPerSample,
	Compression,
	Photometric,
	StripOffsets,
	SamplePerPixel,
	RowsPerStrip,
	StripByteCount,
	Xresolution,
	Yresolution,
	ResolutionUnit,
	CopyRight,
	Orientation,// �������˳���ܵ�!
	ColorMap, 
};

// ** TIFF�ļ�ͷ�ṹ;
typedef struct TIFHEADER
{
	WORD wImageType;
	WORD wVersion;
	LONG lDirOffset;
} TIFHEADER;

// ** TIFF�ļ���ʶ��Ϣ��ṹ(һ����ʶ��Ϣ�����������ʶ��Ϣ��)
typedef struct TIFFTAG
{
	WORD wTag;
	WORD wType;
	DWORD dwCount;
	union
	{
		DWORD dwOffset;
		DWORD dwValue;
		struct	value
		{
			WORD wVal1;
			WORD wVal2;
		} Value;
	} ValOff;
}  TIFFTAG;
typedef TIFFTAG FAR * LPTIFTAG;


// ** ���������ݽṹ;
typedef struct TIFD_VAR
{
	HANDLE hStripOffsets;
	HANDLE hStripByteCount;
	LPSTR  lpDataBuff;
	LPSTR  lpBgnBuff;
	LPSTR  lpEndBuff;
	LPSTR  lpCopyRight;
	DWORD  dwDataLen;
	DWORD  dwWidth;
	DWORD  dwDepth;
	DWORD  dwRowsPerStrip;
	DWORD  wMemLen;
	WORD   wLineBytes;
	WORD   wSrcBits;
	WORD   wNewBits;
	WORD   wSamples;
	WORD   wOffsetsLen;
	WORD   wByteCntLen;
	WORD   wOffsetsType;
	WORD   wByteCntType;
	WORD   wPhotoMet;
	WORD   wPlannarConfig;
	WORD   wCompress;
	WORD   wPredictor;
	WORD   WImageNums;
	BOOL   bEOF;
	BOOL   bINTEL;
	BOOL   bPalette;
	BOOL   bHorMirror;
	BOOL   bVerMirror;
	BOOL   IsJetSoft;
	WORD   FillOrder;//���ںڰ�ͼ�󣬱���һ���ֽ���8λ������˳��
	DWORD  dwGroupOption;//Group3��Group4�ı�ǩ֮һ:292��293��
	DWORD dwxResolution;
	DWORD dwyResolution;
	WORD  wResolutionUnit;

} TIFD_VAR;
typedef TIFD_VAR FAR * LPTIFD_VAR;


// ** ���������ݽṹ;
typedef struct TIF_CVAR
{
	HANDLE hStripOffsets;
	HANDLE hStripByteCount;
	LPSTR  lpDataBuff;
	LPSTR  lpEndBuff;
	LPSTR  lpCopyRight;
	DWORD  dwWidth;
	DWORD  dwDepth;
	DWORD  dwRowsPerStrip;
	DWORD  dwStripLen;
	DWORD  dwTempCode;
	DWORD  dwRowCount;
	WORD   wLineBytes;
	WORD   wBits;
	DWORD   wByteCount;
	WORD   WImageNums;
	BYTE   byLeftBits;
	int*  iLastLineOffset;
	DWORD  dwLastNumChange;
	WORD wLeftBits;//����Group4ѹ��ʱ����strip�����󣬽�С��һ���ֽڵĲ�����ֵ���̣���ΪGroup4�����λ��
	WORD wEncodeMethod;//��ʾ���õı��뷽����
	DWORD dwGroupOption;
	DWORD xResolution;
	DWORD yResolution;
} TIFC_VAR;
typedef TIFC_VAR FAR * LPTIFC_VAR;


// ** TIFF 6.0 Tag Names(73 total) 
#define TN_NewSubfileType                  254
#define TN_SubfileType					   255
#define TN_ImageWidth                      256
#define TN_ImageLength                     257
#define TN_BitsPerSample                   258
#define TN_Compression                     259
#define TN_PhotometricInterpretation       262
#define TN_Thresholding					   263	
#define TN_CellWidth					   264
#define TN_CellHeight					   265
#define TN_FillOrder                       266
#define TN_DocumentName					   269
#define TN_ImageDescription				   270
#define TN_Make							   271
#define TN_Model						   272
#define TN_StripOffsets                    273
#define TN_Orientation                     274
#define TN_SamplePerPixel                  277
#define TN_RowsPerStrip                    278
#define TN_StripByteCounts                 279
#define TN_MinsampleValue				   280
#define TN_MaxsampleValue				   281
#define TN_XResolution                     282
#define TN_YResolution                     283
#define TN_PlanarConfiguration             284
#define TN_PageName                        285
#define TN_Xposition					   286
#define TN_Yposition					   287
#define TN_FreeOffsets					   288
#define TN_FreeByteCounts				   289
#define TN_GreyResponseCurveUnit           290
#define TN_GreyResponseCurve               291
#define TN_Group3Option					   292
#define TN_Group4Option					   293
#define TN_ResolutionUnit                  296
#define TN_PageNumber					   297
#define TN_TransferFunction				   301
#define TN_SoftWare                        305
#define TN_DateTime                        306

#define TN_Artist							315
#define TN_HostComputer						316
#define TN_Predictor						317
#define TN_WhitePoint						318
#define TN_PrimaryChromaticities			319
#define TN_Colormap                         320
#define TN_HalftoneHints					321
#define TN_TileWidth						322
#define TN_TileHeight						323
#define TN_TileOffsets						324
#define TN_TileByteCounts					325
#define TN_Inkset							332
#define TN_InkNames							333
#define TN_NumberOfInks						334
#define TN_DotRange							336
#define TN_TargetPrinter					337
#define TN_ExtraSamples						338
#define TN_SampleFormat						339
#define TN_SMinSampleValue					340
#define TN_SMaxSampleValue					341

#define TN_JPEGProc							512
#define TN_JPEGInterchangeFormat			513
#define TN_JPEGInterchangeFormatLngth		514
#define TN_JPEGRestartInterval				515
#define TN_JPEGLosslessPredictors			517
#define TN_JPEGPointTransforms				518
#define TN_JPEGQTables						519
#define TN_JPEGDCTables						520
#define TN_JPEGACTables						521

#define TN_YCbCrCoefficients				529
#define TN_YCbCrSubSampling					530
#define TN_YCbCrPositioning					531
#define TN_ReferenceBlackWhite				532
#define TN_Copyright                       33432


// ** TIFF 6.0 Data Type Names
#define DTN_BYTE	1
#define DTN_ASCII	2
#define DTN_SHORT	3
#define DTN_LONG	4
#define DTN_RATIONAL 5
#define DTN_SBYTE	6
#define DTN_UNDEFINED 7
#define DTN_SSHORT	8
#define DTN_SLONG	9
#define DTN_SRATIONAL 10
#define DTN_FLOAT	11
#define DTN_DOUBLE	12

// ** TIFF 6.0 Compresssion Names 
#define CN_Uncompressed	    1
#define CN_CCITT1D			2
#define CN_Group3Fax		3
#define CN_Group4Fax		4
#define CN_LZW				5
#define CN_JPEG				6
#define CN_PackBits			32773



//CCITT two-dimensional code table mode
#define MODEPASS 0
#define MODEHOR 1
#define MODEVER0 2
#define MODEVERR1 3
#define MODEVERR2 4
#define MODEVERR3 5
#define MODEVERL1 6
#define MODEVERL2 7
#define MODEVERL3 8
#define MODEEXT2D 9
#define MODEEXT1D 10
#define PAREMETERK   2//����CCITT Group3 Fax T4 ѹ����
#define MAXCHANGE 2560//һ�кڰ׵�����仯������



// end of mcstif.h

