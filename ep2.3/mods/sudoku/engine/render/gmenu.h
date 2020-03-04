
extern char t[];

extern int gmenuc=5;
extern int gm=0;
int gmenu_x_adj=0;
int gmenu_y_adj=0;
char gmenu_str[5][16]={"Проверка","Просмотр","Загрузить","Сохранить","Г. Меню"};



void GMenuRender()
{
 
  int w, h, wt;

  
      WSHDR *s = AllocWS(32);
      wsprintf(s,gmenu_str[2]);
      wt = Get_WS_width(s, bcfg_gmenu_font);   
      FreeWS(s);
  
  w = wt + gmenu_x_adj;
  h = GetFontYSIZE(bcfg_gmenu_font) + gmenu_y_adj;
  
  
  

   for (int k = 0; k < gmenuc; k++) for (int i=0; i<w; i++)                           
     {   
       int a = 100 - (i*100)/w;
       
       char color[2][4]={{bcfg_gmenu_bg1_col[0],bcfg_gmenu_bg1_col[1],bcfg_gmenu_bg1_col[2],a},{bcfg_gmenu_bg2_col[0],bcfg_gmenu_bg2_col[1],bcfg_gmenu_bg2_col[2],a}};
             
       if (k==gm)  DrawRoundedFrame(i + (SCRW - w - 10), SCRH - (gmenuc - k)*h - 2, i + (SCRW - w - 10),SCRH - (gmenuc - k)*h - h - 2,0,0,0,color[1],color[1]);
         else  DrawRoundedFrame(i + (SCRW - w - 10), SCRH - (gmenuc - k)*h - 2, i + (SCRW - w - 10),SCRH - (gmenuc - k)*h - h - 2,0,0,0,color[0],color[0]);
    
     }
    
    for (int k = 0; k < gmenuc; k++)
    {
           WSHDR *ws = AllocWS(16);
           wsprintf(ws,t,gmenu_str[k]);
           DrawString(ws,(SCRW - w - 10) + 2, SCRH - ((gmenuc + 1) - k)*h - 2,SCRW,(SCRH - ((gmenuc + 1) - k)*h) + h, bcfg_gmenu_font,0,bcfg_gmenu_col,alpha);
           FreeWS(ws);
    }
   

}
   




