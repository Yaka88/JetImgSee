// �ļ���	: MCSPCX.H
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����WMF��ͷ�ļ�
// ��������	: 2001/08/08
// ����		: yaka
// ��ע 		: 
// �ɷ���Ԫ�ļ����ݶ���

#define	ALDUSKEY				0x9AC6CDD7
#define OLDWMF					0x00090001

// �ɷ���Ԫ�ļ�ͷ�ṹ
typedef struct tagALDUSMFHEADER
{
    DWORD   key;			// �������ALDUSKEY
    WORD	hmf;
    short   left;
    short   top;
    short   right;
    short   bottom;
    WORD    inch;
    DWORD   reserved;
    WORD    checksum;
} ALDUSMFHEADER;