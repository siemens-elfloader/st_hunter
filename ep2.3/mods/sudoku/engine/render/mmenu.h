

extern int checkgame();
extern void getLoadName();
extern void getTimeSave();

extern char *loadhelp();

extern char sudoku[];
extern char bgm[];

extern char sudokuname[32];
extern char level;
extern char sudokuname_ng[32];
extern char level_ng;

extern char timesave[5][20];
extern char loadname[5][32];

extern int gcell[9][9];

extern int mmenuc=7;
extern int smenuc=5;
extern int mm=0;
extern int sm=0;
extern int lm=0;

//Help vars
extern int helpvar;
extern int helphight;
extern int yhlp;

int menu_x_adj=30;
int menu_y_adj=0;
char menu_str[7][16]={"Новая игра","Загрузить","Настройки","Рекорды","Помощь","О игре","Выход"};
char smenu_str[5][16];


void MMenuRender()
{
  
  int w, h, wt, cmy,csx;

    
  for (int i=0;i<mmenuc;i++)
    {
      WSHDR *s = AllocWS(16);
      wsprintf(s,menu_str[i]);
      if (wt < Get_WS_width(s, bcfg_mmenu_font)) wt = Get_WS_width(s, bcfg_mmenu_font);   
      FreeWS(s);
    }
  
  w = wt + menu_x_adj;
  h = GetFontYSIZE(bcfg_mmenu_font) + menu_y_adj;
  
  cmy = SCRH/2 - (h*mmenuc)/2;
  csx = SCRW/2 - ((GetImgWidth((int)sudoku)))/2;
  
  
  
  DrawImg(0, 0, (int)bgm);
  DrawImg(csx,2,(int)sudoku); 

   for (int k = 0; k < mmenuc; k++) for (int i=0; i<(9*SCRW/10); i++)                           
     {   
       int a = 100 - (i*100)/(9*SCRW/10);
       
       char color[2][4]={{bcfg_mmenu_bg1_col[0],bcfg_mmenu_bg1_col[1],bcfg_mmenu_bg1_col[2],a},{bcfg_mmenu_bg2_col[0],bcfg_mmenu_bg2_col[1],bcfg_mmenu_bg2_col[2],a}};
             
       if (k==mm)  DrawRoundedFrame(i, cmy + k*h, i,cmy+k*h + h,0,0,0,color[1],color[1]);
         else  DrawRoundedFrame(i, cmy + k*h, i,cmy+k*h + h,0,0,0,color[0],color[0]);
    
     }
    
    for (int k = 0; k < mmenuc; k++)
    {
           WSHDR *ws = AllocWS(16);
           wsprintf(ws,t,menu_str[k]);
           DrawString(ws,2,cmy + k*h, w,cmy+ k*h+h, bcfg_mmenu_font,0,bcfg_mmenu_col,alpha);
           FreeWS(ws);
    }
   

    if (gcell[0][0]!=0) DrawRightSoft("Продолжить"); else DrawRightSoft("Обзор");

}




void MMenuRender_NewGame()
{
  
  DrawImg(0, 0, (int)bgm);
 
  if (gcell[0][0]!=0) DrawSoftHeader("Обзор","Играть","Новая игра"); else DrawSoftHeader("Обзор","Назад","Новая игра");

    DrawSudokuLevel("Cложность","Название", FONT_SMALL);
}

void MMenuRender_LoadGame()
{
  
  int h,cmy;
  
  h = GetFontYSIZE(bcfg_smenu_font) + menu_y_adj;
  if (SCRH>140) cmy = (SCRH/2 - (h*smenuc)/2); else cmy = (SCRH/2 - (h*smenuc)/2) + h;
  
  DrawImg(0, 0, (int)bgm);
  
 if (checkgame(sm+1)==1) DrawSoftHeader("Назад","Играть","Загрузить"); else DrawSoftHeader("Назад",0,"Загрузить");
    
    
     for (int k = 0; k < smenuc; k++) for (int i=0; i<(9*SCRW/10); i++)                           
     {   
       int a = 100 - (i*100)/(9*SCRW/10);
       
       char color[2][4]={{bcfg_smenu_bg1_col[0],bcfg_smenu_bg1_col[1],bcfg_smenu_bg1_col[2],a},{bcfg_smenu_bg1_col[0],bcfg_smenu_bg2_col[1],bcfg_smenu_bg2_col[2],a}};
             
       if (k==sm)  DrawRoundedFrame(i, cmy + k*h, i,cmy+k*h + h,0,0,0,color[1],color[1]);
         else  DrawRoundedFrame(i, cmy + k*h, i,cmy+k*h + h,0,0,0,color[0],color[0]);
    
     }
     
     
     WSHDR *tdt=AllocWS(64);
     if (checkgame(sm+1)==1) wsprintf(tdt,"%t:\n%t","Дата сохранения",timesave[sm]); else wsprintf(tdt,"%t:\n%t","Дата сохранения","<Пусто>");
     DrawString(tdt,2,h+2, SCRW,SCRH, bcfg_smenu_font,TEXT_OUTLINE ,bcfg_smenu_col,black);
     FreeWS(tdt);
     
    
    for (int k = 0; k < 5; k++)
    {
      WSHDR *ws = AllocWS(128);
      if (checkgame(k+1)==1) wsprintf(ws,t,loadname[k]); else wsprintf(ws,t,"<Пусто>");
      DrawString(ws,2,cmy + k*h, 9*SCRW/10,cmy+ k*h+h, bcfg_smenu_font,0,bcfg_smenu_col,alpha);
      FreeWS(ws);
    }
    
    

}


void MMenuRender_SaveGame()
{
  
  int h,cmy;
  
  h = GetFontYSIZE(bcfg_smenu_font) + menu_y_adj;
  if (SCRH>140) cmy = (SCRH/2 - (h*smenuc)/2); else cmy = (SCRH/2 - (h*smenuc)/2) + h;
  
  DrawImg(0, 0, (int)bgm);
  
 if (checkgame(lm+1)==1) DrawSoftHeader("Назад","Реврайт","Сохранить"); else DrawSoftHeader("Назад","Сохр.","Сохранить");
    
    
     for (int k = 0; k < smenuc; k++) for (int i=0; i<(9*SCRW/10); i++)                           
     {   
       int a = 100 - (i*100)/(9*SCRW/10);
       
       char color[2][4]={{bcfg_smenu_bg1_col[0],bcfg_smenu_bg1_col[1],bcfg_smenu_bg1_col[2],a},{bcfg_smenu_bg2_col[0],bcfg_smenu_bg2_col[1],bcfg_smenu_bg2_col[2],a}};
             
       if (k==lm)  DrawRoundedFrame(i, cmy + k*h, i,cmy+k*h + h,0,0,0,color[1],color[1]);
         else  DrawRoundedFrame(i, cmy + k*h, i,cmy+k*h + h,0,0,0,color[0],color[0]);
    
     }
     
     
     WSHDR *tdt=AllocWS(64);
     if (checkgame(lm+1)==1) wsprintf(tdt,"%t:\n%t","Дата сохранения",timesave[lm]); else wsprintf(tdt,"%t:\n%t","Дата сохранения","<Пусто>");
     DrawString(tdt,2,h+2, SCRW,SCRH, bcfg_smenu_font,TEXT_OUTLINE ,bcfg_smenu_col,black);
     FreeWS(tdt);
     
    
    for (int k = 0; k < 5; k++)
    {
      WSHDR *ws = AllocWS(128);
      if (checkgame(k+1)==1) wsprintf(ws,t,loadname[k]); else wsprintf(ws,t,"<Пусто>");
      DrawString(ws,2,cmy + k*h, 9*SCRW/10,cmy+ k*h+h, bcfg_smenu_font,0,bcfg_smenu_col,alpha);
      FreeWS(ws);
    }
    
    

}

void MMenuRender_Records()
{

     DrawImg(0, 0, (int)bgm); 
     
     DrawSoftHeader(0,"Назад","Рекорды");
    

}


void MMenuRender_About()
{

     DrawImg(0, 0, (int)bgm); 
     
     DrawSoftHeader(0,"Назад","О игре");
    
    //Автор
     DrawAboutItem("Автор", ELF_AUTHOR, 1);

    //Название
     DrawAboutItem("Название", ELF_NAME, 2);
     
    //Версия
     DrawAboutItem("Версия", ELF_VERSION, 3);
     
    //Платформа
     if (SCRH>140)
     { 
       if (isnewSGold()==0) DrawAboutItem("Платформа", "SGold", 4);
       if (isnewSGold()==1) DrawAboutItem("Платформа", "NewSGold", 4);
       if (isnewSGold()==2) DrawAboutItem("Платформа", "NewSGold (E/EL71)", 4);
     }
}

void MMenuRender_Help()
{
     char hlps[4096];
  
     DrawImg(0, 0, (int)bgm); 
     
     DrawSoftHeader(0,"Назад","Помощь");
    
     int h1 = GetFontYSIZE(bcfg_mmenu_sk_font);
     int h2 = GetFontYSIZE(bcfg_mmenu_head_font);
     
     int sow = GetSymbolWidth((SCRW - 3) - 4,bcfg_help_font);
     
     WSHDR *hlp=AllocWS(4096);
     ascii2ws(hlp,loadhelp(hlps));

     helphight = Get_WS_width(hlp, bcfg_mmenu_sk_font)/(sow+3);
     
     char halfwhite[4]={255,255,255,80};
     DrawRoundedFrame(2, h2 + 2, SCRW - 3, SCRH - ( h1 + 4),0,0,0,black,halfwhite);
        

     DrawString(hlp, 4, h2 + 2 + yhlp, SCRW - 3, SCRH - ( h1 + 6), bcfg_help_font,0,bcfg_help_col,alpha);
     FreeWS(hlp);
     
     if (helpvar==1) DrawLastScreenPart(bcfg_mmenu_head_font,4);
     
     helpvar=1;
     
}


