#ifndef JETIMGSEE_
#define JETIMGSEE_
// 文件名	: JetimgSeeApi.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: API函数的头件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 	: 

//图像格式
#define UNKNOW				0
#define BMP					1
#define TIF					2
#define JPG					3
#define PCX					4
#define TGA					5
#define GIF					6
#define WMF					7
#define ICO					8

//返回码
#define SUCCESS					0   //成功;
//BMP
#define OPENBMP_ERR				103 //打开代压缩BMP文件出错；
#define NOTBMP					104 //指定代压缩文件不是BMP文件；
#define READBMP_ERR				105 //读BMP文件出错；
#define NOSUPPORT_BMP			106 //BMP文件为32位真彩色，不支持；
#define BMP_HAVECOMPRESS		107 //BMP文件已经被压缩过；
//TIFF
#define READ_TIF_TO_MEMORY		201 //解码时读TIFF文件失败；
#define NOT_TIFF				202 //不是TIFF文件；
#define NoSupport_TIF_VERSION	203 //不支持的TIFF版本
#define NoSupport_TIF_FORMAT	204 //不支持的TIFF压缩格式(为专用压缩格式)
#define DERGB_AllocMemERR		205 //解码时多位面转换成单位面申请内存失败
#define DELZW_ALLOCMEMERR		206 //LZW解码申请内存出错
#define CCITT4MODE_NOT_FOUND    207
#define HUFFKEY_NOT_FOUND       208
#define CCITT4_ERROR_CODE       209
#define NOT_SUPPORT_CCITT4EXTEN 210
#define CCITT1D_ERROR_CODE      211
#define CCITT3_ERROR_CODE       212
#define JPEG_TIFF			    213
#define CREATETIF_FAILED		214 //创建TIFF文件出错；
#define CREATETIF_AllOCMEMERR	215 //创建TIFF文件时申请内存出错；
#define ENRGB_AllocMemERR		216 //编码时单位面转换成多位面申请内存失败；
#define ENLZW_ALLOCMEMERR		217 //LZW编码申请内存出错；
#define ENLZW_READBMPERR		218 //LZW编码时读文件失败；
#define DELZW_ERROR				219 
//JPEG
#define CREATEJPG_FAILED		302 // 创建JPEG文件失败;
#define NOTSUPPORT_YIQ			303 // 不支持YIQ色彩系统;
#define NOTSUPPORT_CMYK			304 // 不支持CMYK色彩系统;
#define NOSUCHSYSTEM			305 // 不存在此种色彩系统;
#define NO_EOI_JPG				306 // 找不到JPEG文件结束标志;
#define NOJPEG					307	//不是JPEG文件;
#define UNSUPPORT_JPG			308 //错误的文件格式;
#define NOTSUPPORT_16PRECISION	309 //不支持JPEG16位数据精度;
#define NOTSUPPORT_BW			310	// JPEG压缩方法不能处理黑白图象、调色板图象和32位彩色。
#define NOTSUPPORT_DCT			312  
//PCX
#define NOPCX					401	//非pcx文件
#define NOPCXPALETTE			402	//找不到调色板信息
#define CREATEPCX_FAILED		403 // 创建PCX文件失败;
#define UNSUPPORT_PCX			404
//TGA
#define UNSUPPORT_TGA			501	//不支持的TGA格式
#define CREATETGA_FAILED		502	// 创建TGA文件失败;   
//GIF
#define	NOGIF					601
#define CREATEGIF_FAILED		602	// 创建GIF文件失败;   
#define UNSUPPORT_GIF			603
//WMF
#define	NOWMF					701
#define CREATEWMF_FAILED		702	// 打开WMF文件失败;   
#define UNSUPPORT_WMF			703
//ICO
#define NOICO					801
#define CREATEICO_FAILED		802	// 打开ICO文件失败;   


#define OPENFILE_ERR			1000 //用于统一的OpenImgFile(..);
#define READSOURCEIMG_ERR		1001 //读取源文件图象数据出错;
#define GLOBALLOCKERROR			1002 

// 功能		:打开一个BMP文件
// 返回值	:成功返为打开的文件句柄，失败返回NULL，
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI OpenBmpFile(LPCTSTR p_lpszFileName,		// I 要打开的文件名(全路径)
						LPCTSTR p_lpszDesFileName); // I 要另存为的文件名,可以为空（自动识别后缀）
// 功能		:保存一个BMP文件
// 返回值	:成功返回TRUE，失败返回FALSE,
// 备注		:保留原HDIB,请自行释放
BOOL WINAPI SaveBmpFile(HDIB hDIB,				// I 要保存的文件句柄
						LPCTSTR p_lpszFileName);// I 要保存的文件名(全路径)

// 功能		:打开一个TIFF文件
// 返回值	:成功返为打开的文件句柄，失败返回NULL，
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI OpenTiffFile(LPCTSTR p_lpszFileName,		// I 要打开的文件名(全路径)
						 LPCTSTR p_lpszDesFileName);	// I 要另存为的文件名，可以为空(自动识别后缀)

// 功能		:保存一个TIFF文件
// 返回值	:成功返回TRUE，失败返回FALSE
// 备注		:保留原HDIB,请自行释放
BOOL WINAPI SaveTiffFromDib(HDIB hDIB,				// I 要保存的文件句柄
							LPCTSTR p_lpszFileName, // I 要保存的文件名(全路径)
							int p_nFormat);			// I 为采用的压缩方法：
												 /* 0、不压缩方式；
													1、CCITT Group3 1D压缩(只针对黑白)；
													2、LZW压缩；
													3、PackBits压缩方法。
													4、CCITT Group3Fax T4压缩(只针对黑白)。
													5、CCITT Group4Fax T6压缩(只针对黑白)。
													6. JPEG压缩格式	（压缩比例50）*/

// 功能		:把一张BMP文件保存为TIFF文件
// 返回值	:成功返回TRUE，失败返回FALSE
BOOL WINAPI SaveTiffFromBmp(LPCTSTR p_lpszSrcName, // I 源文件名(全路径)
							LPCTSTR p_lpszDesName, // I 目标文件名(全路径)
							int p_nFormat);		   // I 为采用的压缩方法：
// 功能		:把一张BMP文件保存为TGA文件
// 返回值	:成功返回TRUE，失败返回FALSE
BOOL WINAPI SaveTgaFromBmp(LPCTSTR p_lpszSrcName, // I 源文件名(全路径)
						   LPCTSTR p_lpszDesName, // I 目标文件名(全路径)
						   BOOL p_bRLE);		  // I 表示是否采用RLE压缩
// 功能		:保存一个TGA文件
// 返回值	:成功返回TRUE，失败返回FALSE
// 备注		:保留原HDIB,请自行释放
BOOL WINAPI SaveTgaFromDib(HDIB hDIB,				// I 要保存的文件句柄
						   LPCTSTR p_lpszFileName,  // I 要保存的文件名(全路径)
						   BOOL p_bRLE);			// I 表示是否采用RLE压缩
// 功能		:打开一个TGA文件
// 返回值	:成功返为打开的文件句柄，失败返回NULL，
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI OpenTgaFile(LPCTSTR p_lpszFileName,		// I 要打开的文件名(全路径)
						LPCTSTR p_lpszDesFileName);	// I 要另存为的文件名，可以为空(自动识别后缀)

// 功能		:打开JPEG文件
// 返回值	:成功返为打开的文件句柄，失败返回NULL，
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI OpenJpegFile(LPCTSTR p_lpszFileName,	// I 要打开的文件名(全路径)
						 LPCTSTR p_lpszDesFileName);// I 要另存为的文件名，可以为空(自动识别后缀)

// 功能		:把DIB文件格式保存为JPEG格式
// 返回值	:成功返回TRUE，失败返回FALSE
// 备注		:保留原HDIB,请自行释放
BOOL WINAPI SaveJpegFromDib(HDIB hDIB,				// I 要保存的文件句柄
							LPCTSTR p_lpszFileName, // I 要另存为的文件名
							int p_nCompFactor);		// I 压缩系数,越大则压缩程度越大,但失真也越大,建议为0-50之间

// 功能		:把一张JPEG文件保存为BMP文件
// 返回值	:成功返回TRUE，失败返回FALSE
BOOL WINAPI SaveJpegFromBmp(LPCTSTR p_lpszSrcName,	// I 要保存的文件句柄
							LPCTSTR p_lpszDesName,	// I 要另存为的文件名
							int p_nCompFactor);		// I 压缩系数,越大则压缩程度越大,但失真也越大,建议为0-50之间

// 功能		:打开一个WMF文件或者EMF文件
// 返回值	:成功返为打开的文件句柄，失败返回NULL，
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI OpenWmfFile(LPCTSTR p_lpszFileName,		// I 要打开的文件名(全路径)
						LPCTSTR p_lpszDesFileName); // I 要另存为的文件名,可以为空（自动识别后缀）

// 功能		:打开一个ICO文件或者CUR文件
// 返回值	:成功返为打开的文件句柄，失败返回NULL，
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI OpenIcoFile(LPCTSTR p_lpszFileName,		// I 要打开的文件名(全路径)
						LPCTSTR p_lpszDesFileName); // I 要另存为的文件名,可以为空（自动识别后缀）

BOOL WINAPI SavePcxFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName);
BOOL WINAPI SavePcxFromDib(HDIB hDIB,LPCTSTR p_lpszFileName);
HDIB WINAPI OpenPcxFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName);
//没有必要解释接口了吧？？和其他的一样,处理PCX文件

BOOL WINAPI SaveGifFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName);
BOOL WINAPI SaveGifFromDib(HDIB hDIB,LPCTSTR p_lpszFileName);
HDIB WINAPI OpenGifFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName);
//没有必要解释接口了吧？？和其他的一样,处理Gif文件

//功能		:保存文件，通过自动识别文件名后缀，保存为不同的格式，
//BMP,DIB-------BMP格式（不区分的大小写）
//JPEG,JPG------JPEG格式
//TIFF,TIF------TIFF格式
//PCX,DCX-------PCX格式
//TGA-----------TGA格式
//GIF-----------GIF格式
//未知用BMP代替
//返回值	:BOOL    TRUE		--成功
//					 FALSE		--失败			
BOOL WINAPI SaveFileFromBmp(LPCTSTR p_lpszSrcName, // I 源文件名(全路径)
							LPCTSTR p_lpszDesName); // I 目标文件名(全路径)

//功能		:保存文件，通过自动识别文件名后缀，保存为不同的格式，
//返回值	:BOOL    TRUE		--成功
//					 FALSE		--失败		
BOOL WINAPI SaveFileFromDib(HDIB hDIB,				// I 要保存的文件句柄
							LPCTSTR p_lpszFileName);// I 要保存的文件名(全路径)

// 功能		:打开一个图像文件,自动识别图像文件格式
// 返回值	:成功返为打开的文件句柄，失败返回NULL，
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI OpenImgFile(LPCTSTR p_lpszSrcName,	// I 要打开的文件名(全路径)
						LPCTSTR p_lpszDesName);	// I 要另存为的文件名，可以为空（自动识别后缀）

// 功能		:抠指定区域大小的图
// 返回值	:成功返回所抠区域大小图像句柄，失败返回NULL
// 备注		:返回的句柄要调用GlobalFree函数释放内存
HDIB WINAPI hGetImageBlock(HDIB hDIB,		//  I 源句柄
						   RECT rect);		//  I 所抠区域大小

//功能		:返回最后的错误值
//返回值	:int		0			--成功
//						其它		--失败
int WINAPI GetLastErrorCode();

//功能		:获取图像的数量
//返回值	:int		 图像的数量
int WINAPI GetSubImageCount();

//功能		:用于将DIB图象数据垂直反转
//返回值	:成功返回TRUE，失败返回FALSE
BOOL WINAPI ReverseImage(HDIB pHdib);		//	I 图像句柄

//功能		:用于将DIB图象数据旋转
//返回值	:旋转后的HDIB
HDIB WINAPI RotateImage(HDIB pHdib,			//	I 图像句柄
						double dAngle);		//	I 角度
					   		
//功能		:获取调色版大小
//返回值	: int	调色版大小
WORD WINAPI GetPaletteSize(HDIB hDIB);	//   I 图像句柄
						  
//功能		:获取图像格式
//返回值	:BYTE 图像格式
BYTE WINAPI GetImageFormat();


//功能		:获取图像大小
//返回值	:BOOL 是否成功
BOOL WINAPI GetImageSize(HDIB p_hDIB,			//   I  图像句柄
						 DWORD *p_pdwWidth,		//	 O	图像宽度
						 DWORD *p_pdwHeight);	//	 O	图像高度

//功能		:获取图像DPI
//返回值	:BOOL 是否成功
BOOL WINAPI GetImageDPI(HDIB p_hDIB,			//	I	图像句柄
						DWORD *p_pdwDpiX,		//	O	X坐标DPI
						DWORD *p_pdwDpiY);		//	O	Y坐标DPI

//功能		:判断是否彩色
//返回值	:BOOL 是否彩色
BOOL WINAPI ImageIsColor(HDIB p_hDIB);			//	I	图像句柄

#endif