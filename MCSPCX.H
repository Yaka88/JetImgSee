//mcspcx.h
// �ļ���	: MCSPCX.H
// ������Ŀ	: ������Ŀ
// �汾��	: 1.0
// ����		: ����PCX��ͷ�ļ�
// ��������	: 2001/02/01
// ����		: yaka
// ��ע 		: 
typedef struct pcxheader
   {
      BYTE byManufacturer;
      BYTE byVersion;
      BYTE byEncoding;
      BYTE byBits;
      WORD wLeft;
      WORD wTop;
      WORD wRight;
      WORD wBottom;
      WORD wXResolution;
      WORD wYResolution;
      BYTE byPalette[48];
      BYTE byReserved;
      BYTE byPlanes;
      WORD wLineBytes;
      WORD wPaletteType;
      WORD wScrWidth;
      WORD wScrDepth;
      BYTE byFiller[54];
   }  PCXHEADER;

typedef struct pcxd_var
   {
      LPSTR lpDataBuff;
      LPSTR lpBgnBuff;
      LPSTR lpEndBuff;
      DWORD dwDataLen;
      WORD  wMemLen;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wPlanes;
      WORD  wSrcBits;
      WORD  wNewBits;
      BOOL  bEOF;
   }  PCXD_VAR;
typedef PCXD_VAR FAR *LPPCXD_VAR;

typedef struct pcxc_var
   {
      LPSTR lpDataBuff;
      LPSTR lpEndBuff;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wPlanes;
      WORD  wBits;
      WORD  wByteCnt;
   }  PCXC_VAR;
typedef PCXC_VAR FAR *LPPCXC_VAR;