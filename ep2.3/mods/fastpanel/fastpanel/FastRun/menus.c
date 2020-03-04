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
  NumOfItems_fr = 0;
  Selected_idle=0;
  Selected_fr=0;
  FastRunMenuActive = 0;
  if (IDLE_ENA)// Считаем число закладок
  {
    for (int i=0;i<16;i++)
    {
      const char *s=bm_file_idle(i);
      if ((s)&&(strlen(s)))NumOfItems_idle++;
    }
  }
  if (FASTRUN_ENA)// Считаем число закладок
  {
    for (int i=0;i<16;i++)
    {
      const char *h=bm_file_fr(i);
      if ((h)&&(strlen(h)))NumOfItems_fr++;
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
  switch (FR_PIC_SIZE)
  {
    case 0: fr_pic_size = 16; break;
    case 1: fr_pic_size = 24; break;
    case 2: fr_pic_size = 32; break;
    case 3: fr_pic_size = 48; break;
    case 4: fr_pic_size = 64; break;
  }
}

void InitFastRunMenu()
{
  //Left
  if(DRAW_MODE==0)
  { x1_fr=0;                    y1_fr=BEGIN;
    x2_fr=fr_pic_size+5;        y2_fr=BEGIN+LEN; }
  
  //Right
  if(DRAW_MODE==1)
  { x1_fr=scrW-(fr_pic_size+6); y1_fr=BEGIN;
    x2_fr=(scrW-1);             y2_fr=BEGIN+LEN; }
  
  //Top
  if(DRAW_MODE==2)
  { x1_fr=BEGIN;                y1_fr=0; 
    x2_fr=BEGIN+LEN;            y2_fr=fr_pic_size+5; }
  
  //Bottom
  if(DRAW_MODE==3)
  { x1_fr=BEGIN;                y1_fr=scrH-(fr_pic_size+6);
    x2_fr=BEGIN+LEN;            y2_fr=(scrH-1); }
}

void FastRunMenuOnRedraw(MAIN_GUI *data)
{
  void *canvasdata=BuildCanvas();
  DrawCanvas(canvasdata, x1_fr, y1_fr, x2_fr, y2_fr, 1 );
  DrawRoundedFrame( x1_fr, y1_fr, x2_fr, y2_fr, 2, 2, 0, cfgPanBorderCol, cfgPanBGCol );
    
  int i, z=0, y=0;
  if(!NumOfItems_fr) return;
  if(NumOfItems_fr>(LEN/(fr_pic_size+2)))
    i=Selected_fr-(LEN/(fr_pic_size+2))+1;
  else
    i=0;
  if (i<0) i=0;
  
  while (i<NumOfItems_fr)
  {
    if((DRAW_MODE==0)||(DRAW_MODE==1))
    {
      if ( i == Selected_fr )
      {
        DrawRoundedFrame( x1_fr + 2, y + y1_fr + 2, x1_fr + (fr_pic_size+3), y + y1_fr + (fr_pic_size+3), 2, 2, 0, cfgBookBorderCol, cfgBookBGCol );
        if ((bm_pic_fr(i))&&(strlen(bm_pic_fr(i))))
          DrawImg( x1_fr + 3, y + y1_fr + 3, CalcPic(bm_pic_fr(i)));
        else
        {
          DrawLine( x1_fr + 4 , y + y1_fr + 4 , x1_fr + (fr_pic_size+1), y + y1_fr + (fr_pic_size+1), 0,GetPaletteAdrByColorIndex(2));
          DrawLine( x1_fr + (fr_pic_size+1), y + y1_fr + 4, x1_fr + 4 ,y + y1_fr + (fr_pic_size+1) , 0,GetPaletteAdrByColorIndex(2));
        }
      }
      else
      {
        if ((bm_pic_fr(i))&&(strlen(bm_pic_fr(i))))
          DrawImg( x1_fr + 3, y + y1_fr + 3, CalcPic(bm_pic_fr(i)));
        else
        {
          DrawLine( x1_fr + 4 , y + y1_fr + 4 , x1_fr + 17, y + y1_fr + 17, 0,GetPaletteAdrByColorIndex(2));
          DrawLine( x1_fr + 17, y + y1_fr + 4, x1_fr + 4 ,y + y1_fr + 17 , 0,GetPaletteAdrByColorIndex(2));
        }
      }
    }
    if((DRAW_MODE==2)||(DRAW_MODE==3))
    {
      if ( i == Selected_fr )
      {
        DrawRoundedFrame( y + x1_fr + 2,  y1_fr + 2, y + x1_fr + (fr_pic_size+3),  y1_fr + (fr_pic_size+3), 2, 2, 0, cfgBookBorderCol, cfgBookBGCol );
        DrawImg( y + x1_fr + 3, y1_fr + 3, CalcPic(bm_pic_fr(i)));
      }
      else
        DrawImg( y + x1_fr + 3, y1_fr + 3, CalcPic(bm_pic_fr(i)));
    }
    i ++;
    y += (fr_pic_size+2);
    z ++;
    if (z>(LEN/(fr_pic_size+2))-1) break;
  }
}

void FastRunMenuOnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  InitFastRunMenu();
  FastRunMenuActive=1;
  data->gui.state=1;
}

void FastRunMenuOnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FastRunMenuActive=0;
  data->gui.state=0;
}

void FastRunMenuOnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
}

void FastRunMenuOnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  GeneralFunc_flag1(FASTRUNMENU_GUI_ID,1);// Если гуй не наверху, то мы его убиваем нах
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int FastRunMenuOnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  if ((msg->gbsmsg->msg==KEY_DOWN)||(msg->gbsmsg->msg==LONG_PRESS))// Если клавиша нажата или зажата
  {
    if((DRAW_MODE==0)||(DRAW_MODE==1))// Расположение панели. В данном случае слева или справа
    {
      switch(msg->gbsmsg->submess)
      {
        case DOWN_BUTTON:// Джой вниз
        case '8':
          Selected_fr++;// Курсор вперед
          if(Selected_fr >= NumOfItems_fr)Selected_fr=0;// Если уткнулись в конец, возврат в начало
          REDRAW();
        break;
       
        case UP_BUTTON:// Джой вверх
        case '2':
          Selected_fr--;// Курсор назад
          if(Selected_fr<0)Selected_fr=NumOfItems_fr-1; // Если пришли к началу, возвращаемся в конец
          REDRAW();
        break;
          
        case ENTER_BUTTON:// Джой вверх
        case '5':
          RunAction(Selected_fr,0);// Запускаем выделенную закладку
        return(1);
            
        case '#':
          EditConfig();
        return(1);
      }
    }
    else // Расположение панели ввреху или внизу. Почти то же самое... Только кнопки другие
    {
      switch(msg->gbsmsg->submess)
      {
        case RIGHT_BUTTON:
        case '6':
          Selected_fr++;
          if(Selected_fr >= NumOfItems_fr)Selected_fr=0;
          REDRAW();
        break;
       
        case LEFT_BUTTON:
        case '4':
          Selected_fr--;
          if(Selected_fr<0)Selected_fr=NumOfItems_fr-1;
          REDRAW();
        break;
          
        case ENTER_BUTTON:// Джой вверх
        case '5':
          RunAction(Selected_fr,0);// Запускаем выделенную закладку
        return(1);
            
        case '#':
          EditConfig();
        return(1);
      }
    }
  }
  if (msg->gbsmsg->msg==KEY_DOWN)// Если клавиша нажата или зажата
  {
    if (msg->gbsmsg->submess==FR_EXIT_BUTTON)
      return(1);
  }
  return(0);
}

void FastRunMenuOnDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(data,mfree_adr);
}

int method8(void){return(0);}
int method9(void){return(0);}

const void * const gui_methods_2[11]={
  (void *)FastRunMenuOnRedraw,  //Redraw
  (void *)FastRunMenuOnCreate,  //Create
  (void *)FastRunMenuOnClose,	  //Close
  (void *)FastRunMenuOnFocus,	  //Focus
  (void *)FastRunMenuOnUnfocus, //Unfocus
  (void *)FastRunMenuOnKey,     //OnKey
  0,
  (void *)FastRunMenuOnDestroy, //Destroy
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,0,0};

void RunFastRunPanel()
{
  if (NumOfItems_fr > 0)
  {
    MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(void *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods_2;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
    FASTRUNMENU_GUI_ID=CreateGUI(main_gui);
  }
  else ShowMSG(1,(int)LG_SETBOOKS_FR);
}
