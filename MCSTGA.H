//create by yaka 20010607
//mcstga.h
// 文件名	: MCSTGA.H
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理TGA的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
typedef struct tgaheader
   {
      BYTE byID_Length;
      BYTE byPalType;
      BYTE byImageType;
      WORD wPalFirstNdx;
      WORD wPalLength;
      BYTE byPalBits;
      WORD wLeft;
      WORD wBottom;
      WORD wWidth;
      WORD wDepth;
      BYTE byBits;
      struct descriptor
         {
            BYTE AlphaBits : 4;
            BYTE HorMirror : 1;
            BYTE VerMirror : 1;
            BYTE Reserved  : 2;
         }  Descriptor;
   }  TGAHEADER;

typedef struct tgafooter
   {
      LONG lExtensionOff;
      LONG lDeveloperOff;
      BYTE bySignature[18];
   }  TGAFOOTER;

typedef struct tgaextension
   {
      WORD wAreaSize;
      BYTE byAuthor[41];
      BYTE byComment1[81];
      BYTE byComment2[81];
      BYTE byComment3[81];
      BYTE byComment4[81];
      WORD wCreationMonth;
      WORD wCreationDay;
      WORD wCreationYear;
      WORD wCreationHour;
      WORD wCreationMinute;
      WORD wCreationSecond;
      BYTE byJobName[41];
      WORD wJobHours;
      WORD wJobMinutes;
      WORD wJobSeconds;
      BYTE bySoftwareName[41];
      WORD wVersionNumber;
      BYTE byVersionLetter;
      BYTE byKeyColorB;
      BYTE byKeyColorG;
      BYTE byKeyColorR;
      BYTE byKeyColorA;
      WORD wAspectNumerator;
      WORD wAspectDenominator;
      WORD wGammaNumerator;
      WORD wGammaDenominator;
      LONG lColorCorrectionOff;
      LONG lPostageStampOff;
      LONG lScanLineOff;
      BYTE byAttributeType;
   }  TGAEXTENSION;

typedef struct argb
   {
      WORD wAlpha;
      WORD wRed;
      WORD wGreen;
      WORD wBlue;
   }  ARGB;

typedef struct tgad_var
   {
      LPSTR lpDataBuff;
      LPSTR lpBgnBuff;
      LPSTR lpEndBuff;
      DWORD dwDataLen;
      WORD  wMemLen;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wSrcBits;
      WORD  wNewBits;
      BOOL  bEOF;
      BOOL  bRLE;
      BOOL  bHorMirror;
      BOOL  bVerMirror;
   }  TGAD_VAR;
typedef TGAD_VAR FAR *LPTGAD_VAR;

typedef struct tgac_var
   {
      LPSTR lpDataBuff;
      LPSTR lpEndBuff;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wBits;
      WORD  wByteCnt;
   }  TGAC_VAR;
typedef TGAC_VAR FAR *LPTGAC_VAR;