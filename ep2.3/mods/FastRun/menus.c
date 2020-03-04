// Меню
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
extern void PrintField( int x, int y, char *str, int align, int font, int space); //вывод строки

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
  if(picture[0]=='#') return strtoul(picture+1, 0, 10);
  if ((pic <= 4)&&(strlen(picture) > 1))
  {
     pic = (int)picture;
  }
  return pic;
}

const char *GetName(char *file)
{
  char *p=strrchr(file,'\\');
  WSHDR *ws;
  char *name=malloc(64);
  if (p)
   {
     unsigned int pos;
     int l;
     ws=AllocWS((l=strlen(p+1)));
     str_2ws(ws,p+1,l);
     if ((pos=wstrrchr(ws,ws->wsbody[0],'.'))!=0xFFFF)
         {
           CutWSTR(ws,pos-1);
         }
     ws_2str(ws,name,wstrlen(ws));
   }
  else if(file[2]!='\\'&&(file[(strlen(file))-3]!='.')&&//Если не файл то просто пишем в имя шорткат или энтрик
      (file[(strlen(file))-4]!='.')&&(file[(strlen(file))-5]!='.'))
      strcpy(name,file);
 return name;
}

void DrawIdleMenu()// На IDLE, есди панель не активна
{
  //  canvasdata=BuildCanvas();
    FASTRUN_IDLE *fl;
    if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()))
    {
      if(IDLE_POSITION)//horizontal
       {
         DrawCanvas( canvasdata , x1,  y1, x1+LEN_ID,  y1 + idle_pic_size+2, 1 );
         DrawRoundedFrame(x1,y1,x1+LEN_ID, y1 + idle_pic_size + 2,0,0,0,cfgPanBGCol_ID_ena, cfgPanBGCol_ID_ena );        
       }
      else //vertical
       {
         DrawCanvas( canvasdata , x1,  y1, x1+idle_pic_size+2,  y1 + LEN_ID, 1 );
         DrawRoundedFrame(x1,y1,x1+idle_pic_size + 2, y1 + LEN_ID,0,0,0,cfgPanBGCol_ID_ena, cfgPanBGCol_ID_ena );        
       }
    }
    
    if(IDLE_POSITION)
    {
     if(((!IdleMenuActive)&&(IDLE_CALL_BUTTON)&&(IDLE_EXIT_BUTTON))||(!IsUnlocked()))
       {
         DrawCanvas( canvasdata , x1,  y1, x1+LEN_ID,  y1 + idle_pic_size + 2, 1 );
         DrawRoundedFrame( x1, y1,x1+LEN_ID,  y1 + idle_pic_size + 2,0, 0, 0, cfgPanBGCol_ID_dis, cfgPanBGCol_ID_dis );
       }
    }
    else
    {
      if(((!IdleMenuActive)&&(IDLE_CALL_BUTTON)&&(IDLE_EXIT_BUTTON))||(!IsUnlocked()))
      {
      DrawCanvas( canvasdata , x1,  y1, x1+idle_pic_size + 2,  y1 + LEN_ID, 1 );
      DrawRoundedFrame( x1,  y1,x1+ idle_pic_size + 2,  y1 +LEN_ID, 0, 0, 0, cfgPanBGCol_ID_dis, cfgPanBGCol_ID_dis );
      }
    }
    
    RECT rc;
    int i;
    int z = 0;
    int y = 0;
    WSHDR *ws=AllocWS(256);
    char *s=malloc(256);
    if ( NumOfItems_idle == 0 ) return; 
    if( NumOfItems_idle > ( LEN_ID / (idle_pic_size + 2) )) i = Selected_idle - ( LEN_ID / (idle_pic_size + 2) ) + 1; 
    else i = 0;
    if ( i < 0 ) i = 0;    
    while ( i < NumOfItems_idle )
    {   
      fl=FindByN_IDLE(i);
      if (i==Selected_idle)
      {
        if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()) && IDLE_POSITION) 
        {
          //Горизонтально
          rc.x=y+x1+1;
          rc.y=y1+1;
          rc.x2=y+x1+idle_pic_size+2;
          rc.y2=y1+idle_pic_size+2;
          
          DrawRoundedFrame( rc.x,rc.y,rc.x2,rc.y2 ,2, 2, 0, cfgBookBorderCol_ID, cfgBookBGCol_ID );
          DrawImg( y + x1 + 2, y1 + 2, CalcPic(fl->icon));
        }
        else if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()) && (!IDLE_POSITION))
        {
          //вертикально
          rc.x=x1+1;
          rc.y=y+y1+1;
          rc.x2=x1+idle_pic_size+2;
          rc.y2=y+y1+idle_pic_size+2;
          
          if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked())) 
          DrawRoundedFrame( rc.x,rc.y,rc.x2,rc.y2, 2, 2, 0, cfgBookBorderCol_ID, cfgBookBGCol_ID );
          DrawImg( x1 + 2,y + y1 + 2, CalcPic(fl->icon));
        }
        
        if(((!IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()) && (IDLE_POSITION)) 
          DrawImg( y + x1 + 2, y1 + 2, CalcPic(fl->icon));
        else
         if(((!IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()) && (!IDLE_POSITION))
          DrawImg( x1 + 2, y+y1 + 2, CalcPic(fl->icon));
        
        if(((IdleMenuActive)||((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON)))&&(IsUnlocked()) && (IDLE_NAME_MODE))//Показываем имя
        {          
          if(fl->name) sprintf(s, percent_s,fl->name);
          else 
            {
              //wsprintf(ws,percent_t,GetName(fl->action));//Если не прописано имя то выводится имя файла
              sprintf(s,percent_s,GetName(fl->action));//Если не прописано имя то выводится имя файла
            }
            rc.x=name_idle_x1;
            rc.y=name_idle_y1;
            //rc.x2=name_idle_x1+Get_WS_width(ws,TEXT_FONT);
            //rc.y2=name_idle_y1+GetFontYSIZE(TEXT_FONT);
            PrintField(rc.x, rc.y, s, align_idle, font_idle, space);
         //   DrawCanvas(canvasdata,rc.x-2,rc.y-2,rc.x2+2,rc.y2,1);
           // DrawRoundedFrame(rc.x-2,rc.y-2,rc.x2+2,rc.y2+2,3,0,0,cfgPanBGCol_ID_ena, cfgPanBGCol_ID_ena );    
            //DrawString(ws,rc.x-2,rc.y-2,rc.x2+2,rc.y2+2,TEXT_FONT,2,cfgFontCol_ID, GetPaletteAdrByColorIndex(23) );          
        }        
      }// if(i==Selected_idle)
      else
      {
        if(IDLE_POSITION)  DrawImg( y + x1 + 2, y1 + 2, CalcPic(fl->icon));//Горизонтально
        else   DrawImg( x1 + 2,y + y1 + 2, CalcPic(fl->icon));
      }
      i ++;
      y += idle_pic_size + 2;
      z ++;
      if ( z > ( LEN_ID / (idle_pic_size + 2)) - 1 ) break;
   }
FreeWS(ws);
mfree(s);
}

//-------------------------------------------------------------------------------------------------//

/*
//Еще один вариант для борьбы с еменю))
void FastRunIdleOnRedraw(MAIN_GUI *data)
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  DrawIdleMenu(); 
}

void FastRunIdleOnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  FastRunMenuActive=1;
  data->gui.state=1;
}

void FastRunIdleOnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FastRunMenuActive=0;
  data->gui.state=0;
}

void FastRunIdleOnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
}

void FastRunIdleOnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  GeneralFunc_flag1(FASTRUNMENU_GUI_IDLE_ID,1);// Если гуй не наверху, то мы его убиваем нах
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int FastRunIdleOnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  if ((msg->gbsmsg->msg==KEY_DOWN)||(msg->gbsmsg->msg==LONG_PRESS) && IdleMenuActive)// Если клавиша нажата или зажата
  {
    if(msg->gbsmsg->submess==IDLE_EXIT_BUTTON)
    {
       IdleMenuActive=0;
       GeneralFunc_flag1(FASTRUNMENU_GUI_IDLE_ID,1);
       REDRAW();
    }
    
  switch(msg->gbsmsg->submess)
   {
    case LEFT_BUTTON:case UP_BUTTON:// Джой влево
    case '4':
         Selected_idle--;// Курсор назад
         if(Selected_idle<0)
         Selected_idle=NumOfItems_idle-1; // Если пришли к началу, возвращаемся в конец
         DirectRedrawGUI_ID(FASTRUNMENU_GUI_IDLE_ID); // И перерисовываем саму панель
         break;       
    case RIGHT_BUTTON:case DOWN_BUTTON:// Джой вправо
    case '6':
         Selected_idle++;// Курсор вперед
         if(Selected_idle >= NumOfItems_idle)
         Selected_idle=0;// Если уткнулись в конец, возврат в начало
         DirectRedrawGUI_ID(FASTRUNMENU_GUI_IDLE_ID); // Снова перерисовываем
         break;       
    case ENTER_BUTTON:// Тыкаем на джой
    case '5':
         IdleMenuActive=0;
         RunAction(Selected_idle,1);// И запускаем выделенную закладку
         GeneralFunc_flag1(FASTRUNMENU_GUI_IDLE_ID,1);  
         REDRAW();
         break;
    case '#':
         IdleMenuActive=0;
         EditConfig();
         GeneralFunc_flag1(FASTRUNMENU_GUI_IDLE_ID,1);
         REDRAW();
         break;
    case '0':
         ShowMSG(2,(int)LG_COPYRIGHT);
         break;
   }
 }
return 0;
}

void FastRunIdleOnDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(data,mfree_adr);
}

int method88(void){return(0);}
int method99(void){return(0);}

const void * const gui_methods_id[11]={
  (void *)FastRunIdleOnRedraw,  //Redraw
  (void *)FastRunIdleOnCreate,  //Create
  (void *)FastRunIdleOnClose,	  //Close
  (void *)FastRunIdleOnFocus,	  //Focus
  (void *)FastRunIdleOnUnfocus, //Unfocus
  (void *)FastRunIdleOnKey,     //OnKey
  0,
  (void *)FastRunIdleOnDestroy, //Destroy
  (void *)method88,
  (void *)method99,
  0
};
*/
//-------------------------------------------------------------------------------------------//

void MenuInit()
{
  NumOfItems_idle = 0;
  NumOfItems_fr = 0;
  NumOfItems_fr_2 = 0;
  NumOfItems_fr_3 = 0;
  Selected_idle=0;
  Selected_fr=0;
  Selected_fr_2=0;
  FastRunMenuActive = 0;
  FastRunMenuActive_2 = 0;
  
  if (IDLE_ENA)// Считаем число закладок
  {
    NumOfItems_idle=GetFListN_IDLE();
  }
  
  if (FASTRUN_ENA)// Считаем число закладок
  {
    NumOfItems_fr=GetFListN_1();
  }
 
  if (FASTRUN_ENA_2)// Считаем число закладок
  {
    NumOfItems_fr_2=GetFListN_2();
  }
  if (FASTRUN_ENA_3)// Считаем число закладок
  {
    NumOfItems_fr_3=GetFListN_3();
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
  switch (FR_PIC_SIZE_2)
  {
    case 0: fr_pic_size_2 = 16; break;
    case 1: fr_pic_size_2 = 24; break;
    case 2: fr_pic_size_2 = 32; break;
    case 3: fr_pic_size_2 = 48; break;
    case 4: fr_pic_size_2 = 64; break;
  }
  switch (FR_PIC_SIZE_3)
  {
    case 0: fr_pic_size_3 = 16; break;
    case 1: fr_pic_size_3 = 24; break;
    case 2: fr_pic_size_3 = 32; break;
    case 3: fr_pic_size_3 = 48; break;
    case 4: fr_pic_size_3 = 64; break;
  }
}


void FastRunMenuOnRedraw(MAIN_GUI *data)
{
#ifdef ELKA
  DisableIconBar(1);
#endif      
  int i, z=0, y=0;
  char *s;
  RECT rc;
  if(PANEL_FS==1)
  {
    if(DRAW_MODE)//Вертикально
     {
       rc.x=x1_fr;
       rc.y=y1_fr;
       rc.x2=x1_fr+fr_pic_size+5;
       rc.y2=y1_fr+LEN;
       
       DrawCanvas(canvasdata, rc.x, rc.y, rc.x2,rc.y2, 1 );
       DrawRoundedFrame( rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgPanBorderCol, cfgPanBGCol );
    }
    else//Горизонтально
    {
       rc.x=x1_fr;
       rc.y=y1_fr;
       rc.x2=x1_fr+LEN;
       rc.y2=y1_fr+fr_pic_size+5;
       
       DrawCanvas(canvasdata, rc.x, rc.y, rc.x2,rc.y2, 1 );
       DrawRoundedFrame( rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgPanBorderCol, cfgPanBGCol );
    }
    
  if(!NumOfItems_fr) return;
  if(NumOfItems_fr>(LEN/(fr_pic_size+2)))
     i=Selected_fr-(LEN/(fr_pic_size+2))+1;
  else i=0;
  if (i<0) i=0;
  FASTRUN_1 *fl;
  while (i<NumOfItems_fr)
  {
    fl=FindByN_1(i);
    if(DRAW_MODE==0)//Горизонтально
    {
       rc.x=y+x1_fr+2;
       rc.y=y1_fr+2;
       rc.x2=y+x1_fr+(fr_pic_size+3);
       rc.y2=y1_fr+(fr_pic_size+3);    
    }
    else//Vertical
    {
       rc.x=x1_fr+2;
       rc.y=y+y1_fr+2;
       rc.x2=x1_fr+(fr_pic_size+3);
       rc.y2=y+y1_fr+(fr_pic_size+3);    
    }      
    
    if ( i == Selected_fr )
    {
      DrawRoundedFrame(rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgBookBorderCol, cfgBookBGCol );
      if(FASTRUN_NAME_1)
      {
       if(fl->name)//Имя у панели fastrun
        {          
          s=malloc(strlen(fl->name));
          sprintf(s,per_s,fl->name);
        }
       else//если имя отсутствует то выводим имя файла
        {
          s=malloc(64);
          sprintf(s,per_s,GetName(fl->action));
        }
        rc.x=name_fr1_x1;
        rc.y=name_fr1_y1;
        PrintField(rc.x, rc.y, s, align_1, font_1, space);                 
      }
      
      if ((fl->icon)&&(strlen(fl->icon)))
      {
        if(DRAW_MODE==0) DrawImg( y+x1_fr+3, y1_fr+3, CalcPic(fl->icon));
        else DrawImg(x1_fr+3, y+y1_fr+3, CalcPic(fl->icon));
      }
      else 
      {
       if(DRAW_MODE==0)
        {
          rc.x=x1_fr;
          rc.y= y + y1_fr;
          rc.x2=x1_fr;
          rc.y2=y + y1_fr;
        }
       else
        {
          rc.x=y+x1_fr;
          rc.y=y1_fr;
          rc.x2=y+x1_fr;
          rc.y2=y1_fr;
        }
        DrawLine( rc.x+4 ,rc.y+4,rc.x2+(fr_pic_size+1),rc.y2+(fr_pic_size+1), 0,GetPaletteAdrByColorIndex(2));
        DrawLine( rc.x+(fr_pic_size+1),rc.y+4,rc.x2+4, rc.y2+(fr_pic_size+1), 0,GetPaletteAdrByColorIndex(2));
      }
    }
    else
    {
     if(fl->icon)//Если есть иконка то рисуем ее
      {
       if(DRAW_MODE==0) DrawImg( y+x1_fr+3, y1_fr+3, CalcPic(fl->icon));//Вертикально
       else DrawImg(x1_fr+3, y+y1_fr+3, CalcPic(fl->icon));//Горизонтально
      }
     else//если нет то рисуем красный крестик:)
      {
       if(DRAW_MODE==0)
       {
         rc.x=x1_fr;
         rc.y= y + y1_fr;
         rc.x2=x1_fr;
         rc.y2=y + y1_fr;
       }
       else
       {
         rc.x=y+x1_fr;
         rc.y=y1_fr;
         rc.x2=y+x1_fr;
         rc.y2=y1_fr;
       }
       DrawLine( rc.x+4 ,rc.y+4,rc.x2+17,rc.y2+17, 0,GetPaletteAdrByColorIndex(2));
       DrawLine( rc.x+17,rc.y+4,rc.x2+4, rc.y2+17, 0,GetPaletteAdrByColorIndex(2));
      }
    }
    i ++;
    y += (fr_pic_size+2);
    z ++;
    if (z>(LEN/(fr_pic_size+2))-1) break;
  }
}  
  
//----------------------------------------------------------------------------//
   
 if(PANEL_FS==2)//Вторая фастран панель
  {
   if(DRAW_MODE_2)//Вертикально
     {
       rc.x=x1_fr_2;
       rc.y=y1_fr_2;
       rc.x2=x1_fr_2+fr_pic_size_2+5;
       rc.y2=y1_fr_2+LEN_2;
       
       DrawCanvas(canvasdata, rc.x, rc.y, rc.x2,rc.y2, 1 );
       DrawRoundedFrame( rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgPanBorderCol, cfgPanBGCol );
    }
    else//Горизонтально
    {
       rc.x=x1_fr_2;
       rc.y=y1_fr_2;
       rc.x2=x1_fr_2+LEN_2;
       rc.y2=y1_fr_2+fr_pic_size_2+5;
       
       DrawCanvas(canvasdata, rc.x, rc.y, rc.x2,rc.y2, 1 );
       DrawRoundedFrame( rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgPanBorderCol, cfgPanBGCol );
    }
    
  if(!NumOfItems_fr_2) return;
  if(NumOfItems_fr_2>(LEN_2/(fr_pic_size_2+2)))
     i=Selected_fr_2-(LEN_2/(fr_pic_size_2+2))+1;
  else i=0;
  if (i<0) i=0;
  FASTRUN_2 *fl;
  while (i<NumOfItems_fr_2)
  {
    fl=FindByN_2(i);
    if(DRAW_MODE_2==0)//Горизонтально
    {
       rc.x=y+x1_fr_2+2;
       rc.y=y1_fr_2+2;
       rc.x2=y+x1_fr_2+(fr_pic_size_2+3);
       rc.y2=y1_fr_2+(fr_pic_size_2+3);    
    }
    else//Vertical
    {
       rc.x=x1_fr_2+2;
       rc.y=y+y1_fr_2+2;
       rc.x2=x1_fr_2+(fr_pic_size_2+3);
       rc.y2=y+y1_fr_2+(fr_pic_size_2+3);    
    }      
    
    if(i==Selected_fr_2)
    {
      DrawRoundedFrame(rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgBookBorderCol, cfgBookBGCol );

      if(FASTRUN_NAME_2)
      {
       if(fl->name)//Имя у панели fastrun
        {          
          s=malloc(strlen(fl->name));
          sprintf(s,per_s,fl->name);
        }
       else//если имя отсутствует то выводим имя файла
        {
          s=malloc(64);
          sprintf(s,per_s,GetName(fl->action));
        }
        rc.x=name_fr2_x1;
        rc.y=name_fr2_y1;
        PrintField(rc.x, rc.y, s, align_2, font_2, space);            
      }
      
      if(fl->icon)
      {
        if(DRAW_MODE_2==0) DrawImg( y+x1_fr_2+3, y1_fr_2+3, CalcPic(fl->icon));
        else DrawImg(x1_fr_2+3, y+y1_fr_2+3, CalcPic(fl->icon));
      }
      else 
      {
       if(DRAW_MODE_2==0)
        {
          rc.x=x1_fr_2;
          rc.y= y + y1_fr_2;
          rc.x2=x1_fr_2;
          rc.y2=y + y1_fr_2;
        }
       else
        {
          rc.x=y+x1_fr_2;
          rc.y=y1_fr_2;
          rc.x2=y+x1_fr_2;
          rc.y2=y1_fr_2;
        }
        DrawLine( rc.x+4 ,rc.y+4,rc.x2+(fr_pic_size+1),rc.y2+(fr_pic_size+1), 0,GetPaletteAdrByColorIndex(2));
        DrawLine( rc.x+(fr_pic_size+1),rc.y+4,rc.x2+4, rc.y2+(fr_pic_size+1), 0,GetPaletteAdrByColorIndex(2));
      }
    }
    else
    {
     if(fl->icon)//Если есть иконка то рисуем ее
      {
       if(DRAW_MODE_2==0) DrawImg( y+x1_fr_2+3, y1_fr_2+3, CalcPic(fl->icon));//Вертикально
       else DrawImg(x1_fr_2+3, y+y1_fr_2+3, CalcPic(fl->icon));//Горизонтально
      }
     else//если нет то рисуем красный крестик:)
      {
       if(DRAW_MODE_2==0)
       {
         rc.x=x1_fr_2;
         rc.y= y + y1_fr_2;
         rc.x2=x1_fr_2;
         rc.y2=y + y1_fr_2;
       }
       else
       {
         rc.x=y+x1_fr_2;
         rc.y=y1_fr_2;
         rc.x2=y+x1_fr_2;
         rc.y2=y1_fr_2;
       }
       DrawLine( rc.x+4 ,rc.y+4,rc.x2+17,rc.y2+17, 0,GetPaletteAdrByColorIndex(2));
       DrawLine( rc.x+17,rc.y+4,rc.x2+4, rc.y2+17, 0,GetPaletteAdrByColorIndex(2));
      }
    }
    i ++;
    y += (fr_pic_size_2+2);
    z ++;
    if (z>(LEN_2/(fr_pic_size_2+2))-1) break;
  }
 }   
 //---------------------------------------------------------------//
 
 if(PANEL_FS==3) //Третья фастран панель
  {
    if(DRAW_MODE_3)//Вертикально
     {
       rc.x=x1_fr_3;
       rc.y=y1_fr_3;
       rc.x2=x1_fr_3+fr_pic_size_3+5;
       rc.y2=y1_fr_3+LEN_3;
       
       DrawCanvas(canvasdata, rc.x, rc.y, rc.x2,rc.y2, 1 );
       DrawRoundedFrame( rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgPanBorderCol, cfgPanBGCol );
    }
    else//Горизонтально
    {
       rc.x=x1_fr_3;
       rc.y=y1_fr_3;
       rc.x2=x1_fr_3+LEN_3;
       rc.y2=y1_fr_3+fr_pic_size_3+5;
       
       DrawCanvas(canvasdata, rc.x, rc.y, rc.x2,rc.y2, 1 );
       DrawRoundedFrame( rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgPanBorderCol, cfgPanBGCol );
    }
    
  if(!NumOfItems_fr_3) return;
  if(NumOfItems_fr_3>(LEN_3/(fr_pic_size_3+2)))
     i=Selected_fr_3-(LEN_3/(fr_pic_size_3+2))+1;
  else i=0;
  if (i<0) i=0;
  FASTRUN_3 *fl;
  while (i<NumOfItems_fr_3)
  {
    fl=FindByN_3(i);
    if(DRAW_MODE_3==0)//Горизонтально
    {
       rc.x=y+x1_fr_3+2;
       rc.y=y1_fr_3+2;
       rc.x2=y+x1_fr_3+(fr_pic_size_3+3);
       rc.y2=y1_fr_3+(fr_pic_size_3+3);    
    }
    else//Vertical
    {
       rc.x=x1_fr_3+2;
       rc.y=y+y1_fr_3+2;
       rc.x2=x1_fr_3+(fr_pic_size_3+3);
       rc.y2=y+y1_fr_3+(fr_pic_size_3+3);    
    }      
    
    if ( i == Selected_fr_3 )
    {
      DrawRoundedFrame(rc.x, rc.y, rc.x2,rc.y2, 2, 2, 0, cfgBookBorderCol, cfgBookBGCol );
      if(FASTRUN_NAME_3)
      {
       if(fl->name)//Имя у панели fastrun
        {          
          s=malloc(strlen(fl->name));
          sprintf(s,per_s,fl->name);
        }
       else//если имя отсутствует то выводим имя файла
        {
          s=malloc(64);
          sprintf(s,per_s,GetName(fl->action));
        }
        rc.x=name_fr3_x1;
        rc.y=name_fr3_y1;
        PrintField(rc.x, rc.y, s, align_3, font_3, space);          
      }
      
      if(fl->icon)
      {
        if(DRAW_MODE_3==0) DrawImg( y+x1_fr_3+3, y1_fr_3+3, CalcPic(fl->icon));
        else DrawImg(x1_fr_3+3, y+y1_fr_3+3, CalcPic(fl->icon));
      }
      else 
      {
       if(DRAW_MODE_3==0)
        {
          rc.x=x1_fr_3;
          rc.y= y + y1_fr_3;
          rc.x2=x1_fr_3;
          rc.y2=y + y1_fr_3;
        }
       else
        {
          rc.x=y+x1_fr_3;
          rc.y=y1_fr_3;
          rc.x2=y+x1_fr_3;
          rc.y2=y1_fr_3;
        }
        DrawLine( rc.x+4 ,rc.y+4,rc.x2+(fr_pic_size+1),rc.y2+(fr_pic_size+1), 0,GetPaletteAdrByColorIndex(2));
        DrawLine( rc.x+(fr_pic_size+1),rc.y+4,rc.x2+4, rc.y2+(fr_pic_size+1), 0,GetPaletteAdrByColorIndex(2));
      }
    }
    else
    {
     if(fl->icon)//Если есть иконка то рисуем ее
      {
       if(DRAW_MODE_3==0) DrawImg( y+x1_fr_3+3, y1_fr_3+3, CalcPic(fl->icon));//Вертикально
       else DrawImg(x1_fr_3+3, y+y1_fr_3+3, CalcPic(fl->icon));//Горизонтально
      }
     else//если нет то рисуем красный крестик:)
      {
       if(DRAW_MODE==0)
       {
         rc.x=x1_fr_3;
         rc.y= y + y1_fr_3;
         rc.x2=x1_fr_3;
         rc.y2=y + y1_fr_3;
       }
       else
       {
         rc.x=y+x1_fr_3;
         rc.y=y1_fr_3;
         rc.x2=y+x1_fr_3;
         rc.y2=y1_fr_3;
       }
       DrawLine( rc.x+4 ,rc.y+4,rc.x2+17,rc.y2+17, 0,GetPaletteAdrByColorIndex(2));
       DrawLine( rc.x+17,rc.y+4,rc.x2+4, rc.y2+17, 0,GetPaletteAdrByColorIndex(2));
      }
    }
    i ++;
    y += (fr_pic_size_3+2);
    z ++;
    if (z>(LEN_3/(fr_pic_size_3+2))-1) break;
  }
 } 
}

void FastRunMenuOnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
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

extern void RunFastRunPanel();

int FastRunMenuOnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  if ((msg->gbsmsg->msg==KEY_DOWN)||(msg->gbsmsg->msg==LONG_PRESS))// Если клавиша нажата или зажата
  {
   if(msg->gbsmsg->submess=='#') { EditConfig();  return 1; }
   if(msg->gbsmsg->submess=='0') { ShowMSG(2,(int)LG_COPYRIGHT); return 0;}
   if(msg->gbsmsg->submess==KEY_CHANGE_PANEL)//'*'
   {
     if(FASTRUN_ENA_3)
     {
       if(PANEL_FS<3) PANEL_FS++;
       else PANEL_FS=1;
     }
     else
       if(FASTRUN_ENA_2)
       {
         if(PANEL_FS<2)PANEL_FS++;
         else PANEL_FS=1;
       }
     else
       if(FASTRUN_ENA) PANEL_FS=1;
       else PANEL_FS=0;
     
     REDRAW();
     DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
   }
   
   if(PANEL_FS==1)
   {
    if(DRAW_MODE)// Панель вертикальная
    {
      switch(msg->gbsmsg->submess)
      {
        case DOWN_BUTTON:// Джой вниз
        case '8':
            Selected_fr++;// Курсор вперед
            if(Selected_fr >= NumOfItems_fr)Selected_fr=0;// Если уткнулись в конец, возврат в начало
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
            break;       
        case UP_BUTTON:// Джой вверх
        case '2':
            Selected_fr--;// Курсор назад
            if(Selected_fr<0)Selected_fr=NumOfItems_fr-1; // Если пришли к началу, возвращаемся в конец
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
            break;          
        case ENTER_BUTTON:
        case '5':
            RunAction(Selected_fr,0);// Запускаем выделенную закладку
            return(1);
       }      
    }
    else// Горизонтально
    {
      switch(msg->gbsmsg->submess)
      {
        case RIGHT_BUTTON:
        case '6':
            Selected_fr++;
            if(Selected_fr >= NumOfItems_fr)Selected_fr=0;
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
            break;
        case LEFT_BUTTON:
        case '4':
            Selected_fr--;
            if(Selected_fr<0)Selected_fr=NumOfItems_fr-1;
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
            break;
        case ENTER_BUTTON:// Джой вверх
        case '5':
            RunAction(Selected_fr,0);// Запускаем выделенную закладку
            return(1);            
      }
    }
  }
       
//----------------------FastRun2---------------------//

 if(PANEL_FS==2)
  {
   if(DRAW_MODE_2)// Панель вертикальная
    {
      switch(msg->gbsmsg->submess)
      {
        case DOWN_BUTTON:// Джой вниз
        case '8':
           Selected_fr_2++;// Курсор вперед
           if(Selected_fr_2 >= NumOfItems_fr_2)Selected_fr_2=0;// Если уткнулись в конец, возврат в начало
           DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
        break;       
        case UP_BUTTON:// Джой вверх
        case '2':
            Selected_fr_2--;// Курсор назад
            if(Selected_fr_2<0)Selected_fr_2=NumOfItems_fr_2-1; // Если пришли к началу, возвращаемся в конец
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
        break;          
        case ENTER_BUTTON:// Джой вверх
        case '5':
          RunAction(Selected_fr_2,2);// Запускаем выделенную закладку
          return(1);
      }
    }
    else //Горизонтальная
    {
      switch(msg->gbsmsg->submess)
      {
        case RIGHT_BUTTON:
        case '6':
            Selected_fr_2++;
            if(Selected_fr_2 >= NumOfItems_fr_2)Selected_fr_2=0;
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
            break;       
        case LEFT_BUTTON:
        case '4':
            Selected_fr_2--;
            if(Selected_fr_2<0)Selected_fr_2=NumOfItems_fr_2-1;
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
            break;          
        case ENTER_BUTTON:// Джой вверх
        case '5':
            RunAction(Selected_fr_2,2);// Запускаем выделенную закладку
            return(1);  
      }
    }
  }
         
//----------------------FastRun3---------------------//

 if(PANEL_FS==3)
  {
   if(DRAW_MODE_3)// Панель вертикальная
    {
      switch(msg->gbsmsg->submess)
      {
        case DOWN_BUTTON:// Джой вниз
        case '8':
           Selected_fr_3++;// Курсор вперед
           if(Selected_fr_3 >= NumOfItems_fr_3)Selected_fr_3=0;// Если уткнулись в конец, возврат в начало
           DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
        break;       
        case UP_BUTTON:// Джой вверх
        case '2':
            Selected_fr_3--;// Курсор назад
            if(Selected_fr_3<0)Selected_fr_3=NumOfItems_fr_3-1; // Если пришли к началу, возвращаемся в конец
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
        break;          
        case ENTER_BUTTON:// Джой вверх
        case '5':
          RunAction(Selected_fr_3,3);// Запускаем выделенную закладку
          return(1);
      }
    }
    else //Горизонтальная
    {
      switch(msg->gbsmsg->submess)
      {
        case RIGHT_BUTTON:
        case '6':
            Selected_fr_3++;
            if(Selected_fr_3 >= NumOfItems_fr_3)Selected_fr_3=0;
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
            break;       
        case LEFT_BUTTON:
        case '4':
            Selected_fr_3--;
            if(Selected_fr_3<0)Selected_fr_3=NumOfItems_fr_3-1;
            DirectRedrawGUI_ID(FASTRUNMENU_GUI_ID);
        break;          
        case ENTER_BUTTON:// Джой вверх
        case '5':
          RunAction(Selected_fr_2,3);// Запускаем выделенную закладку
          return(1);         
       }
     }
   }
 }
  
 if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)// Если клавиша нажата или зажата
  {
    if (msg->gbsmsg->submess==FR_EXIT_BUTTON && PANEL_FS==1
        || msg->gbsmsg->submess==FR_EXIT_BUTTON_2 && PANEL_FS==2
          || msg->gbsmsg->submess==FR_EXIT_BUTTON_3 && PANEL_FS==3)
    {
      extern void ClearScreen(); 
      ClearScreen(); 
      return(1);
    }
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

extern void DrawScreenShot();
void RunFastRunPanel()
{
  if(PANEL_FS==1)
  {
   if (NumOfItems_fr > 0)
     {
        if(!IsNoJava()) DrawScreenShot();
        MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
        zeromem(main_gui,sizeof(MAIN_GUI));
        main_gui->gui.canvas=(void *)(&Canvas);
        main_gui->gui.methods=(void *)gui_methods_2;
        main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
        patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
        FASTRUNMENU_GUI_ID=CreateGUI(main_gui);
        canvasdata=BuildCanvas();
      }
  }
  if(PANEL_FS==2)
    {
     if (NumOfItems_fr_2 > 0)
      {
        if(!IsNoJava()) DrawScreenShot();
        MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
        zeromem(main_gui,sizeof(MAIN_GUI));
        main_gui->gui.canvas=(void *)(&Canvas);
        main_gui->gui.methods=(void *)gui_methods_2;
        main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
        patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
        FASTRUNMENU_GUI_ID=CreateGUI(main_gui);
      }
   }
   if(PANEL_FS==3)
    {
     if (NumOfItems_fr_3 > 0)
      {
        if(!IsNoJava()) DrawScreenShot();
        MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
        zeromem(main_gui,sizeof(MAIN_GUI));
        main_gui->gui.canvas=(void *)(&Canvas);
        main_gui->gui.methods=(void *)gui_methods_2;
        main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
        patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
        FASTRUNMENU_GUI_ID=CreateGUI(main_gui);
      }
   }
}
