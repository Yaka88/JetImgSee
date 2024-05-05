// TIF_DCCITT.cpp: implementation of the CTIF_DCCITT class.
//
//////////////////////////////////////////////////////////////////////
// �ļ���	: TIFFDCCITT.cpp
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����TIFF��CCITT�����ʵ���ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
#include "stdafx.h"
#include "TIFDCCITT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTIF_DCCITT::CTIF_DCCITT()
{
}
CTIF_DCCITT::~CTIF_DCCITT()
{
}
extern int iReturn;
int iModeTab[] = {/*For serching Mode codes*/
	2,/*(0),		   0*/			-2,/*(1),	   1*/
	4,/*(2),		  00*/			6,/*(3),	  01*/
	8,/*(4),		 000*/			-1,/*(5),	 001*/
	-6,/*(6),		 010*/			-3,/*(7),	 011*/
	10,/*(8),		0000*/			-0,/*(9),	0001*/
	12,/*(10),		00000*/			14,/*(11),	00001*/
	16,/*(12),		000000*/		18,/*(13),	000001*/
	-7,/*(14),		000010*/		-4,/*(15),	000011*/
	20,/*(16),		0000000*/		-9,/*(17),	0000001*/
	-8,/*(18),		0000010*/		-5,/*(19),	0000011*/
	22,/*(20),		00000000*/		32767,/*(21),	00000001*/
	24,/*(22),	000000000*/			-10,/*(23),	000000001*/
	26,/*(24),  0000000000*/		32767,/*(25),0000000001*/
	28,/*(26),  00000000000*/		32767,/*(27),00000000001*/
	32767,/*(28),  000000000000*/   30,/*(29),000000000001*/
	-32765,/*(30),  0000000000010*/  -32766/*(31),0000000000011*/
};
int iModeTab1[] = {/*For serching Mode codes*/
	2,/*(0),		   0*/			-2,/*(1),	   1*/
	4,/*(2),		  00*/			6,/*(3),	  01*/
	8,/*(4),		 000*/			-1,/*(5),	 001*/
	-6,/*(6),		 010*/			-3,/*(7),	 011*/
	10,/*(8),		0000*/			-0,/*(9),	0001*/
	12,/*(10),		00000*/			14,/*(11),	00001*/
	16,/*(12),		000000*/		18,/*(13),	000001*/
	-7,/*(14),		000010*/		-4,/*(15),	000011*/
	20,/*(16),		0000000*/		-9,/*(17),	0000001*/
	-8,/*(18),		0000010*/		-5,/*(19),	0000011*/
	22,/*(20),		00000000*/		32767,/*(21),	00000001*/
	24,/*(22),	000000000*/			-10,/*(23),	000000001*/
	26,/*(24),  0000000000*/		32767,/*(25),0000000001*/
	28,/*(26),  00000000000*/		32767,/*(27),00000000001*/
	32767,/*(28),  000000000000*/   -32766,/*(29),000000000001*/
};

int iWhiteRunFull[218] = {/*White Run Code Words*/
2,		4,		6,		8,		10,		12,		14,		16,		18,
20,		22,		24,		26,		28,		30,		32,		34,		36,		
38,		40,		42,		-2,		-3,		44,		46,		-4,		-5,		
48,		-6,		-7,		50,		52,		54,		56,		58,		60,			
62,		-10,	-11,	64,		66,		68,		70,		72,		-128,		
-8,		-9,		74,		76,		-64,	78,		80,		82,		-13,				
84,		86,		88,		-1,		-12,	90,		92,		94,		96,							
98,		100,	102,	104,	106,	108,	-192,	-1664,	110,							
112,	114,	-16,	-17,	-14,	-15,	116,	118,	120,						
-22,	-23,	122,	-20,	124,	126,	128,	-19,	130,		
132,	-26,	134,	136,	138,	-21,	-28,	140,	142,	
144,	-27,	146,	148,	-18,	-24,	150,	152,	-25,	
154,	156,	158,	160,	162,	164,	166,	-256,	168,
170,	-29,	-30,	-45,	-46,	-47,	-48,	-33,	-34,
-35,	-36,	-37,	-38,	-31,	-32,	-53,	-54,	-39,	
-40,	-41,	-42,	-43,	-44,	-61,	-62,	-63,	-0,

-320,	-384,	-59,	-60,	172,	174,	-49,	-50,	-51,
-52,	-55,	-56,	-57,	-58,	-448,	-512,	176,	-640,
-576,	178,	180,	182,	184,	186,	188,	-32767,	190,
192,	-1472,	-1536,	-1600,	-1728,	-704,	-768,	-832,	-896,
-960,	-1024,	-1088,	-1152,	-1216,	-1280,	-1344,	-1408,	194,	
-32767,	196,	198,	200,	202,	204,	-32767,	-1792,	206,
208,	210,	-1856,	-1920,	212,	214,	-32767,	216,    -1984,
-2048,	-2112,	-2176,	-2240,	-2304,	-2368,	-2432,	-2496,	-2560,
-32765,	-32766
};

int iBlackRunFull[218] = {/*Black Run Code Words*/
2,		4,		6,		8,		-3,		-2,		10,		12,		-1,//9
-4,		14,		16,		-6,		-5,		18,		20,		22,		-7,//18
24,		26,		28,		30,		-9,		-8,		32,		34,		36,//27	
38,		-10,	-11,	40,		-12,	42,		44,		46,		48,//36
-13,	50,		52,		-14,	54,		56,		58,		-32767,	60,	//45
62,		64,		66,		68,		70,		72,		74,		76,		78,	//54
-15,	80,		82,		84,		86,		-32767,	88,		90,		92,//63
94,		-18,	96,		98,		100,	102,	104,	106,	-64,//72
108,	110,	112,	-16,	-17,	114,	116,	118,	120,	//81
122,	124,	126,	128,	-0,		130,	-32767,	-1792,	132,//90
134,	136,	-1856,	-1920,	138,	140,	142,	144,	146,//99
148,	150,	-24,	-25,	152,	154,	156,	158,	160,//108
-23,	162,	164,	166,	168,	170,	172,	174,	176,//117
178,	180,	-22,	182,	184,	186,	-19,	-20,	188,//126
190,	192,	-21,	194,	-32767,	216,	-1984,	-2048,	-2112,//135
-2176,	-2240,	-2304,	-2368,	-2432,	-2496,	-2560,	-52,	196,//144
198,	-55,	-56,	200,	202,	-59,	-60,	204,	206,//153
-320,	-384,	-448,	208,	-53,	-54,	210,	212,	214,//162
-50,	-51,	-44,	-45,	-46,	-47,	-57,	-58,	-61,//171
-256,	-48,	-49,	-62,	-63,	-30,	-31,	-32,	-33,//180
-40,	-41,	-128,	-192,	-26,	-27,	-28,	-29,	-34,//189
-35,	-36,	-37,	-38,	-39,	-42,	-43,	-640,	-704,//198
-768,	-832,	-1280,	-1344,	-1408,	-1472,	-1536,	-1600,	-1664,//207
-1728,	-512,	-576,	-896,	-960,	-1024,	-1088,	-1152,	-1216,//216
-32765,	-32766
};

#define ONEDFLAG 32766
#define TWODFLAG 32765
// ** ע�⣺Group4 ����ʱ(��2ά����)��Fill Bits,�����λ����1ά������ÿ�е�ĩβ���������λ(���ص��8��)��
// ** ����CCITT Group 3 ���õ�2ά���룬Ҳ�õ�1ά���롣
// ��������	: TIFF��CCITT ����
// ����		: CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar,LPBYTE pOutTiff, WORD wStripNum,DWORD& dwRowCnt,DWORD& dwRowNum)
//					ԭ�ļ�			TIFF�ṹ			������		����					����		�к�
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int CTIF_DCCITT::iDecodeCCITTNGroup(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar,LPBYTE pOutTiff, WORD wStripNum,DWORD& dwRowCnt,DWORD& dwRowNum)
{
	unsigned int ii; int kk=0;	
	int number;//�г�
	int LastLineOffset[MAXCHANGE],LastNumChange=1;//0;
	int LineOffset[MAXCHANGE];//��¼ǰһ�б任��
	for(ii=0;ii<MAXCHANGE;ii++)//(ԭ��Ϊ0�Ǵ����);���״���MODEVER0ģʽ�Ǳ����,�����ڵ���WriteAlineʱҲ�Ǳ���ġ�
		LineOffset[ii]=LastLineOffset[ii]=lpTIFDVar->dwWidth;
	unsigned int LineNum = 0;//��¼����������
	int tempLineOffset=0;//[MAXCHANGE];//for pass mode,for a0'(that is just below b2).
	int tempLineOffset0;//for a0
    unsigned int numchange=0;//��¼�任����Ŀ,//Ϊ����������λ�õ�ĳɫ�г�����Ӧ�ı任��������0�α任���ǴӰ�ɫ�㿪ʼ�������Ǽ���İ�ɫ�㡣
	unsigned int sumnum1 = 0,sumnum2 = 0;//��¼��ǰ����һ������Ŀ
	int iMode;//����ģʽ��
	DWORD dwDataNdx ;//���Զ�λ�����һ�����ݱ����λ�á�

	//��ʼ����
	BOOL bWhiteRun = TRUE;
	BYTE byMask = 0x80;
	BOOL bFound = FALSE;
	WORD wCodeMethod = lpTIFDVar->wCompress;
	bIsGroup3 =((wCodeMethod == CN_Group3Fax) ? 1: 0);
	BOOL bAddTag = FALSE;//��ΪFALSE��Group3������һά���룻
	if(lpTIFDVar->dwGroupOption & (DWORD)0x01)
		bAddTag = TRUE;
	bIsLastLine = FALSE;//�Ƿ������һ�У���GROUP3ʱ��Ч��
    bEolInBound = FALSE;//EOL�Ƿ�ʹ�������ֽڱ߽磻
	if(lpTIFDVar->dwGroupOption & (DWORD)0x04)
		bEolInBound = TRUE;

	bIsTwoD = TRUE;//ָ����ǰ�������Ƿ���ö�ά���뷽������GROUP3ʱ��Ч��
	if(bIsGroup3)
		bIsTwoD = FALSE;//��֤ÿһstrip��һ�б�����һά��
	else
	{
		bAddTag = TRUE;//����GROUP3ʱ���������ã�
		bEolInBound = TRUE;//����GROUP3ʱ���������ã�
	}
	if(bAddTag)
		{ wEol = 0x03; wEolLength = 13;}
	else
		{ wEol = 0x01; wEolLength = 12;}

    int BmpWidth=(((lpTIFDVar->dwWidth+7)/8+3)/4)*4;
	HANDLE hImage = GlobalAlloc(GHND,(DWORD)BmpWidth);
	LPSTR lpImage = (char*)GlobalLock(hImage);
	if (lpImage == NULL)		//add by yaka 20010528
		return GLOBALLOCKERROR;
	while(LineNum < lpTIFDVar->dwRowsPerStrip)                                                                                                                                                                                                 
	{                                                                                                                                                                
		if(wCodeMethod==CN_CCITT1D)
		{
			numchange = iSearchLineHuffman(m_SrcFile, lpTIFDVar,LineOffset,byMask);
			if(numchange == -1)
				return CCITT1D_ERROR_CODE;
			else
			{
				WriteAline(lpImage, LineOffset,lpTIFDVar->dwWidth,lpTIFDVar->wPhotoMet);
				dwDataNdx = dwRowNum * (DWORD)BmpWidth;
				memcpy(pOutTiff+dwDataNdx,lpImage,BmpWidth);
				(lpTIFDVar->bVerMirror ? dwRowNum++ : dwRowNum--);
				LineNum++;	dwRowCnt++;	
			}
		}
		else
		{
			if(bIsTwoD || ((LineNum==0) && (bIsGroup3==TRUE)))//���ж�ά���룻(wStripNum==0) && 
			{
				number = 0;//����
				sumnum1 = sumnum2 = 0;	
				PASSLOOP: iMode = iSearchModeTab(m_SrcFile, lpTIFDVar, byMask);
				kk=0;
				if (tempLineOffset)
					tempLineOffset0 = tempLineOffset;
				else
				{ 
					if (!numchange)
						tempLineOffset0 = -1;
					else
						tempLineOffset0 = LineOffset[numchange - 1];
				}
				switch (iMode)
				{
				case MODEPASS:
					// ** ֮���Լ�2������Ϊb1����ɫ��Ҫ��a0�෴������kk�α任�����ɫӦ��ͬ��
					// ** �����е�(numchange-1)�����ɫ��ע�⣺(numchange-1)���Ѿ�֪����numchange���������롣
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)//kk��numchange-1��ɫ,ż�� is white
						if(LastLineOffset[kk] > tempLineOffset0)	break;
					if (kk>=LastNumChange-1)
						kk = LastNumChange-1;
					if (tempLineOffset0<0)
						tempLineOffset0 =0;
					if (numchange)
						sumnum1 = LastLineOffset[kk+1] - LineOffset[numchange -1];
					else
						sumnum1 = LastLineOffset[kk+1];//sumnum1��ԭb2-a0?
					tempLineOffset = LastLineOffset[kk+1];//tempLineOffset��a0'? tempLineOffset0��a0?
					goto PASSLOOP;
					break;
				case MODEHOR://**???��Group 3������£�2ά�����е�1ά�����������һ��ĩβ���Ƿ�������λ�����⣻
					// ���� a0~a1;
					number = iSearchKeyTab(m_SrcFile, lpTIFDVar, bWhiteRun,byMask);//numchange%2Ϊ0�����ʾ�������г̣���֮���г̣�����number������ַ���ݣ���
					if(number != -1)
						sumnum1 += number;
					else
						return HUFFKEY_NOT_FOUND;
					//����a1~a2;
					number = iSearchKeyTab(m_SrcFile, lpTIFDVar, bWhiteRun,byMask);//numchange%2Ϊ0�����ʾ�������г̣���֮���г̣�����number������ַ���ݣ���
					if(number != -1)
						sumnum2 += number;
					else
						return HUFFKEY_NOT_FOUND;
					if (numchange)
						LineOffset[numchange] = LineOffset[numchange-1] + (sumnum1);
					else
						LineOffset[numchange] = sumnum1;
					LineOffset[numchange+1] = LineOffset[numchange++] + (sumnum2);//numchange++���ܶ���
					break;
				case MODEVER0:
					// ** ֮���Լ�2������Ϊb1����ɫ��Ҫ��a0�෴������kk�α任�����ɫӦ��ͬ��
					// ** �����е�(numchange-1)�����ɫ��ע�⣺(numchange-1)���Ѿ�֪����numchange���������롣
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)
					//kk��numchange-1��ɫ,ż���α仯����ɫ is white����0�μ�Ϊ����İ�ɫ��
						if(LastLineOffset[kk] > tempLineOffset0)//kkΪb1���ڵأ�
							break;
					if (kk>=LastNumChange)
						kk = LastNumChange;//b1������β��
					LineOffset[numchange] = LastLineOffset[kk];
					bWhiteRun = (bWhiteRun ? FALSE : TRUE);
					break;
				case MODEVERR1:
					if (LineNum==0)		{
						AfxMessageBox("Error Coding!",NULL,MB_OK);
						return CCITT4_ERROR_CODE;
					}
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)	//kk��numchange-1��ɫ,ż�� is white
						if(LastLineOffset[kk] > tempLineOffset0)	break;
					if (kk>=LastNumChange)
						kk = LastNumChange ;
					LineOffset[numchange] = LastLineOffset[kk]+ 1;
					bWhiteRun = (bWhiteRun ? FALSE : TRUE);
					break;
				case MODEVERR2:
					if (LineNum==0)	{	
						AfxMessageBox("Error Coding!",NULL,MB_OK);
						return CCITT4_ERROR_CODE;
					}
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)	//kk��numchange-1��ɫ,ż�� is white
						if(LastLineOffset[kk] > tempLineOffset0)	break;
					if (kk>=LastNumChange)
						kk = LastNumChange;
					LineOffset[numchange] = LastLineOffset[kk] +  2;
					bWhiteRun = (bWhiteRun ? FALSE : TRUE);
					break;
				case MODEVERR3:
					if (LineNum==0)		{
						AfxMessageBox("Error Coding!",NULL,MB_OK);
						return CCITT4_ERROR_CODE;
					}
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)	//kk��numchange-1��ɫ,ż�� is white
						if(LastLineOffset[kk] > tempLineOffset0)	break;
					if (kk>=LastNumChange)
						kk = LastNumChange;
					LineOffset[numchange] = LastLineOffset[kk] +  3;
					bWhiteRun = (bWhiteRun ? FALSE : TRUE);
					break;
				case MODEVERL1:
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)	//kk��numchange-1��ɫ,ż�� is white
						if(LastLineOffset[kk] > tempLineOffset0)	break;
					if (kk>=LastNumChange)
						kk = LastNumChange;
					LineOffset[numchange] = LastLineOffset[kk] - 1;
					bWhiteRun = (bWhiteRun ? FALSE : TRUE);
					break;
				case MODEVERL2:
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)	//kk��numchange-1��ɫ,ż�� is white
						if(LastLineOffset[kk] > tempLineOffset0)	break;
					if (kk>=LastNumChange)
						kk = LastNumChange;
					LineOffset[numchange] = LastLineOffset[kk] - 2;
					bWhiteRun = (bWhiteRun ? FALSE : TRUE);
					break;
				case MODEVERL3:
					for(kk=numchange%2;kk<=LastNumChange+1;kk+=2)	//kk��numchange-1��ɫ,ż�� is white
						if(LastLineOffset[kk] > tempLineOffset0)	break;
					if (kk>=LastNumChange)
						kk = LastNumChange;
					LineOffset[numchange] = LastLineOffset[kk] - 3;
					bWhiteRun = (bWhiteRun ? FALSE : TRUE);
					break;
				case ONEDFLAG:
					bIsTwoD = FALSE; break;
				case TWODFLAG:
					bIsTwoD = TRUE;  break;
				case MODEEXT2D:
					AfxMessageBox("Not Supported!Changing from compression to non-compression",NULL,MB_OK);
					return NOT_SUPPORT_CCITT4EXTEN;
				case MODEEXT1D:
					AfxMessageBox("Not Supported!Changing from compression to non-compression",NULL,MB_OK);
					return NOT_SUPPORT_CCITT4EXTEN;
				default:
					AfxMessageBox("mode error!",NULL,MB_OK);
					return CCITT4MODE_NOT_FOUND;
				}// end of switch;
			}
			if(!bIsTwoD)//һά���룻
			{
				numchange = iSearchLineHuffman(m_SrcFile, lpTIFDVar,LineOffset,byMask);
 				if(numchange == -1)
					return CCITT1D_ERROR_CODE;
				WriteAline(lpImage, LineOffset,lpTIFDVar->dwWidth,lpTIFDVar->wPhotoMet);
				dwDataNdx = dwRowNum * (DWORD)BmpWidth;
				memcpy(pOutTiff+dwDataNdx,lpImage,BmpWidth);
				(lpTIFDVar->bVerMirror ? dwRowNum++ : dwRowNum--);
				for(unsigned int j=0;j<=numchange;j++)
				{
					LastLineOffset[j] = LineOffset[j];
					LineOffset[j] = 0;
				}
				LastLineOffset[numchange+1] = LastLineOffset[numchange];//0;// ???????????????????
				LastNumChange = numchange;
				numchange = 0;
				LineNum++;	bWhiteRun = TRUE;
				dwRowCnt++;
			}
			else if(bAddTag)//��GROUP3ֻ�õ�һά����ʱ��bAddTag�����ã�
			{
				if (iMode != MODEHOR)
					if (numchange)//��ʱLineOffset[numchange]����ֵ��
						sumnum1 = LineOffset[numchange] - LineOffset[numchange -1];//a1-a0;
					else	sumnum1 = LineOffset[numchange];
				if (LineOffset[numchange] > (int)lpTIFDVar->dwWidth)
				{
					AfxMessageBox("Coding Error!LineLength>LineWidth",NULL,MB_OK);	return CCITT4_ERROR_CODE;
				}
				if (LineOffset[numchange] < (int)lpTIFDVar->dwWidth)
					numchange++;
				else//һ�н��������
				{				
					if(wCodeMethod == CN_Group3Fax)
					{
						if(bEolInBound) //skip some Fill bits before the EOL to enable EOL loacated in byte bound;
						{ 
							if(bIsLastLine){
								if(byMask>=((wEolLength==13) ? 0x20 : 0x00))  byMask = ((wEolLength==13) ? 0x20 : 0x00);
								else{
									lpTIFDVar->lpBgnBuff++;
									byMask = (0x80 >> ((wEolLength==13) ? 2 : 0));
								}
							}
							else{
								if(byMask>=((wEolLength==13) ? 0x10 : 0x08))  byMask = ((wEolLength==13) ? 0x10 : 0x08);
								else{
									lpTIFDVar->lpBgnBuff++;
									byMask = (0x80 >> ((wEolLength==13) ? 3 : 4));
								}
							}
						}
						else{//����Fill bits before the EOL;
							if(byMask != 0x80){
								lpTIFDVar->lpBgnBuff++;
								byMask = 0x80;
							}
						}
						iMode = iSearchModeTab(m_SrcFile, lpTIFDVar, byMask);
						if(iMode == ONEDFLAG) bIsTwoD = FALSE;
						else if(iMode == TWODFLAG) bIsTwoD = TRUE;
						else{
							AfxMessageBox("���ļ��޷���ȷ����!");
							return CCITT3_ERROR_CODE;
						}
						// ** Skip Fill Bits after the EOL;but when bEolInBound is TURE, can't skip after EOL;
						if((byMask != 0x80) && bEolInBound)
						{
							lpTIFDVar->lpBgnBuff++;
							byMask =0x80;
							if(iReturn=iReadDataToBuff(lpTIFDVar, m_SrcFile)) return iReturn;
						}
					}
					WriteAline(lpImage, LineOffset,lpTIFDVar->dwWidth,lpTIFDVar->wPhotoMet);
					dwDataNdx = dwRowNum * (DWORD)BmpWidth;
					memcpy(pOutTiff+dwDataNdx,lpImage,BmpWidth);
					(lpTIFDVar->bVerMirror ? dwRowNum++ : dwRowNum--);
					for(unsigned int j=0;j<=numchange;j++){
						LastLineOffset[j] = LineOffset[j];	LineOffset[j] = 0;
					}
					LastLineOffset[numchange+1] = LastLineOffset[numchange];//0;// ???????????????????
					LastNumChange = numchange;
					numchange = 0;
					LineNum++;	bWhiteRun = TRUE;
					dwRowCnt++;
				}//end of elseһ�н��������
				tempLineOffset=0;
			}//end of if;
			if((LineNum==lpTIFDVar->dwRowsPerStrip-1) && (wStripNum == lpTIFDVar->wOffsetsLen-1))
				bIsLastLine = TRUE;
		}//end of outer else
	} //end of while
	GlobalUnlock(hImage);
	GlobalFree(hImage);
	return 0;
}

void CTIF_DCCITT::vReverseByte(BYTE* byChar)
{
	BYTE byValue = *byChar;
	*byChar = 0;
	BYTE byMask1 = 0x80;
	BYTE byMask2 = 0x01;
	for(WORD i=0; i<8; i++)
	{
		if(byValue & byMask1)
			*byChar |= byMask2;
		byMask1 >>= 1;
		byMask2 <<= 1;
	}
}

int CTIF_DCCITT::iSearchModeTab(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar,BYTE& byMask)
{
	LPSTR lpTemp;
	DWORD dwBitCnt = 0;//, dwWidthCnt;
	BOOL bFound;
	BYTE byChar,byTemp;//, byLeftChar, byMask, byStrNdx, ;
	int iCount, iBase, iNdx;
	bFound = FALSE;
	BYTE byStrNdx = 0x00;
	BYTE byBitStr[15];
	while(!bFound)
	{
		byChar = *lpTIFDVar->lpBgnBuff;// ��ȡ��һ���ֽڣ�
		if(lpTIFDVar->FillOrder == 2)
			vReverseByte(&byChar);
		byBitStr[byStrNdx++] = (BYTE)((byMask & byChar) ? 0x01 : 0x00);
		byBitStr[byStrNdx] = 0x02; //�Զ����־��(��ʱ����)������һ��ѭ���ͻᱻ��һ������

		lpTemp = (LPSTR)byBitStr;
		iCount = -1;
		iBase = 0;
		iNdx = 0;
		byTemp = *lpTemp++;
		while((byTemp != 0x02) && (iCount == -1))
		{
			iNdx += (int)byTemp;
			  //add by yaka 20010614
			if(iNdx>=30 && wEolLength==12 || iNdx>=32 && wEolLength==13)
					{
						return 32766;
					}
			//add end
			iBase = ((wEolLength==13) ? iModeTab[iNdx] : iModeTab1[iNdx]);//(bWhiteRun ? iWhiteRunFull[iNdx] : iBlackRunFull[iNdx]);
			if(iBase > 0)
				iNdx = iBase;
			else
				iCount = -iBase;
			byTemp = *lpTemp++;
		}
		if(iCount != -1)
			bFound = TRUE;// ** Terminating Code Words
		byMask >>= 1;
		if(byMask  == 0x00)
		{
			byMask = 0x80;
			byChar = *lpTIFDVar->lpBgnBuff++;
			if(lpTIFDVar->FillOrder == 2)
				vReverseByte(&byChar);
			if(iReturn=iReadDataToBuff(lpTIFDVar, m_SrcFile)) return iReturn;
		}
	}//end of  while********************************
	return iCount;
}

int CTIF_DCCITT::iSearchKeyTab(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar,BOOL& bWhiteRun, BYTE& byMask)
{
	LPSTR lpTemp;
	DWORD dwBitCnt = 0;//, dwWidthCnt;
	BOOL bFound;
	BYTE byChar,byTemp;//, byLeftChar, byMask, byStrNdx, ;
	int iCount, iBase, iNdx;
	bFound = FALSE;
	BYTE byStrNdx = 0x00;
	BYTE byBitStr[15];

	while(!bFound)
	{
		byChar = *lpTIFDVar->lpBgnBuff;// ��ȡ��һ���ֽڣ�
		if(lpTIFDVar->FillOrder == 2)
			vReverseByte(&byChar);
		byBitStr[byStrNdx++] = (BYTE)((byMask & byChar) ? 0x01 : 0x00);
		byBitStr[byStrNdx] = 0x02; //�Զ����־��(��ʱ����)������һ��ѭ���ͻᱻ��һ������
		// ** See if the byStrNdx>=Minimun Code Word Length
		if(byStrNdx >= (BYTE)(bWhiteRun ? 0x04 : 0x02))
		{
			lpTemp = (LPSTR)byBitStr;
			iCount = -1;
			iBase = 0;
			iNdx = 0;
			byTemp = *lpTemp++;
			while((byTemp != 0x02) && (iCount == -1))	{
				iNdx += (int)byTemp;
				//add by yaka 20010614
				if(iNdx>=216 && wEolLength==12 || iNdx>=218 && wEolLength==13)  //modify by yaka 20010613
					{
						AfxMessageBox("�����CCITT 2-Dѹ������!");
						return -1;
					}
				//add end
				if(wEolLength==13)  
					iBase = (bWhiteRun ? iWhiteRunFull[iNdx] : iBlackRunFull[iNdx]);
				else
					iBase = (bWhiteRun ? iWhiteRun[iNdx] : iBlackRun[iNdx]);
				if(iBase > 0)
					iNdx = iBase;
				else
					iCount = -iBase;
				byTemp = *lpTemp++;
			}
			if(iCount != -1) {
				if(iCount < 32765)	{
					dwBitCnt += (DWORD)iCount;
					if(iCount > 63)
						byStrNdx = 0x00;// ** Make_up Code Words
					else
						bFound = TRUE;// ** Terminating Code Words
				}
				else
					return -1;//iCount;
			}
		}// end of if
		byMask >>= 1;
		if(byMask  == 0x00)
		{
			byMask = 0x80;
			byChar = *lpTIFDVar->lpBgnBuff++;
			if(lpTIFDVar->FillOrder == 2)
				vReverseByte(&byChar);
			if(iReturn=iReadDataToBuff(lpTIFDVar, m_SrcFile)) return iReturn;
		}
	}//end of  while********************************
	bWhiteRun = (bWhiteRun ? FALSE : TRUE);
	return dwBitCnt;
}

int CTIF_DCCITT::iSearchLineHuffman(CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar, int* LineOffset1, BYTE& byMask)
{
	int numchange = 0;//the number of switch between black and white point;
	DWORD dwWidthCnt = 0UL;
	BOOL bFound, bWhiteRun = TRUE;//�����԰�ɫ�㿪ʼ
	BYTE byChar,byTemp;
	int iCount, iBase, iNdx;//test
	LPSTR lpTemp; 
	BYTE byStrNdx = 0x00;
	BYTE byBitStr[15];

while(dwWidthCnt < lpTIFDVar->dwWidth)
	{
HERE1:	bFound = FALSE;
		while(!bFound)
		{
			byChar = *lpTIFDVar->lpBgnBuff;// ��ȡ��һ���ֽڣ�
			if(lpTIFDVar->FillOrder == 2)
				vReverseByte(&byChar);
			byBitStr[byStrNdx++] = (BYTE)((byMask & byChar) ? 0x01 : 0x00);
			byBitStr[byStrNdx] = 0x02; //�Զ����־��(��ʱ����)������һ��ѭ���ͻᱻ��һ������
			// ** See if the byStrNdx>=Minimun Code Word Length(Minimum Length:��Ϊ4����Ϊ2��
			if(byStrNdx >= (BYTE)(bWhiteRun ? 0x04 : 0x02))
			{
				// ** Search Huffman Table
				lpTemp = (LPSTR)byBitStr;
				iCount = -1;
				iBase = 0;
				iNdx = 0;
				byTemp = *lpTemp++;
				// ** iCount = 32767 : Error Code Words;  =32766 : EOL Code Words;  =-1 : Not Found
				while((byTemp != 0x02) && (iCount == -1))
				{
					iNdx += (int)byTemp;//test
					if(iNdx>=216 && wEolLength==12 || iNdx>=218 && wEolLength==13)  //modify by yaka 20010613
					{
						AfxMessageBox("�����CCITT Group3 1-Dѹ������!");
						return -1;
					}
					if(wEolLength==13)  
						iBase = (bWhiteRun ? iWhiteRunFull[iNdx] : iBlackRunFull[iNdx]);
					else
						iBase = (bWhiteRun ? iWhiteRun[iNdx] : iBlackRun[iNdx]);
					if(iBase > 0)
						iNdx = iBase;
					else
						iCount = -iBase;
					byTemp = *lpTemp++;
				}
				if(iCount != -1)
				{
					if(iCount < 32765)
					{
						dwWidthCnt += (DWORD)iCount;
						if(dwWidthCnt>lpTIFDVar->dwWidth)
							dwWidthCnt = lpTIFDVar->dwWidth;
						if(iCount > 63)
							byStrNdx = 0x00;// ** Make_up Code Words
						else
							bFound = TRUE;// ** Terminating Code Words
					}
					else
					{
						if(iCount == 32767)
						{
							AfxMessageBox("�����CCITT Group3 1-Dѹ������!");
							return -1;//CCITT1D_ERROR_CODE;
						}
						else
						{
							if(iCount == 32765) bIsTwoD = TRUE;
							else bIsTwoD = FALSE; // Is 32766 !;
							byMask >>= 1;
							goto HERE2;
						}
					}
				}
			}// end of if
			byMask >>= 1;
			if(byMask  == 0x00)
			{
				byMask = 0x80;
				lpTIFDVar->lpBgnBuff++; 
				if(iReturn=iReadDataToBuff(lpTIFDVar, m_SrcFile))  return iReturn;
			}
		}//end of  while********************************
		byStrNdx = 0x00;
		LineOffset1[numchange++] = dwWidthCnt;
		bWhiteRun = (bWhiteRun ? FALSE : TRUE);
	}//end of outer while
	if(lpTIFDVar->wCompress == CN_Group3Fax)
	{
		if(bEolInBound) //skip some Fill bits before the EOL to enable EOL loacated in byte bound;
		{ 
			if(bIsLastLine){
				if(byMask>=((wEolLength==13) ? 0x20 : 0x00)) {
					byMask = ((wEolLength==13) ? 0x20 : 0x00);
					if(byMask==0x00){
						byMask = 0x80;
						lpTIFDVar->lpBgnBuff++; 
						if(iReturn=iReadDataToBuff(lpTIFDVar, m_SrcFile))  return iReturn;
					}
				}
				else{
					lpTIFDVar->lpBgnBuff++;
					byMask = (0x80 >> ((wEolLength==13) ? 2 : 0));
				}
			}
			else{
				if(byMask>=((wEolLength==13) ? 0x10 : 0x08))  byMask = ((wEolLength==13) ? 0x10 : 0x08);
				else{
					lpTIFDVar->lpBgnBuff++;
					byMask = (0x80 >> ((wEolLength==13) ? 3 : 4));
				}
			}
		}
		else//skip Fill bits before the EOL;
		{
			if(byMask != 0x80){
				lpTIFDVar->lpBgnBuff++;
				byMask = 0x80;
			}
		}
		goto HERE1;
	}
HERE2:if((byMask != 0x80) && bEolInBound)
	{
		// ** Skip Fill Bits after the EOL;but when bEolInBound is TURE, can't skip after EOL;		
		lpTIFDVar->lpBgnBuff++;
		byMask =0x80;
		if(iReturn=iReadDataToBuff(lpTIFDVar, m_SrcFile)) return iReturn;
	}
	return (numchange-1);
}

/*������֪��һ�кڰ׵�ֲ��������LineOffset1�У����ָ����е�ͼ����󣬷ŵ�lpImage�У�wPhotoMet
//������WHITE_IS_ZERO����BLACK_IS_ZERO; iLineWidthΪ����������ָ�����еĵ�����*/
//�Ѿ�֪�����еĺڰ׵�ֲ�״������������������LineOffset1�У�ÿ��Ԫ�ش���ڰ׵��г���һ���е�λ�á�
void CTIF_DCCITT::WriteAline(LPSTR lpImage, int* LineOffset1,int iLineWidth,WORD wPhotoMet)
{
	DWORD dwBitCnt;
	WORD wPrevBitCnt, wLeftBits;
	BOOL bWhiteRun;
	BYTE byLeftBits[8] = {0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};
	BYTE  byLeftChar, byTemp;
	int iLastValue = 0;
	
	wPrevBitCnt = 0;
	wLeftBits = 0;
	bWhiteRun = TRUE;
	for(int i=0; i<MAXCHANGE;i++)//һ�кڰ׵����任MAXCHANGE�Ρ�
	{
		dwBitCnt = *LineOffset1;
		dwBitCnt -= iLastValue;//��ǰ��һ���г̣�
		iLastValue = *LineOffset1++;//��¼��ǰ�㣬��Ϊ��һ���г̵�iLastValue��
		if(wLeftBits)
		{
			if(((DWORD)wLeftBits + dwBitCnt) >= 8UL)
			{
				dwBitCnt -= (DWORD)(8-wLeftBits);
				if(wPhotoMet == 0)
					byTemp = (bWhiteRun ? (BYTE)0x00 : byLeftBits[8-wLeftBits]);
				else
					byTemp = (bWhiteRun ? byLeftBits[8-wLeftBits] : (BYTE)0x00);
				byLeftChar |= (byTemp >> wLeftBits);
				*lpImage++ = byLeftChar;
				wLeftBits = 0;
			}
			else
			{
				if(wPhotoMet == 0)
					byTemp = (bWhiteRun ? (BYTE)0x00 : byLeftBits[dwBitCnt]);
				else
					byTemp = (bWhiteRun ? byLeftBits[dwBitCnt]  : (BYTE)0x00);
				byLeftChar |= (byTemp >> wLeftBits);
			}
		}
		// ** byTemp=0xFF:White Pixel, byTemp=0x00:Black Pixel
		if(wPhotoMet == 0)
			byTemp = (BYTE)(bWhiteRun ? 0x00 : 0xFF);
		else
			byTemp = (BYTE)(bWhiteRun ? 0xFF : 0x00);
		while(dwBitCnt >= 8)
		{
			*lpImage++ = byTemp;
			dwBitCnt -= 8;
		}
		wPrevBitCnt = (WORD)dwBitCnt;
		wLeftBits += wPrevBitCnt;
		if(wLeftBits == wPrevBitCnt)
		{
			if(wPhotoMet == 0)
				byLeftChar = (bWhiteRun ? (BYTE)0x00 : byLeftBits[wLeftBits]);
			else
				byLeftChar = (bWhiteRun ? byLeftBits[wLeftBits] : (BYTE)0x00);
		}
		if(iLastValue == iLineWidth)//�����Ƶ�����β��ע��LineOffset[]�ĳ�ʼ����ÿ�����ص�����
			break;
		bWhiteRun = (bWhiteRun ? FALSE : TRUE);
	} 
	if(wLeftBits)
		*lpImage++ = byLeftChar;
}
// ��������	: ������
// ����		: CFile *m_SrcFile, LPTIFD_VAR lpTIFDVar,
//					ԭ�ļ�			TIFF�ṹ		
// ����ֵ	: int ����ֵ
// ��������	: 2000/02/01
// ����		: yaka
// ��ע 	: 
int inline CTIF_DCCITT::iReadDataToBuff(LPTIFD_VAR lpTIFDVar, CFile *m_SrcFile)
{
	if((lpTIFDVar->lpBgnBuff == lpTIFDVar->lpEndBuff) && !lpTIFDVar->bEOF)
	{
		if(iReturn = m_utility.iReadSrcData(m_SrcFile,(DWORD*)&lpTIFDVar->wMemLen, &lpTIFDVar->dwDataLen, lpTIFDVar->lpDataBuff, &lpTIFDVar->bEOF))
			return iReturn;
		lpTIFDVar->lpBgnBuff = lpTIFDVar->lpDataBuff;
		lpTIFDVar->lpEndBuff = lpTIFDVar->lpBgnBuff + lpTIFDVar->wMemLen;
	}
	return 0;
}
