//mcsjpeg.h
//
// 文件名	: MCSjpeg.H
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理JPEG的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 

#define DCT_SIZE			8 /*The basic DCT block is 8*8 samples*/
#define DCT_SQUARE			64 /*DCT_SIZE squared*/
#define NUM_QUANT_TABLES	4/*Number of Quantization tables in baseline*/
#define NUM_HUFF_TABLES		4/*Number of Huffman tables in baseline */
#define MAX_COMPS_IN_SCAN	4/*JPEG limit on # of component in one scan */
#define MAX_SAMPLE_FACTOR	4/*JPEG limit on sampling factors */
#define MAX_BLOCKS_IN_MCU	10/*JPEG limit on # of blocks in an MCU */

typedef enum YUV
{
	YUV_444,	
	YUV_422,
	YUV_411
} YUV;

typedef enum COLORSYSTEM
{
	CS_UNKNOWN, /* Error/Unspecifed  */
	CS_GRAY,	/*Monochrome(only 1 component) */
	CS_RGB,		/*Red/Blue/Green			   */
	CS_YCbCr,	/*Y/cB/Cr (also known as YUV)  */
	CS_YIQ,		/*Y/I/Q						   */
	CS_CMYK		/* C/M/Y/K					   */
} COLORSYSTEM;

typedef enum JPEGMARKER
{
	/* Non_hierarchical Huffman coding */
	M_SOF0 = (BYTE)0xC0, /* Baseline DCT */
	M_SOF1 = (BYTE)0xC1, /* Extended sequential DCT */
	M_SOF2 = (BYTE)0xC2, /* Progressive DCT */
	M_SOF3 = (BYTE)0xC3, /* Lossless(sequential) */
	
	/* Hierarchical Huffman coding */
	M_SOF5 = (BYTE)0xC5, /* Differential sequential DCT */
	M_SOF6 = (BYTE)0xC6, /* Differential Progressive DCT */
	M_SOF7 = (BYTE)0xC7, /* Differential Lossless  */
	
	/* Non_hierarchical arithmetic coding */
	M_JPEG = (BYTE)0xC8, /*Reserved for JPEG extensions */
	M_SOF9 = (BYTE)0xC9, /* Extended sequential DCT */
	M_SOF10 = (BYTE)0xCA, /*  Progressive DCT */
	M_SOF11 = (BYTE)0xCB, /* Lossless(sequential)  */

	/* Hierarchical arithmetic coding */
	M_SOF13 = (BYTE)0xCD, /* Differential sequential DCT */
	M_SOF14 = (BYTE)0xCE, /* Differential Progressive DCT */
	M_SOF15 = (BYTE)0xCF, /* Differential Lossless  */

	/* Huffman table specification  */
	M_DHT = (BYTE) 0xC4, /*DHT_Define Huffman table(s) */
	/* Arithmetic coding conditioning specification */
	M_DAC = (BYTE)0xCC, /* DAC- Define arithmetic conditioning table(s) */

	/* Restart interval termination */
	M_RST0 = (BYTE)0xD0, /* RSTm-Restart with modulo 8 counter m */
	M_RST1 = (BYTE)0xD1, 
	M_RST2 = (BYTE)0xD2, 
	M_RST3 = (BYTE)0xD3, 
	M_RST4 = (BYTE)0xD4, 
	M_RST5 = (BYTE)0xD5, 
	M_RST6 = (BYTE)0xD6, 
	M_RST7 = (BYTE)0xD7, 

	/* Other markers */
	M_SOI = (BYTE)0xD8, /* SOI-Start of image */
	M_EOI = (BYTE)0xD9, /*EOI-End of image  */
	M_SOS = (BYTE)0xDA, /*SOS-Start of scan  */
	M_DQT = (BYTE)0xDB, /*DQT-Define quantization table(s)  */
	M_DNL = (BYTE)0xDC, /*DNL-Define number of lines*/
	M_DRI = (BYTE)0xDD, /*DRI-Define restart interval  */
	M_DHP = (BYTE)0xDE, /*DHP-Define hierarchical progression  */
	M_EXP = (BYTE)0xDF, /* EXP-Expand reference image */
	M_APP0 = (BYTE)0xE0, /*Reserved for JPEG extension  */
	M_APP1 = (BYTE)0xE1,
	M_APP2 = (BYTE)0xE2,
	M_JPG0 = (BYTE)0xF0, /*  */
	M_JPGn = (BYTE)0xFD, /*Resered for jpeg extension */
	M_COM = (BYTE)0xFE, /* Comment  */

	/*Reserved markers */
	TEM = (BYTE)0x01 /*For temporary use in arithmetic coding */
} JPEGMARKER;

typedef short int MYWORD; // 应该占用两个字节;
typedef MYWORD* LPMYWORD;
typedef unsigned short UNMYWORD;
/* JPEG Quantization table */
typedef MYWORD JPG_QUANT_TBL[DCT_SQUARE];//自定义整形数组类型；
typedef LPMYWORD LPJPG_QUANT_TBL;

/* JPEG Block Table */
typedef MYWORD JPG_BLOCK[DCT_SQUARE];
typedef LPMYWORD LPJPG_BLOCK;
typedef JPG_BLOCK FAR * LPJPG_BLOCKROW;

/*JPEG component information */
typedef struct JPG_COMP_INFO
{
	/*These values are fixed over the whole image;*/
	/* For compression,they must be supplied by the user interface;*/
	/* For decompression,they are read from the SOF marker;*/
	WORD wComponent_ID; /* Identifer for this component(0..255) */
	WORD wComponent_Index; /*Its index in SOF or cinfo->comp_info[] */
	WORD wHor_SampleFactor;/* Horizontal sampling factor(1..4) */
	WORD wVer_SampleFactor;/* Vertical sampling factor(1..4) */
	WORD wQuant_TBL_No;    /* Quantization table seletor(0..3) */

	/* These values may vary between scans;*/
	/* For compression ,they must be supplied by user interface;*/
	/* For decompression,they are read from the SOS marker.*/
	WORD wDC_TBL_No; /* DC entropy table selector(0..3) */
	WORD wAC_TBL_No; /* AC entropy table selector(0..3) */
	
	/* These values are computed before starting a scan of the component */
	WORD wMCU_Width; /* Number of blocks per MCU, horizontally */
	WORD wMCU_Depth; /* Number of blocks per MCU, vertically */
	WORD wMCU_Blocks; /* MCU_width * MCU_height */
	WORD wTrue_Comp_Width; /* Component's image width in samples */
	WORD wTrue_Comp_Depth; /* Component's image depth in samples */
	/* the above are the logical dimensions of the subsampled image */
	WORD wSubsampled_Width; /* Image width in samples, after expansion */
	WORD wSubsampled_Depth; /* Image depth in samples,after expansion */

	/* the above are the true_comp_xxx values rounded up to multiples of*/
	/* the MCU dimensions; these are the working dimensions of the array */
	/* as it is passed through the DCT or IDCT step. NOTE:these values */
	/* differ depending on whether the component is interleaved or not !! */
} JPG_COMP_INFO;

typedef JPG_COMP_INFO FAR* LPJPG_COMP_INFO;

typedef struct JPGD_HUFF_TBL
{
	/* These values directly represent the contents of a JPEG DHT marker */
	BYTE byBits[17];/*bits[k] = # of symbols with codes of length k bits; bits[0] is unused */
	BYTE byHuffVal[256];/* The symbols, in order of incr code length */
	/* Decoding tables:(element[0] of each array is unused)*/
	WORD wMinCode[17]; /* smallest code of length k*/
	LONG lMaxCode[18]; /*largest code of length k(-1 if none)*/
	/* (maxcode[17] is a sentinel to ensure huff_DECODE terminates) */
	MYWORD iValPtr[17]; /* huffval[] index of 1st symbol of length k */
} JPGD_HUFF_TBL;
typedef JPGD_HUFF_TBL FAR * LPJPGD_HUFF_TBL;

/* JPEG decode variables */
typedef struct JPGD_VAR
{
	COLORSYSTEM ColorSystem;
	JPG_COMP_INFO Component[MAX_COMPS_IN_SCAN];
	JPG_QUANT_TBL QuantTBL[NUM_QUANT_TABLES];
	HANDLE hDC_HuffTBL[NUM_HUFF_TABLES];
	HANDLE hAC_HuffTBL[NUM_HUFF_TABLES];
	LPSTR lpBgnBuff;
	LPSTR lpEndBuff;
	LPSTR lpDataBuff;
	DWORD dwDataLen;
	WORD wMemLen;
	WORD wComps_SOF;
	WORD wComps_SOS;
	WORD wWidth;
	WORD wDepth;
	WORD wXDensity;
	WORD wYDensity;
	WORD wMaxHor_SampleFactor;
	WORD wMaxVer_SampleFactor;
	WORD wW_MCUs;
	WORD wH_MCUs;
	WORD wBlocks_in_MCU;
	WORD wBlock_Mapping_Comp[MAX_BLOCKS_IN_MCU];
	WORD wRestart_Interval;
	WORD wRestart_Cnt;
	WORD wRestart_Num;
	WORD Is16;
	WORD IsJetSoft;
	BOOL bEOF;
	LONG lHuffBuff;
	BYTE byDensityUnit;
	int iHuffBits;
	MYWORD iPrev_DC_Val[MAX_COMPS_IN_SCAN];
} JPGD_VAR;
typedef JPGD_VAR FAR* LPJPGD_VAR;


/* JPEG Huffman encode information */
typedef struct JPGC_HUFF_TBL
{
	/* These values directly represent the contents of a JPEG DHT marker */
	BYTE byBits[17]; //JPEG定义的最大码长为16，所以下标范围为0-16；该数组定义了每个n位码长的个数
	BYTE byHuffVal[256];//因为有多个码的长度会相同，所以可以表示多于16的symbol
	/* encode tables; */
	WORD wHuffCode[256]; /*code for each symbol */
	WORD wHuffSize[256]; /* length of code for each symbol */
} JPGC_HUFF_TBL;
typedef JPGC_HUFF_TBL FAR * LPJPGC_HUFF_TBL;


/*JPEG Encode variables */
typedef struct JPEGC_VAR
{
	COLORSYSTEM ColorSystem;
	JPG_COMP_INFO Component[MAX_COMPS_IN_SCAN];
	JPG_QUANT_TBL QuantTBL[NUM_QUANT_TABLES];
	HANDLE hDC_HuffTBL[NUM_HUFF_TABLES];//指向DC的HUFF表内存（在baseline中为4）
	HANDLE hAC_HuffTBL[NUM_HUFF_TABLES];//指向AC的HUFF表内存（在baseline中为4）
	HANDLE hEncodeBuff;
	LPSTR lpDataBuff;//和hEndBuff相对应。
	WORD wComps_SOF;
	WORD wComps_SOS;
	WORD wWidth; //图象宽度（以象素为单位）
	WORD wDepth; //图象高度（以行为单位）
	WORD wXDensity;
	WORD wYDensity;
	WORD wMaxHor_SampleFactor;
	WORD wMaxVer_SampleFactor;
	WORD wW_MCUs;
	WORD wH_MCUs;
	WORD wBlocks_in_MCU;
	WORD wBlock_Mapping_Comp[MAX_BLOCKS_IN_MCU];
	WORD wRestart_Interval;
	WORD wRestart_Cnt;
	WORD wRestart_Num;
	DWORD dwBuffCnt;
	WORD Is16;
	LONG lHuffBuff;
	BYTE byDensityUnit;
	int iHuffBits;
	MYWORD iPrev_DC_Val[MAX_COMPS_IN_SCAN];
} JPGC_VAR;
typedef JPGC_VAR FAR* LPJPGC_VAR;














