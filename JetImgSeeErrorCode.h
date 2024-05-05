//ErrorCode.h
//
// 文件名	: JetimgSeeErrorcode.h
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 错误代码
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
#define SUCCESS			   0 //成功;
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
#define HUFFKEY_NOT_FOUND    208
#define CCITT4_ERROR_CODE    209
#define NOT_SUPPORT_CCITT4EXTEN    210
#define CCITT1D_ERROR_CODE     211
#define CCITT3_ERROR_CODE      212
#define JPEG_TIFF			   213
#define CREATETIF_FAILED		214 //创建TIFF文件出错；
#define CREATETIF_AllOCMEMERR	215 //创建TIFF文件时申请内存出错；
#define ENRGB_AllocMemERR		216 //编码时单位面转换成多位面申请内存失败；
#define ENLZW_ALLOCMEMERR		217 //LZW编码申请内存出错；
#define ENLZW_READBMPERR		218 //LZW编码时读文件失败；
#define DELZW_ERROR				219 
//JPEG

#define CREATEJPG_FAILED   302 // 创建JPEG文件失败;
#define NOTSUPPORT_YIQ	   303 // 不支持YIQ色彩系统;
#define NOTSUPPORT_CMYK    304 // 不支持CMYK色彩系统;
#define NOSUCHSYSTEM       305 // 不存在此种色彩系统;
#define NO_EOI_JPG         306 // 找不到JPEG文件结束标志;
#define NOJPEG			   307//不是JPEG文件;
#define UNSUPPORT_JPG     308 //错误的文件格式;
#define NOTSUPPORT_16PRECISION 309 //不支持JPEG16位数据精度;
#define NOTSUPPORT_BW  310 // JPEG压缩方法不能处理黑白图象、调色板图象和32位彩色。
#define NOTSUPPORT_DCT		312  //add by yaka 20010525
//add by yaka 20010530
#define NOPCX				401			//非pcx文件
#define NOPCXPALETTE		402			//找不到调色板信息
#define CREATEPCX_FAILED    403        // 创建PCX文件失败;
#define UNSUPPORT_PCX		404

#define UNSUPPORT_TGA		501			//不支持的TGA格式
#define CREATETGA_FAILED    502			 // 创建TGA文件失败;   

#define	NOGIF				601
#define CREATEGIF_FAILED    602			 // 创建GIF文件失败;   
#define UNSUPPORT_GIF		603

#define OPENFILE_ERR 1000//用于统一的OpenImgFile(..);
#define READSOURCEIMG_ERR  1001 //读取源文件图象数据出错;
#define GLOBALLOCKERROR   1002 // add by yaka 20010528


