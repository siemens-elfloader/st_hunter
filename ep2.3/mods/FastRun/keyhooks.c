// Клавиатурные хуки ;)

int IsIdle(void)
{ 
  void *icsm=FindCSMbyID(CSM_root()->idle_id); 
  if (IsGuiOnTop(((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4 ])) return(1); 
  else return(0); 
}

void RunIdlePanel()
{
  if ( NumOfItems_idle > 0)
  {
    /*
    MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(void *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods_id;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
    patch_rect((RECT*)&Canvas,x1,y1,x1+LEN_ID,y1+idle_pic_size+2);
    FASTRUNMENU_GUI_IDLE_ID=CreateGUI(main_gui);*/
    IdleMenuActive=1;
    DrawIdleMenu();
  }
  else EditConfig();
  DirectRedrawGUI_ID(FASTRUNMENU_GUI_IDLE_ID);
  REDRAW();
}

//--------------------------------------------------------------------//
//Эта для работы на именно для идле,без левых кейхуков))правда пикает при заквытии  эльфа...
//и конфликт с еменю остается,так как кейхук то не добавляется)

int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);

int (*old_ed_onkey)(GUI *gui, GUI_MSG *msg);
void (*old_ed_ghook)(GUI *gui, int cmd);
void (*old_ed_redraw)(void *data);

void (*old_icsm_onClose)(CSM_RAM*);

int my_ed_onkey(GUI *gui, GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  int m=msg->gbsmsg->msg;
  int r;
  if (IsUnlocked() && (m==KEY_DOWN) && (key==IDLE_CALL_BUTTON) && (!IdleMenuActive) && (!ENA_IDLE_LONGPRESS))
  {
    RunIdlePanel();
    r=0;
  }
  else
    if (IsUnlocked()&&(m==LONG_PRESS)&&(key==IDLE_CALL_BUTTON)&&(!IdleMenuActive)&&(ENA_IDLE_LONGPRESS))
      {
        RunIdlePanel();
        r=0;
      }
 /* else if((m==KEY_DOWN || m==LONG_PRESS) && (IdleMenuActive) && (IsUnlocked()))
  {
    if(key==IDLE_EXIT_BUTTON)
    {
       IdleMenuActive=0;
       DrawIdleMenu(); // Снова перерисовываем
    }
   switch(key)
    {
    case LEFT_BUTTON:case UP_BUTTON:// Джой влево
    case '4':
         Selected_idle--;// Курсор назад
         if(Selected_idle<0)
         Selected_idle=NumOfItems_idle-1; // Если пришли к началу, возвращаемся в конец
         DrawIdleMenu(); // перерисовываем
         break;       
    case RIGHT_BUTTON:case DOWN_BUTTON:// Джой вправо
    case '6':
         Selected_idle++;// Курсор вперед
         if(Selected_idle >= NumOfItems_idle)
         Selected_idle=0;// Если уткнулись в конец, возврат в начало
         DrawIdleMenu(); // Снова перерисовываем
         break;       
    case ENTER_BUTTON:// Тыкаем на джой
    case '5':
         IdleMenuActive=0;
         RunAction(Selected_idle,1);// И запускаем выделенную закладку
         DrawIdleMenu(); // Снова перерисовываем
         REDRAW();
         break;
    case '#':
         IdleMenuActive=0;
         EditConfig();
         DrawIdleMenu(); // Снова перерисовываем
         REDRAW();
         break;
    case '0':
         ShowMSG(2,(int)LG_COPYRIGHT);
         break;
   }  
  }*/
  else    
    r=old_ed_onkey(gui,msg);
  return(r);
}

CSM_DESC icsmd;
extern void maincsm_onclose(CSM_RAM *csm);
extern int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg);
void DoSplices(GUI *gui)
{
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
/*  memcpy(&icsmd,icsm->constr,sizeof(icsmd));
  old_icsm_onClose=icsmd.onClose;
  old_icsm_onMessage=icsmd.onMessage;
  icsmd.onClose=maincsm_onclose;
  icsmd.onMessage=maincsm_onmessage;
  icsm->constr=&icsmd;*/
  
  if(icsm)
  {
  static INPUTDIA_DESC my_ed;
  memcpy(&my_ed,gui->definition,sizeof(INPUTDIA_DESC));
  if (my_ed.onKey != my_ed_onkey)
  {
    old_ed_onkey=my_ed.onKey;
    my_ed.onKey=my_ed_onkey;
    gui->definition=&my_ed;
  }
  }
}

//--------------------------------------------------------------------//

#pragma swi_number=0x0C5
__swi __arm int IsIdleUiOnTop();//На сг нет этой функции

#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])
int ENA_LOCK=0;
int IDLE=0;
int IDLE_KeyHook(int submsg, int msg)// Это хук, действующий на IDLE.
{
 CSM_RAM *data=FindCSMbyID(CSM_root()->idle_id);
// void *data=FindGUIbyId(CSM_root()->idle_id,NULL);
  if((IsUnlocked())&&(!FastRunMenuActive))
  {
    if (submsg==IDLE_CALL_BUTTON && !IdleMenuActive)
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ENA_IDLE_LONGPRESS)
          {
            if(IsIdle())
              {
                 if (IsUnlocked()||ENA_LOCK)
                  {
                    RunIdlePanel();
                  }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,IDLE_CALL_BUTTON);break;
              }
          }            
          else
            if(IsUnlocked()||ENA_LOCK)
             {
               if(IsIdleUiOnTop())
                   RunIdlePanel();
             }
          //  return(2);          
          
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,IDLE_CALL_BUTTON);
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case KEY_UP:
          if (ENA_IDLE_LONGPRESS)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,IDLE_CALL_BUTTON);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case LONG_PRESS:
          if (ENA_IDLE_LONGPRESS)
          {
            mode_enter=1;
            if(IDLE)
             {
              if(IsIdle())
              {
                if (IsUnlocked()||ENA_LOCK)
                {
                  RunIdlePanel();
                }
              }
              else  GBS_SendMessage(MMI_CEPID,msg,IDLE_CALL_BUTTON);
            }
            else
             if (IsUnlocked()||ENA_LOCK)
             {
              RunIdlePanel();
             }
          }
         break;
      }
    }
   /*if(submsg==IDLE_CALL_BUTTON && !IdleMenuActive)
   {
     if(IsGuiOnTop(idlegui_id))
     {
     GUI *igui=GetTopGUI();
     if(igui)
     {
//       DoSplices(igui);
     switch(msg)
     {
      case KEY_DOWN:
        if(IsIdle())
        {
         if (!ENA_IDLE_LONGPRESS) RunIdlePanel();
         else
         {   
          if (mode_enter==2)
          {
           GBS_SendMessage(MMI_CEPID,KEY_UP,IDLE_CALL_BUTTON);
           return (0);
          }
          mode_enter=0;
          }
         }
         else
         {
           GBS_SendMessage(MMI_CEPID,KEY_UP,IDLE_CALL_BUTTON);break;
         }
         return (2);                    
     case KEY_UP:
         if(IsIdle())
         {
          if (ENA_IDLE_LONGPRESS)
          {
           if (mode_enter==0)
           {
            mode_enter=2;
            GBS_SendMessage(MMI_CEPID,KEY_DOWN,IDLE_CALL_BUTTON);
            return (2);
           }
           if (mode_enter==2)
           {
            mode_enter=0;
            return (0);
           }
           mode_enter=0;
           }
          }
          else
          {
           GBS_SendMessage(MMI_CEPID,KEY_UP,IDLE_CALL_BUTTON);break;
          }
          return (2);
      case LONG_PRESS:
          if(IsIdle())
          {
           if (ENA_IDLE_LONGPRESS)
            {
             mode_enter=1;
             RunIdlePanel();
            }
           }
           else
            {
             GBS_SendMessage(MMI_CEPID,KEY_UP,IDLE_CALL_BUTTON);break;
            }
            break;
      }*///switch
    //if(submsg==IDLE_CALL_BUTTON && !IdleMenuActive)
               
          if((!IDLE_CALL_BUTTON)&&(!IDLE_EXIT_BUTTON))
          {
            if ((msg==KEY_DOWN)||(msg==LONG_PRESS))// Если клавиша нажата или зажата
            {
              switch(submsg)
              {
              case LEFT_BUTTON:case UP_BUTTON:// Джой влево
                  Selected_idle--;// Курсор назад
                  if(Selected_idle<0)
                    Selected_idle=NumOfItems_idle-1; // Если пришли к началу, возвращаемся в конец
                  DrawIdleMenu(); // И перерисовываем саму панель
                  return (2); 
       
              case RIGHT_BUTTON:case DOWN_BUTTON:// Джой вправо
                  Selected_idle++;// Курсор вперед
                  if(Selected_idle >= NumOfItems_idle)
                    Selected_idle=0;// Если уткнулись в конец, возврат в начало
                  DrawIdleMenu(); // Снова перерисовываем
                  return (2);
       
                case ENTER_BUTTON:// Тыкаем на джой
                  IdleMenuActive=0;
                  RunAction(Selected_idle,1);// И запускаем выделенную закладку
                  return (2);
              }
            }
          }
          else
          {
            if(IdleMenuActive)
            {
              if ((msg==KEY_DOWN)||(msg==LONG_PRESS))// Если клавиша нажата или зажата
              {
                switch(submsg)
                {
                case LEFT_BUTTON:case UP_BUTTON:// Джой влево
                  case '4':
                    Selected_idle--;// Курсор назад
                    if(Selected_idle<0)
                      Selected_idle=NumOfItems_idle-1; // Если пришли к началу, возвращаемся в конец
                    DrawIdleMenu(); // И перерисовываем саму панель
                    return (2); // Возвращаем 2, чтобы кнопка больще ничего не запускала...
       
                case RIGHT_BUTTON:case DOWN_BUTTON:// Джой вправо
                  case '6':
                    Selected_idle++;// Курсор вперед
                    if(Selected_idle >= NumOfItems_idle)
                      Selected_idle=0;// Если уткнулись в конец, возврат в начало
                    DrawIdleMenu(); // Снова перерисовываем
                    return (2);
       
                  case ENTER_BUTTON:// Тыкаем на джой
                  case '5':
                    IdleMenuActive=0;
                    RunAction(Selected_idle,1);// И запускаем выделенную закладку
                    return (2);
        
                  case '#':
                    IdleMenuActive=0;
                    EditConfig();
                    return (2);
        
                  case '0':
                    ShowMSG(2,(int)LG_COPYRIGHT);
                    return (2);
                }
              }
              if (submsg==IDLE_EXIT_BUTTON)// Кнопка закрытия панели. Тут все уже ясно
              {
                switch (msg)
                {
                  case KEY_DOWN:
                    IdleMenuActive=0;
                    REDRAW();
                    DrawIdleMenu();
                    extern void ClearScreen(); 
                    ClearScreen(); 
                    return (2);
                    
                  case LONG_PRESS:
                    return (2);
                }
              }
              else return (2);
         // }
        }
      }
    }
//  }
//  }
 return (0);
}

int FastRun_KeyHook(int submsg, int msg)// Вызов панели FastRun
{
if((IsUnlocked())&&(!FastRunMenuActive))
{
    if (submsg==FR_CALL_BUTTON)//first panel
    {
      PANEL_FS=1;
      switch(msg)
      {
        case KEY_DOWN:
          if (!ENA_FR_LONGPRESS)
          {
            if(IDLE_1)
            {
              if(IsIdle())
              {
                RunFastRunPanel();//RunFastRunPanel();
              }            
              else GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON);break;
            }
            else RunFastRunPanel();//RunFastRunPanel();
                
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON);
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case KEY_UP:
          if (ENA_FR_LONGPRESS)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,FR_CALL_BUTTON);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case LONG_PRESS:
          if (ENA_FR_LONGPRESS)
          {
            mode_enter=1;
           if(IDLE_1)
            {
              if(IsIdle())
              {
                RunFastRunPanel();//RunFastRunPanel();
              }            
              else GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON);
            }
            else RunFastRunPanel();//RunFastRunPanel();
          }
          break;
      }
    }//if
   }
return(0); 
}

int FastRun_KeyHook_2(int submsg, int msg)// Вызов панели FastRun
{
if((IsUnlocked())&&(!FastRunMenuActive))
{
    if (submsg==FR_CALL_BUTTON_2)//second panel
    {
      PANEL_FS=2;
      switch(msg)
      {
        case KEY_DOWN:
          if (!ENA_FR_LONGPRESS_2)
          {
            if(IDLE_2)
            {
              if(IsIdle())
              {
                RunFastRunPanel();//RunFastRunPanel();
              }            
              else GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON_2);break;
            }
            else RunFastRunPanel();//RunFastRunPanel();
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON_2);
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case KEY_UP:
          if (ENA_FR_LONGPRESS_2)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,FR_CALL_BUTTON_2);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case LONG_PRESS:
          if (ENA_FR_LONGPRESS_2)
          {
            mode_enter=1;
            if(IDLE_2)
            {
              if(IsIdle())
              {
                RunFastRunPanel();//RunFastRunPanel();
              }            
              else GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON_2);
            }
            else RunFastRunPanel();//RunFastRunPanel();
          }
          break;
      }
    }//if
  }
return(0); 
}

int FastRun_KeyHook_3(int submsg, int msg)// Вызов панели FastRun
{
if((IsUnlocked())&&(!FastRunMenuActive))
{
    if (submsg==FR_CALL_BUTTON_3)//second panel
    {
      PANEL_FS=3;
      switch(msg)
      {
        case KEY_DOWN:
          if (!ENA_FR_LONGPRESS_3)
          {
            if(IDLE_3)
            {
              if(IsIdle())
              {
                RunFastRunPanel();//RunFastRunPanel();
              }            
              else GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON_3);break;
            }
            else RunFastRunPanel();//RunFastRunPanel();
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON_3);
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case KEY_UP:
          if (ENA_FR_LONGPRESS_3)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,FR_CALL_BUTTON_3);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case LONG_PRESS:
          if (ENA_FR_LONGPRESS_3)
          {
            mode_enter=1;
            if(IDLE_3)
            {
              if(IsIdle())
              {
                RunFastRunPanel();//RunFastRunPanel();
              }            
              else GBS_SendMessage(MMI_CEPID,KEY_UP,FR_CALL_BUTTON_3);
            }
            else RunFastRunPanel();//RunFastRunPanel();
          }
          break;
      }
    }//if
  }
return(0); 
}

