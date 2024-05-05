// 文件名	: MCSPCX.H
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理WMF的头文件
// 创建日期	: 2001/08/08
// 作者		: yaka
// 备注 		: 
// 可放置元文件数据定义

#define	ALDUSKEY				0x9AC6CDD7
#define OLDWMF					0x00090001

// 可放置元文件头结构
typedef struct tagALDUSMFHEADER
{
    DWORD   key;			// 必需等于ALDUSKEY
    WORD	hmf;
    short   left;
    short   top;
    short   right;
    short   bottom;
    WORD    inch;
    DWORD   reserved;
    WORD    checksum;
} ALDUSMFHEADER;
