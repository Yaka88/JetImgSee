// �ļ���	: MCSICO.H
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����ICO,CUR��ͷ�ļ�
// ��������	: 2001/08/13
// ����		: yaka
// ��ע 		: 
// ����ͼ��ͼ�����Ϣͷ�ṹ
typedef struct
{
	BYTE	bWidth;               // Width of the image
	BYTE	bHeight;              // Height of the image (times 2)
	BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;            // Reserved
	WORD	wPlanes;              // Color Planes
	WORD	wBitCount;            // Bits per pixel
	DWORD	dwBytesInRes;         // how many bytes in this resource?
	DWORD	dwImageOffset;        // where in the file is this image
} ICONDIRENTRY, *LPICONDIRENTRY;

// ͼ���ļ���Ϣͷ�ṹ
typedef struct 
{
	WORD			idReserved;   // Reserved
	WORD			idType;       // resource type (1 for icons)
	WORD			idCount;      // how many images?
	ICONDIRENTRY	idEntries[1]; // the entries for each image
} ICONDIR, *LPICONDIR;
