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
extern const char description16[32];
extern const char description17[32];
extern const char description18[32];
extern const char description19[32];
extern const char description20[32];
extern const char description21[32];
extern const char description22[32];
extern const char description23[32];
extern const char description24[32];
extern const char description25[32];
extern const char description26[32];
extern const char description27[32];
extern const char description28[32];

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
extern const char ICON16[128];
extern const char ICON17[128];
extern const char ICON18[128];
extern const char ICON19[128];
extern const char ICON20[128];
extern const char ICON21[128];
extern const char ICON22[128];
extern const char ICON23[128];
extern const char ICON24[128];
extern const char ICON25[128];
extern const char ICON26[128];
extern const char ICON27[128];
extern const char ICON28[128];

extern const char path[128];

int S_ICONS[28]={
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
(int)ICON16,
(int)ICON17,
(int)ICON18,
(int)ICON19,
(int)ICON20,
(int)ICON21,
(int)ICON22,
(int)ICON23,
(int)ICON24,
(int)ICON25,
(int)ICON26,
(int)ICON27,
(int)ICON28
//(int)NO_ICON
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
extern const char BM16FILE[128];
extern const char BM17FILE[128];
extern const char BM18FILE[128];
extern const char BM19FILE[128];
extern const char BM20FILE[128];
extern const char BM21FILE[128];
extern const char BM22FILE[128];
extern const char BM23FILE[128];
extern const char BM24FILE[128];
extern const char BM25FILE[128];
extern const char BM26FILE[128];
extern const char BM27FILE[128];
extern const char BM28FILE[128];


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
extern const char BMNAME16[32];
extern const char BMNAME17[32];
extern const char BMNAME18[32];
extern const char BMNAME19[32];
extern const char BMNAME20[32];
extern const char BMNAME21[32];
extern const char BMNAME22[32];
extern const char BMNAME23[32];
extern const char BMNAME24[32];
extern const char BMNAME25[32];
extern const char BMNAME26[32];
extern const char BMNAME27[32];
extern const char BMNAME28[32];



extern const int name_cl;//цвет имен закладок
extern const int description_cl;//цвет описаний закладок
extern const int ACTIVE_KEY_1;
extern const int ACTIVE_KEY_STYLE;
extern const int ENA_LOCK;
extern const int IDLE_MODE;
extern void InitConfig();
extern const int iconSize;
extern const int VIEW_MODE;
extern const int LINE_MODE;


extern const int isSystemFont;
extern const unsigned int pngFont;//=FONT_SMALL;
extern const char fontColor[];//Для системного фонта
extern const char bgrColor[];//Для системного фонта
extern const int isOutline;//Для системного фонта
extern const char fontpath[];//путь к кнг щрифту
extern const int fullfont;//если полный шрифт
extern const unsigned int space;
/*
extern const unsigned int pngFont_2;
extern const int isSystemFont_2;
extern const char fontColor_2[];//Для системного фонта
extern const char bgrColor_2[];//Для системного фонта
extern const int isOutline_2;//Для системного фонта
extern const char fontpath_2[];//путь к кнг щрифту
extern const int fullfont_2;//если полный шрифт
extern const unsigned int space_2;
*/

extern const char CursorClr[4];
extern const char CursorBGClr[4];
extern const int isGradient;
//extern const char CursorClr2[4];
//extern const char CursorBGClr2[4];

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}



int IsIdle(void)
{ 
  void *icsm=FindCSMbyID(CSM_root()->idle_id); 
  if (IsGuiOnTop(((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4 ])) return(1); 
  else return(0); 
}


const char *bmname(int bm)//возвращает имена для закладок
{
  switch(bm)
  {
  case 0: return BMNAME1;
  case 1: return BMNAME2;
  case 2: return BMNAME3;
  case 3: return BMNAME4;
  case 4: return BMNAME5;
  case 5: return BMNAME6;
  case 6: return BMNAME7;
  case 7: return BMNAME8;
  case 8: return BMNAME9;
  case 9: return BMNAME10;
  case 10: return BMNAME11;
  case 11: return BMNAME12;
  case 12: return BMNAME13;
  case 13: return BMNAME14;
  case 14: return BMNAME15;
  case 15: return BMNAME16;
  case 16: return BMNAME17;
  case 17: return BMNAME18;
  case 18: return BMNAME19;
  case 19: return BMNAME20;
  case 20: return BMNAME21;
  case 21: return BMNAME22;
  case 22: return BMNAME23;
  case 23: return BMNAME24;
  case 24: return BMNAME25;
  case 25: return BMNAME26;
  case 26: return BMNAME27;
  case 27: return BMNAME28;
  }
  return(0);
}

const char *description(int bm)//Описание для закладок
{
  switch(bm)
  {
  case 0: return description1;
  case 1: return description2;
  case 2: return description3;
  case 3: return description4;
  case 4: return description5;
  case 5: return description6;
  case 6: return description7;
  case 7: return description8;
  case 8: return description9;
  case 9: return description10;
  case 10: return description11;
  case 11: return description12;
  case 12: return description13;
  case 13: return description14;
  case 14: return description15;
  case 15: return description16;
  case 16: return description17;
  case 17: return description18;
  case 18: return description19;
  case 19: return description20;
  case 20: return description21;
  case 21: return description22;
  case 22: return description23;
  case 23: return description24;
  case 24: return description25;
  case 25: return description26;
  case 26: return description27;
  case 27: return description28;
  }
  return(0);
}

const char *bmfile(int bm)//вызов файлов
{
  switch(bm)
  {
  case 0: return BM1FILE;
  case 1: return BM2FILE;
  case 2: return BM3FILE;
  case 3: return BM4FILE;
  case 4: return BM5FILE;
  case 5: return BM6FILE;
  case 6: return BM7FILE;
  case 7: return BM8FILE;
  case 8: return BM9FILE;  
  case 9: return BM10FILE; 
  case 10: return BM11FILE;
  case 11: return BM12FILE;
  case 12: return BM13FILE;
  case 13: return BM14FILE;
  case 14: return BM15FILE;
  case 15: return BM16FILE;
  case 16: return BM17FILE;
  case 17: return BM18FILE;
  case 18: return BM19FILE;
  case 19: return BM20FILE;
  case 20: return BM21FILE;
  case 21: return BM22FILE;
  case 22: return BM23FILE;
  case 23: return BM24FILE;
  case 24: return BM25FILE;
  case 25: return BM26FILE;
  case 26: return BM27FILE;
  case 27: return BM28FILE;
  }
  return(0);
}

const char *icon(int bm)//вызов файлов
{
  switch(bm)
  {
  case 0: return ICON1;
  case 1: return ICON2;
  case 2: return ICON3;
  case 3: return ICON4;
  case 4: return ICON5;
  case 5: return ICON6;
  case 6: return ICON7;
  case 7: return ICON8;
  case 8: return ICON9;  
  case 9: return ICON10; 
  case 10: return ICON11;
  case 11: return ICON12;
  case 12: return ICON13;
  case 13: return ICON14;
  case 14: return ICON15;
  case 15: return ICON16;
  case 16: return ICON17;
  case 17: return ICON18;
  case 18: return ICON19;
  case 19: return ICON20;
  case 20: return ICON21;
  case 21: return ICON22;
  case 22: return ICON23;
  case 23: return ICON24;
  case 24: return ICON25;
  case 25: return ICON26;
  case 26: return ICON27;
  case 27: return ICON28;
  }
  return(0);
}

#endif
