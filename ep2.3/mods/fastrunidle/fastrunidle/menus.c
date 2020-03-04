// Меню
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

#pragma inline
void patch_rect(const RECT*rcc,int x,int y, int x2, int y2)
{
  RECT *rc=(RECT *)rcc;
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

int CalcPic(const char* picture)
{
  int pic = strtoul(picture, 0, 10);
  if ( (pic <= 4) && (strlen(picture) > 1) )
  {
     pic = (int)picture;
  }
  return pic;
}

void DrawIdleMenu()// На IDLE
{
    void *canvasdata=BuildCanvas();
    if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()))
    {
      DrawCanvas( canvasdata , x1,  y1, x1+LEN_ID,  y1 + idle_pic_size + 2 + GetFontYSIZE(TEXT_FONT), 1 );
      DrawRoundedFrame( x1,  y1,x1+LEN_ID,  y1 + idle_pic_size + 2 + GetFontYSIZE(TEXT_FONT),
                       0, 0, 0, cfgPanBGCol_ID_ena, cfgPanBGCol_ID_ena );
    }
    if(((!IdleMenuActive)&&(IDLE_CALL_BUTTON)&&(IDLE_EXIT_BUTTON))||(!IsUnlocked()))
    {
      DrawCanvas( canvasdata , x1,  y1, x1+LEN_ID,  y1 + idle_pic_size + 2, 1 );
      DrawRoundedFrame( x1,  y1,x1+LEN_ID,  y1 + idle_pic_size + 2,
                       0, 0, 0, cfgPanBGCol_ID_dis, cfgPanBGCol_ID_dis );
    }
    int i;
    int z = 0;
    int y = 0;
    if ( NumOfItems_idle == 0 ) return; 
    if( NumOfItems_idle > ( LEN_ID / (idle_pic_size + 2) )) i = Selected_idle - ( LEN_ID / (idle_pic_size + 2) ) + 1; 
    else i = 0;
    if ( i < 0 ) i = 0;
    
    while ( i < NumOfItems_idle )
    {
      if (i==Selected_idle )
      {
        if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked())) 
          DrawRoundedFrame( y + x1 + 1,  y1 + 1, y + x1 + idle_pic_size + 2,  y1 + idle_pic_size + 2,
                           2, 2, 0, cfgBookBorderCol_ID, cfgBookBGCol_ID );
          DrawImg( y + x1 + 2, y1 + 2, CalcPic(bm_pic_idle(i)));
        if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()))
        {
          WSHDR * ws = AllocWS( 256 );
          if ((bm_name_idle(i))&&(strlen(bm_name_idle(i)))) wsprintf(ws, percent_t, bm_name_idle(i) );
          else wsprintf(ws, percent_t, LG_NONAME);
          DrawString( ws, x1,  y1 + idle_pic_size + 4, x1+LEN_ID,  y1 + idle_pic_size + 4 + GetFontYSIZE(TEXT_FONT), TEXT_FONT, 2, 
                     cfgFontCol_ID, GetPaletteAdrByColorIndex( 23 ) );
          FreeWS(ws);
        }
      }
      else
      {
          DrawImg( y + x1 + 2, y1 + 2, CalcPic(bm_pic_idle(i)));
      }
      i ++;
      y += idle_pic_size + 2;
      z ++;
      if ( z > ( LEN_ID / (idle_pic_size + 2)) - 1 ) break;
    }
}

void MenuInit()
{
  scrH=ScreenH();
  scrW=ScreenW();
  NumOfItems_idle = 0;
  Selected_idle=0;
  if (IDLE_ENA)// Считаем число закладок
  {
    for (int i=0;i<16;i++)
    {
      const char *s=bm_file_idle(i);
      if ((s)&&(strlen(s)))NumOfItems_idle++;
    }
  }

  switch (IDLE_PIC_SIZE)//Размер иконок
  {
    case 0: idle_pic_size = 16; break;
    case 1: idle_pic_size = 24; break;
    case 2: idle_pic_size = 32; break;
    case 3: idle_pic_size = 48; break;
    case 4: idle_pic_size = 64; break;
  }
 
}
