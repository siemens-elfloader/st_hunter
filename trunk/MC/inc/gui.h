
#ifndef _GUI_H
#define _GUI_H

#define FONT_N   FONT_SMALL
#define FONT_B   (FONT_SMALL+1)

#ifdef ELKA
#define YDISP1 24
#define MAIN_B	3
#else
#define MAIN_B	2
#define YDISP1 0
#endif

extern int ICO_X, ICO_Y, TXT_X1, TXT_X2, CUR_X;

#define VISOTA scr_h1-soft_h-1

#define TOP_B	2
#define TOP_Y   (YDISP1+MAIN_B)
#define TOP_H   (TOP_B+head_h)
#define DRV_X   MAIN_B+2
#define TAB_X   (scr_w-MAIN_B-TAB_W)
#define TAB_W   (TOP_H-2)

#define FLS_B   1
#define FLS_Y   (TOP_Y+TOP_H+FLS_B+1)
#define FLS_H   (scr_h-FLS_B-FLS_Y)

#define ITM_B	1
#define ITM_S	1
#define ITM_FH  (cur_h)
#define ITM_X1  (MAIN_B+1)
#define ITM_W   MAIN_B
#define TXT_X	TXT_X1

#define SB_HS   3
#define SB_VS   8
#define SB_B	1
#define SB_X	(scr_w-MAIN_B-SB_HS)
#define SB_H	(FLS_H-1-SB_B)

#ifdef ELKA
#define PRGP_H   9
#else
#define PRGP_H   5
#endif

extern int scr_h;
extern int txt_h;
extern int popup_h;
extern int prop_h;
extern int font;
extern int scr_w;
extern int scr_h1;
extern int ico_hw;
extern int itms_max;
extern const int CONFIG_SOFT_BUTTON, CONFIG_FOLDER_VID;

extern IMGHDR *cursor, *background, *iinfo, *iprop;

extern const int CONFIG_PUT_IN_HEAD;

extern int NormalFont;
extern int BoldFont;

extern GBSTMR sctm;
extern FILEINF *scfile;
extern CHKDS_PARAM SelParam;
void InitScr();
void DrwName();
void ShowFiles();
void ShowProgr();
void menugrafic();
void DrwSoftButton(char *Lbut,char *Rbut);
void main_gui_free();
void DrawFon();

RECT DrawINF();
RECT DrawPROP();

#endif
