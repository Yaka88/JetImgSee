// mcsdef.h
//
// 文件名	: mcsdef.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 公共定义，宏
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#define MAX_BUFF_SIZE	32768 //61440
#define MAX_HASH_SIZE	5051 
#define MAX_TABLE_SIZE	4096  //4K
#define MAX_STRIP_SIZE	32768 //8K *4 //modify by yaka 20010606
#define MAX_SUBBLOCK_SIZE         255

#define SHADE_NUM		2

DECLARE_HANDLE(HDIB);
//BMP palette version
#define BMP_PAL_VER		0X0300

// Header signatutes for various resource
#define TIF_INTEL				0X4949//'II'
#define TIF_MOTOROLA			0X4D4D//'MM'
#define TIF_VERSION				0X002A//42
#define MMF_MARK				0X464D//'MF'
#define BMP_MARK				0X4D42	//'BM'
#define PCX_MARK                0x0A       //add by yaka 20010530
#define JPG_SOI_MARK            0xD8FF
#define JPG_EOI_MARK            0xD9FF
#define GIF_MARK				0x4947		
#define TGA_MARK_0				0x00		//add by yaka 20010607  可能是TGA
#define WMF_MARK				0xCDD7
#define	EMF_MARK				0x0001
#define	CUR_MARK				0x00020000
#define	ICO_MARK				0x00010000
/* Macro to determine if image is a JPG */
#define CHECK_JPG_SOI(type)     ( (type)==JPG_SOI_MARK )
#define CHECK_JPG_EOI(type)     ( (type)==JPG_EOI_MARK )
#define CHECK_PCX(type)         ( (type)==PCX_MARK ) //add by yaka 20010530
#define CHECK_GIF(type)         ( (type)==GIF_MARK ) //add by yaka 20010618



// Macro to determine if image is a TIF_INTEL
#define ISTIF_INTEL(type)	((type) == TIF_INTEL)

// Macro to determine if image is a TIF_MOTOROLA
#define ISTIF_MOTOROLA(type)	((type) == TIF_MOTOROLA)

//Macro to determine if image is a MMF or BMP
#define CHECK_MMF(type)		((type) == MMF_MARK)
#define CHECK_BMP(type)		((type) == BMP_MARK)

// Macro to determine to round off the given value to the closest byte
#define DWORD_WBYTES(x)	((((x) + 31UL) >> 5) << 2)
#define WORD_WBYTES(x)	((((x) + 15 ) >> 4) << 1)
#define BYTE_WBYTES(x)	(((x) + 7UL) >> 3)

// Macro to find the image colors
#define IMAGE_COLORS(x,y)		(1 << ((x)* (y)) )

// Macro to swap two values
#define SWAP(x,y)		((x)^=(y)^=(x)^=(y))

typedef struct tagMONO_BITMAP_INFO {
	BITMAPINFOHEADER	bmiHeader;
	RGBQUAD				bmiColors[2];
} MONO_BITMAP_INFO;

#define MAX(x,y) ((x>y) ? x : y )
#define MIN(x,y) ((x<y) ? x : y )

//DECLARE_HANDLE(HDIB);
typedef BOOL FAR * LPBOOL;
const int MAX_TAG_COUNT = 15;//每个标识信息区的标签个数
const WORD CopyRightSize = 50;
typedef BOOL FAR * LPBOOL;

