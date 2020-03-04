#ifndef _MAIN_H_
  #define _MAIN_H_

unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

const int minus11=-11;
unsigned short maincsm_name_body[140];

int my_csm_id=0;

extern const char description1[32];// Описание закладок
extern const char description2[32];
extern const char description3[32];
extern const char description4[32];
extern const char description5[32];
extern const char description6[32];
extern const char description7[32];
extern const char description8[32];
extern const char description9[32];
extern const char description10[32];
extern const char description11[32];
extern const char description12[32];
extern const char description13[32];
extern const char description14[32];
extern const char description15[32];

extern const char ICON1[128];//пути к иконкам
extern const char ICON2[128];
extern const char ICON3[128];
extern const char ICON4[128];
extern const char ICON5[128];
extern const char ICON6[128];
extern const char ICON7[128];
extern const char ICON8[128];
extern const char ICON9[128];
extern const char ICON10[128];
extern const char ICON11[128];
extern const char ICON12[128];
extern const char ICON13[128];
extern const char ICON14[128];
extern const char ICON15[128];
extern const char NO_ICON[128];

int S_ICONS[16]={
(int)ICON1,
(int)ICON2,
(int)ICON3,
(int)ICON4,
(int)ICON5,
(int)ICON6,
(int)ICON7,
(int)ICON8,
(int)ICON9,
(int)ICON10,
(int)ICON11,
(int)ICON12,
(int)ICON13,
(int)ICON14,
(int)ICON15,
(int)NO_ICON
};

extern const char BM9FILE[128];//пути к файлам
extern const char BM1FILE[128];
extern const char BM2FILE[128];
extern const char BM3FILE[128];
extern const char BM4FILE[128];
extern const char BM5FILE[128];
extern const char BM6FILE[128];
extern const char BM7FILE[128];
extern const char BM8FILE[128];
extern const char BM9FILE[128];
extern const char BM10FILE[128];
extern const char BM11FILE[128];
extern const char BM12FILE[128];
extern const char BM13FILE[128];
extern const char BM14FILE[128];
extern const char BM15FILE[128];

extern const char BMNAME1[32];//Название закладок
extern const char BMNAME2[32];
extern const char BMNAME3[32];
extern const char BMNAME4[32];
extern const char BMNAME5[32];
extern const char BMNAME6[32];
extern const char BMNAME7[32];
extern const char BMNAME8[32];
extern const char BMNAME9[32];
extern const char BMNAME10[32];
extern const char BMNAME11[32];
extern const char BMNAME12[32];
extern const char BMNAME13[32];
extern const char BMNAME14[32];
extern const char BMNAME15[32];

extern const int name_cl;//цвет имен закладок
extern const int description_cl;//цвет описаний закладок
extern const int ACTIVE_KEY_1;
extern const int ACTIVE_KEY_STYLE;
extern const int ACTIVE_KEY_2;
extern const int ACTIVE_KEY_STYLE_2;
extern const int ENA_LOCK;
extern const int IDLE_MODE;
extern const int IDLE_MODE_2;
extern void InitConfig();

void *about()
{ 
  char s[128];
  snprintf(s,255,"AltMyMenu v0.2fix\n(c)kluchnik\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
  return 0;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}

#endif
