//mcsgif.h
//create by yaka 20010618
// 文件名	: MCSGIF.H
// 所属项目	: 公共项目
// 版本号	: 1.0
// 内容		: 处理GIF的头文件
// 创建日期	: 2001/02/01
// 作者		: yaka
// 备注 		: 
typedef struct gifheader
   {
      BYTE bySignature[3];
      BYTE byVersion[3];
   }  GIFHEADER;

typedef struct gifscrdesc
   {
      WORD wWidth;
      WORD wDepth;
      struct globalflag
         {
            BYTE PalBits   : 3;
            BYTE SortFlag  : 1;
            BYTE ColorRes  : 3;
            BYTE GlobalPal : 1;
         }  GlobalFlag;
      BYTE byBackground;
      BYTE byAspect;
   }  GIFSCRDESC;

typedef struct gifimage
   {
      WORD wLeft;
      WORD wTop;
      WORD wWidth;
      WORD wDepth;
      struct localflag
         {
            BYTE PalBits   : 3;
            BYTE Reserved  : 2;
            BYTE SortFlag  : 1;
            BYTE Interlace : 1;
            BYTE LocalPal  : 1;
         }  LocalFlag;
   }  GIFIMAGE;

typedef struct gifcontrol
   {
      BYTE byBlockSize;
      struct flag
         {
            BYTE Transparency   : 1;
            BYTE UserInput      : 1;
            BYTE DisposalMethod : 3;
            BYTE Reserved       : 3;
         }  Flag;
      WORD wDelayTime;
      BYTE byTransparencyIndex;
      BYTE byTerminator;
   }  GIFCONTROL;

typedef struct gifplaintext
   {
      BYTE byBlockSize;
      WORD wTextGridLeft;
      WORD wTextGridTop;
      WORD wTextGridWidth;
      WORD wTextGridDepth;
      BYTE byCharCellWidth;
      BYTE byCharCellDepth;
      BYTE byForeColorIndex;
      BYTE byBackColorIndex;
   }  GIFPLAINTEXT;

typedef struct gifapplication
   {
      BYTE byBlockSize;
      BYTE byIdentifier[8];
      BYTE byAuthentication[3];
   }  GIFAPPLICATION;

typedef struct gifd_var
   {
      LPSTR lpDataBuff;
      LPSTR lpBgnBuff;
      LPSTR lpEndBuff;
      DWORD dwDataLen;
      WORD  wMemLen;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wBits;
      BOOL  bEOF;
      BOOL  bInterlace;
   }  GIFD_VAR;
typedef GIFD_VAR FAR *LPGIFD_VAR;

typedef struct gifc_var
   {
      LPSTR lpDataBuff;
      LPSTR lpEndBuff;
      DWORD dwTempCode;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wBits;
      WORD  wByteCnt;
      WORD  wBlockNdx;
      BYTE  byLeftBits;
   }  GIFC_VAR;
typedef GIFC_VAR FAR *LPGIFC_VAR;