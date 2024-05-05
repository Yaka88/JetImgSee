//ErrorCode.h
//
// �ļ���	: JetimgSeeErrorcode.h
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: �������
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#define SUCCESS			   0 //�ɹ�;
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
#define HUFFKEY_NOT_FOUND    208
#define CCITT4_ERROR_CODE    209
#define NOT_SUPPORT_CCITT4EXTEN    210
#define CCITT1D_ERROR_CODE     211
#define CCITT3_ERROR_CODE      212
#define JPEG_TIFF			   213
#define CREATETIF_FAILED		214 //����TIFF�ļ�����
#define CREATETIF_AllOCMEMERR	215 //����TIFF�ļ�ʱ�����ڴ����
#define ENRGB_AllocMemERR		216 //����ʱ��λ��ת���ɶ�λ�������ڴ�ʧ�ܣ�
#define ENLZW_ALLOCMEMERR		217 //LZW���������ڴ����
#define ENLZW_READBMPERR		218 //LZW����ʱ���ļ�ʧ�ܣ�
#define DELZW_ERROR				219 
//JPEG

#define CREATEJPG_FAILED   302 // ����JPEG�ļ�ʧ��;
#define NOTSUPPORT_YIQ	   303 // ��֧��YIQɫ��ϵͳ;
#define NOTSUPPORT_CMYK    304 // ��֧��CMYKɫ��ϵͳ;
#define NOSUCHSYSTEM       305 // �����ڴ���ɫ��ϵͳ;
#define NO_EOI_JPG         306 // �Ҳ���JPEG�ļ�������־;
#define NOJPEG			   307//����JPEG�ļ�;
#define UNSUPPORT_JPG     308 //������ļ���ʽ;
#define NOTSUPPORT_16PRECISION 309 //��֧��JPEG16λ���ݾ���;
#define NOTSUPPORT_BW  310 // JPEGѹ���������ܴ���ڰ�ͼ�󡢵�ɫ��ͼ���32λ��ɫ��
#define NOTSUPPORT_DCT		312  //add by yaka 20010525
//add by yaka 20010530
#define NOPCX				401			//��pcx�ļ�
#define NOPCXPALETTE		402			//�Ҳ�����ɫ����Ϣ
#define CREATEPCX_FAILED    403        // ����PCX�ļ�ʧ��;
#define UNSUPPORT_PCX		404

#define UNSUPPORT_TGA		501			//��֧�ֵ�TGA��ʽ
#define CREATETGA_FAILED    502			 // ����TGA�ļ�ʧ��;   

#define	NOGIF				601
#define CREATEGIF_FAILED    602			 // ����GIF�ļ�ʧ��;   
#define UNSUPPORT_GIF		603

#define OPENFILE_ERR 1000//����ͳһ��OpenImgFile(..);
#define READSOURCEIMG_ERR  1001 //��ȡԴ�ļ�ͼ�����ݳ���;
#define GLOBALLOCKERROR   1002 // add by yaka 20010528


