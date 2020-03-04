#ifndef _MAIN_H_
  #define _MAIN_H_

//unsigned int MAINCSM_ID = 0;
//unsigned int MAINGUI_ID = 0;

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
/*
int IsIdle(void)
{ 
  void *icsm=FindCSMbyID(CSM_root()->idle_id); 
  if (IsGuiOnTop(((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4 ])) return(1); 
  else return(0); 
}*/


/*
//===============================================================================================
#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}
//===============================================================================================
*/


#endif
