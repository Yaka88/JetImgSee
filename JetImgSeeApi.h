#ifndef JETIMGSEE_
#define JETIMGSEE_
// �ļ���	: JetimgSeeApi.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: API������ͷ��
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 	: 

//ͼ���ʽ
#define UNKNOW				0
#define BMP					1
#define TIF					2
#define JPG					3
#define PCX					4
#define TGA					5
#define GIF					6
#define WMF					7
#define ICO					8

//������
#define SUCCESS					0   //�ɹ�;
//BMP
#define OPENBMP_ERR				103 //�򿪴�ѹ��BMP�ļ�����
#define NOTBMP					104 //ָ����ѹ���ļ�����BMP�ļ���
#define READBMP_ERR				105 //��BMP�ļ�����
#define NOSUPPORT_BMP			106 //BMP�ļ�Ϊ32λ���ɫ����֧�֣�
#define BMP_HAVECOMPRESS		107 //BMP�ļ��Ѿ���ѹ������
//TIFF
#define READ_TIF_TO_MEMORY		201 //����ʱ��TIFF�ļ�ʧ�ܣ�
#define NOT_TIFF				202 //����TIFF�ļ���
#define NoSupport_TIF_VERSION	203 //��֧�ֵ�TIFF�汾
#define NoSupport_TIF_FORMAT	204 //��֧�ֵ�TIFFѹ����ʽ(Ϊר��ѹ����ʽ)
#define DERGB_AllocMemERR		205 //����ʱ��λ��ת���ɵ�λ�������ڴ�ʧ��
#define DELZW_ALLOCMEMERR		206 //LZW���������ڴ����
#define CCITT4MODE_NOT_FOUND    207
#define HUFFKEY_NOT_FOUND       208
#define CCITT4_ERROR_CODE       209
#define NOT_SUPPORT_CCITT4EXTEN 210
#define CCITT1D_ERROR_CODE      211
#define CCITT3_ERROR_CODE       212
#define JPEG_TIFF			    213
#define CREATETIF_FAILED		214 //����TIFF�ļ�����
#define CREATETIF_AllOCMEMERR	215 //����TIFF�ļ�ʱ�����ڴ����
#define ENRGB_AllocMemERR		216 //����ʱ��λ��ת���ɶ�λ�������ڴ�ʧ�ܣ�
#define ENLZW_ALLOCMEMERR		217 //LZW���������ڴ����
#define ENLZW_READBMPERR		218 //LZW����ʱ���ļ�ʧ�ܣ�
#define DELZW_ERROR				219 
//JPEG
#define CREATEJPG_FAILED		302 // ����JPEG�ļ�ʧ��;
#define NOTSUPPORT_YIQ			303 // ��֧��YIQɫ��ϵͳ;
#define NOTSUPPORT_CMYK			304 // ��֧��CMYKɫ��ϵͳ;
#define NOSUCHSYSTEM			305 // �����ڴ���ɫ��ϵͳ;
#define NO_EOI_JPG				306 // �Ҳ���JPEG�ļ�������־;
#define NOJPEG					307	//����JPEG�ļ�;
#define UNSUPPORT_JPG			308 //������ļ���ʽ;
#define NOTSUPPORT_16PRECISION	309 //��֧��JPEG16λ���ݾ���;
#define NOTSUPPORT_BW			310	// JPEGѹ���������ܴ���ڰ�ͼ�󡢵�ɫ��ͼ���32λ��ɫ��
#define NOTSUPPORT_DCT			312  
//PCX
#define NOPCX					401	//��pcx�ļ�
#define NOPCXPALETTE			402	//�Ҳ�����ɫ����Ϣ
#define CREATEPCX_FAILED		403 // ����PCX�ļ�ʧ��;
#define UNSUPPORT_PCX			404
//TGA
#define UNSUPPORT_TGA			501	//��֧�ֵ�TGA��ʽ
#define CREATETGA_FAILED		502	// ����TGA�ļ�ʧ��;   
//GIF
#define	NOGIF					601
#define CREATEGIF_FAILED		602	// ����GIF�ļ�ʧ��;   
#define UNSUPPORT_GIF			603
//WMF
#define	NOWMF					701
#define CREATEWMF_FAILED		702	// ��WMF�ļ�ʧ��;   
#define UNSUPPORT_WMF			703
//ICO
#define NOICO					801
#define CREATEICO_FAILED		802	// ��ICO�ļ�ʧ��;   


#define OPENFILE_ERR			1000 //����ͳһ��OpenImgFile(..);
#define READSOURCEIMG_ERR		1001 //��ȡԴ�ļ�ͼ�����ݳ���;
#define GLOBALLOCKERROR			1002 

// ����		:��һ��BMP�ļ�
// ����ֵ	:�ɹ���Ϊ�򿪵��ļ������ʧ�ܷ���NULL��
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI OpenBmpFile(LPCTSTR p_lpszFileName,		// I Ҫ�򿪵��ļ���(ȫ·��)
						LPCTSTR p_lpszDesFileName); // I Ҫ���Ϊ���ļ���,����Ϊ�գ��Զ�ʶ���׺��
// ����		:����һ��BMP�ļ�
// ����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE,
// ��ע		:����ԭHDIB,�������ͷ�
BOOL WINAPI SaveBmpFile(HDIB hDIB,				// I Ҫ������ļ����
						LPCTSTR p_lpszFileName);// I Ҫ������ļ���(ȫ·��)

// ����		:��һ��TIFF�ļ�
// ����ֵ	:�ɹ���Ϊ�򿪵��ļ������ʧ�ܷ���NULL��
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI OpenTiffFile(LPCTSTR p_lpszFileName,		// I Ҫ�򿪵��ļ���(ȫ·��)
						 LPCTSTR p_lpszDesFileName);	// I Ҫ���Ϊ���ļ���������Ϊ��(�Զ�ʶ���׺)

// ����		:����һ��TIFF�ļ�
// ����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE
// ��ע		:����ԭHDIB,�������ͷ�
BOOL WINAPI SaveTiffFromDib(HDIB hDIB,				// I Ҫ������ļ����
							LPCTSTR p_lpszFileName, // I Ҫ������ļ���(ȫ·��)
							int p_nFormat);			// I Ϊ���õ�ѹ��������
												 /* 0����ѹ����ʽ��
													1��CCITT Group3 1Dѹ��(ֻ��Ժڰ�)��
													2��LZWѹ����
													3��PackBitsѹ��������
													4��CCITT Group3Fax T4ѹ��(ֻ��Ժڰ�)��
													5��CCITT Group4Fax T6ѹ��(ֻ��Ժڰ�)��
													6. JPEGѹ����ʽ	��ѹ������50��*/

// ����		:��һ��BMP�ļ�����ΪTIFF�ļ�
// ����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL WINAPI SaveTiffFromBmp(LPCTSTR p_lpszSrcName, // I Դ�ļ���(ȫ·��)
							LPCTSTR p_lpszDesName, // I Ŀ���ļ���(ȫ·��)
							int p_nFormat);		   // I Ϊ���õ�ѹ��������
// ����		:��һ��BMP�ļ�����ΪTGA�ļ�
// ����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL WINAPI SaveTgaFromBmp(LPCTSTR p_lpszSrcName, // I Դ�ļ���(ȫ·��)
						   LPCTSTR p_lpszDesName, // I Ŀ���ļ���(ȫ·��)
						   BOOL p_bRLE);		  // I ��ʾ�Ƿ����RLEѹ��
// ����		:����һ��TGA�ļ�
// ����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE
// ��ע		:����ԭHDIB,�������ͷ�
BOOL WINAPI SaveTgaFromDib(HDIB hDIB,				// I Ҫ������ļ����
						   LPCTSTR p_lpszFileName,  // I Ҫ������ļ���(ȫ·��)
						   BOOL p_bRLE);			// I ��ʾ�Ƿ����RLEѹ��
// ����		:��һ��TGA�ļ�
// ����ֵ	:�ɹ���Ϊ�򿪵��ļ������ʧ�ܷ���NULL��
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI OpenTgaFile(LPCTSTR p_lpszFileName,		// I Ҫ�򿪵��ļ���(ȫ·��)
						LPCTSTR p_lpszDesFileName);	// I Ҫ���Ϊ���ļ���������Ϊ��(�Զ�ʶ���׺)

// ����		:��JPEG�ļ�
// ����ֵ	:�ɹ���Ϊ�򿪵��ļ������ʧ�ܷ���NULL��
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI OpenJpegFile(LPCTSTR p_lpszFileName,	// I Ҫ�򿪵��ļ���(ȫ·��)
						 LPCTSTR p_lpszDesFileName);// I Ҫ���Ϊ���ļ���������Ϊ��(�Զ�ʶ���׺)

// ����		:��DIB�ļ���ʽ����ΪJPEG��ʽ
// ����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE
// ��ע		:����ԭHDIB,�������ͷ�
BOOL WINAPI SaveJpegFromDib(HDIB hDIB,				// I Ҫ������ļ����
							LPCTSTR p_lpszFileName, // I Ҫ���Ϊ���ļ���
							int p_nCompFactor);		// I ѹ��ϵ��,Խ����ѹ���̶�Խ��,��ʧ��ҲԽ��,����Ϊ0-50֮��

// ����		:��һ��JPEG�ļ�����ΪBMP�ļ�
// ����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL WINAPI SaveJpegFromBmp(LPCTSTR p_lpszSrcName,	// I Ҫ������ļ����
							LPCTSTR p_lpszDesName,	// I Ҫ���Ϊ���ļ���
							int p_nCompFactor);		// I ѹ��ϵ��,Խ����ѹ���̶�Խ��,��ʧ��ҲԽ��,����Ϊ0-50֮��

// ����		:��һ��WMF�ļ�����EMF�ļ�
// ����ֵ	:�ɹ���Ϊ�򿪵��ļ������ʧ�ܷ���NULL��
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI OpenWmfFile(LPCTSTR p_lpszFileName,		// I Ҫ�򿪵��ļ���(ȫ·��)
						LPCTSTR p_lpszDesFileName); // I Ҫ���Ϊ���ļ���,����Ϊ�գ��Զ�ʶ���׺��

// ����		:��һ��ICO�ļ�����CUR�ļ�
// ����ֵ	:�ɹ���Ϊ�򿪵��ļ������ʧ�ܷ���NULL��
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI OpenIcoFile(LPCTSTR p_lpszFileName,		// I Ҫ�򿪵��ļ���(ȫ·��)
						LPCTSTR p_lpszDesFileName); // I Ҫ���Ϊ���ļ���,����Ϊ�գ��Զ�ʶ���׺��

BOOL WINAPI SavePcxFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName);
BOOL WINAPI SavePcxFromDib(HDIB hDIB,LPCTSTR p_lpszFileName);
HDIB WINAPI OpenPcxFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName);
//û�б�Ҫ���ͽӿ��˰ɣ�����������һ��,����PCX�ļ�

BOOL WINAPI SaveGifFromBmp(LPCTSTR p_lpszSrcName, LPCTSTR p_lpszDesName);
BOOL WINAPI SaveGifFromDib(HDIB hDIB,LPCTSTR p_lpszFileName);
HDIB WINAPI OpenGifFile(LPCTSTR p_lpszFileName,LPCTSTR p_lpszDesFileName);
//û�б�Ҫ���ͽӿ��˰ɣ�����������һ��,����Gif�ļ�

//����		:�����ļ���ͨ���Զ�ʶ���ļ�����׺������Ϊ��ͬ�ĸ�ʽ��
//BMP,DIB-------BMP��ʽ�������ֵĴ�Сд��
//JPEG,JPG------JPEG��ʽ
//TIFF,TIF------TIFF��ʽ
//PCX,DCX-------PCX��ʽ
//TGA-----------TGA��ʽ
//GIF-----------GIF��ʽ
//δ֪��BMP����
//����ֵ	:BOOL    TRUE		--�ɹ�
//					 FALSE		--ʧ��			
BOOL WINAPI SaveFileFromBmp(LPCTSTR p_lpszSrcName, // I Դ�ļ���(ȫ·��)
							LPCTSTR p_lpszDesName); // I Ŀ���ļ���(ȫ·��)

//����		:�����ļ���ͨ���Զ�ʶ���ļ�����׺������Ϊ��ͬ�ĸ�ʽ��
//����ֵ	:BOOL    TRUE		--�ɹ�
//					 FALSE		--ʧ��		
BOOL WINAPI SaveFileFromDib(HDIB hDIB,				// I Ҫ������ļ����
							LPCTSTR p_lpszFileName);// I Ҫ������ļ���(ȫ·��)

// ����		:��һ��ͼ���ļ�,�Զ�ʶ��ͼ���ļ���ʽ
// ����ֵ	:�ɹ���Ϊ�򿪵��ļ������ʧ�ܷ���NULL��
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI OpenImgFile(LPCTSTR p_lpszSrcName,	// I Ҫ�򿪵��ļ���(ȫ·��)
						LPCTSTR p_lpszDesName);	// I Ҫ���Ϊ���ļ���������Ϊ�գ��Զ�ʶ���׺��

// ����		:��ָ�������С��ͼ
// ����ֵ	:�ɹ��������������Сͼ������ʧ�ܷ���NULL
// ��ע		:���صľ��Ҫ����GlobalFree�����ͷ��ڴ�
HDIB WINAPI hGetImageBlock(HDIB hDIB,		//  I Դ���
						   RECT rect);		//  I ���������С

//����		:�������Ĵ���ֵ
//����ֵ	:int		0			--�ɹ�
//						����		--ʧ��
int WINAPI GetLastErrorCode();

//����		:��ȡͼ�������
//����ֵ	:int		 ͼ�������
int WINAPI GetSubImageCount();

//����		:���ڽ�DIBͼ�����ݴ�ֱ��ת
//����ֵ	:�ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL WINAPI ReverseImage(HDIB pHdib);		//	I ͼ����

//����		:���ڽ�DIBͼ��������ת
//����ֵ	:��ת���HDIB
HDIB WINAPI RotateImage(HDIB pHdib,			//	I ͼ����
						double dAngle);		//	I �Ƕ�
					   		
//����		:��ȡ��ɫ���С
//����ֵ	: int	��ɫ���С
WORD WINAPI GetPaletteSize(HDIB hDIB);	//   I ͼ����
						  
//����		:��ȡͼ���ʽ
//����ֵ	:BYTE ͼ���ʽ
BYTE WINAPI GetImageFormat();


//����		:��ȡͼ���С
//����ֵ	:BOOL �Ƿ�ɹ�
BOOL WINAPI GetImageSize(HDIB p_hDIB,			//   I  ͼ����
						 DWORD *p_pdwWidth,		//	 O	ͼ����
						 DWORD *p_pdwHeight);	//	 O	ͼ��߶�

//����		:��ȡͼ��DPI
//����ֵ	:BOOL �Ƿ�ɹ�
BOOL WINAPI GetImageDPI(HDIB p_hDIB,			//	I	ͼ����
						DWORD *p_pdwDpiX,		//	O	X����DPI
						DWORD *p_pdwDpiY);		//	O	Y����DPI

//����		:�ж��Ƿ��ɫ
//����ֵ	:BOOL �Ƿ��ɫ
BOOL WINAPI ImageIsColor(HDIB p_hDIB);			//	I	ͼ����

#endif