#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\gui.h"
#include "inc\newmenu.h"
#include "inc\color.h"
#include "inc\ColorMap.h"
#include "inc\zslib.h"
#include "inc\ScreenMake.h"

typedef struct
{
  int name;
  KEY_PROC act;
}MENU_ITEMS;

int cur=0, sab=0, cur_sab=0, ssab=0, cur_ssab=0, showmenu=0;
int cur_save, cur_sab_save, sab_save;
int menu_txt, mnf, mnc;
IMGHDR *mn_fon, *mn_cur;

int filetype=0;
int Isdirect=0;
#define MMenuH   8
#define MMenuH1  10
#define MMenuH2  9
#ifdef NEWSGOLD
  #define CreateMH 3
#else
  #define CreateMH 2
#endif
#define PictMH   3
#define SoundMH  4
#define FileMH   4
#define OperMH   8
#define ViewMH   3
#define SortMH   5
#define MiskMH   4
#define BM_MH    2

#ifdef NEWSGOLD
void GetType()
{
  if(filetype && cur>2 && cursave)
    cur--;

  filetype=0;
  
  if(!IsInArchive())
  {
    if(scfile==NULL) return;
  
    if(IsMedia(scfile->sname))
    {
      if(cur>2 && cursave)cur++;
      filetype=2;
    }

    if(IsPict(scfile->sname))
    {
      if(cur>2 && cursave)cur++;
      filetype=1;
    }
  }
}
#endif

void freemenu()
{
  if (mnf==1)
  {
    mfree(mn_fon->bitmap);
    mfree(mn_fon);
  }
  if (mnc==1)
  {
    mfree(mn_cur->bitmap);
    mfree(mn_cur);
  }
}

void DoOpenSab1()
{
  sab=1;
  if (sab_save==sab && cursave)
    cur_sab=cur_sab_save;
  else
    cur_sab=0;
}

void DoOpenSab2()
{
  sab=2;
  if (sab_save==sab && cursave)
    cur_sab=cur_sab_save;
  else
    cur_sab=0;
}

void DoOpenSab3()
{
  sab=3;
  if (sab_save==sab && cursave)
    cur_sab=cur_sab_save;
  else
    cur_sab=0;
}

void DoOpenSab4()
{
  sab=4;
  if (sab_save==sab && cursave)
    cur_sab=cur_sab_save;
  else
    cur_sab=0;
}

void DoOpenSab5()
{
  sab=5;
  if (sab_save==sab && cursave)
    cur_sab=cur_sab_save;
  else
    cur_sab=0;
}
#ifdef NEWSGOLD
void DoOpenSab6()
{
  if(filetype==1)
  {
    sab=6;
    if (sab_save==sab && cursave)
      cur_sab=cur_sab_save;
    else
      cur_sab=0;
  }
  
  if(filetype==2)
  {
    sab=7;
    if (sab_save==sab && cursave)
      cur_sab=cur_sab_save;
    else
      cur_sab=0;
  }
}
#endif
void DoOpenSSab()
{
  ssab=1;
  cur_ssab=0;
}

void Menu_DoAltOpen()
{
  menuclose();
  DoAltOpen();
}

void Menu_DoSent()
{
  menuclose();
  DoSent();
}

void Menu_DoSeach()
{
  DoSeach();
}

void Menu_DoFilter()
{
  menuclose();
  DoFilter();
}

void Menu_DoRefresh()
{
  menuclose();
  DoRefresh();
}

void Menu_DoFilePropNew()
{
  menuclose();
  DoFilePropNew();
}

void Menu_DoDrvInf()
{
  menuclose();
  DoDrvInf();
}

void Menu_DoOptions()
{
  menuclose();
  DoOptions();
}

void Menu_DoAboutDlg()
{
  menuclose();
  DoAboutDlg();
}

void Menu_DoExit()
{
  menuclose();
  CB_Exit(IDYES);
}

#ifdef NEWSGOLD
void Menu_SetWall()
{
  CurFullPathByName(scfile->sname);
  SetWallpaper(pathbuf);
  menuclose();
}

void Menu_SetLogo()
{
  CurFullPathByName(scfile->sname);
  SetLogo(pathbuf);
  menuclose();
}

void Menu_SetScreenSaver()
{
  CurFullPathByName(scfile->sname);
  SetScreenSaver(pathbuf);
  menuclose();
}

void Menu_SetCall()
{
  CurFullPathByName(scfile->sname);
  SetCall(pathbuf);
  menuclose();
}

void Menu_SetMessages()
{
  CurFullPathByName(scfile->sname);
  SetMessages(pathbuf);
  menuclose();
}

void Menu_SetAlarmClock()
{
  CurFullPathByName(scfile->sname);
  SetAlarmClock(pathbuf);
  menuclose();
}

void Menu_SetAppointments()
{
  CurFullPathByName(scfile->sname);
  SetAppointments(pathbuf);
  menuclose();
}

MENU_ITEMS MMenu1[]=
{
  {ind_altopen, Menu_DoAltOpen},
  {ind_bt, Menu_DoSent},
  {ind_set_ass, DoOpenSab6},
  {ind_create, DoOpenSab1},
  {ind_file_m, DoOpenSab2},
  {ind_oper_m, DoOpenSab3},
  {ind_prop, Menu_DoFilePropNew},
  {ind_view_m, DoOpenSab4},
  {ind_misc_m, DoOpenSab5},
  {ind_exit, Menu_DoExit}
};

MENU_ITEMS MSetPict[]=
{
  {ind_wall, Menu_SetWall},
  {ind_logo, Menu_SetLogo},
  {ind_ss, Menu_SetScreenSaver}
};

MENU_ITEMS MSetSound[]=
{
  {ind_call, Menu_SetCall},
  {ind_mess, Menu_SetMessages},
  {ind_alarm, Menu_SetAlarmClock},
  {ind_org, Menu_SetAppointments}
};

#endif


MENU_ITEMS MMenu[]=
{
  {ind_altopen, Menu_DoAltOpen},
  {ind_create, DoOpenSab1},
  {ind_file_m, DoOpenSab2},
  {ind_oper_m, DoOpenSab3},
  {ind_prop, Menu_DoFilePropNew},
  {ind_view_m, DoOpenSab4},
  {ind_misc_m, DoOpenSab5},
  {ind_exit, Menu_DoExit}
};

MENU_ITEMS MMenu2[]=
{
  {ind_altopen, Menu_DoAltOpen},
  {ind_bt, Menu_DoSent},
  {ind_create, DoOpenSab1},
  {ind_file_m, DoOpenSab2},
  {ind_oper_m, DoOpenSab3},
  {ind_prop, Menu_DoFilePropNew},
  {ind_view_m, DoOpenSab4},
  {ind_misc_m, DoOpenSab5},
  {ind_exit, Menu_DoExit}
};
#ifdef NEWSGOLD
MENU_ITEMS CreateM[]=
{
  {ind_newfile, DoNewFile},
  {ind_newdir, DoNewDir},
  {ind_arch, DoArchive}
};
#else
MENU_ITEMS CreateM[]=
{
  {ind_newfile, DoNewFile},
  {ind_newdir, DoNewDir}
};
#endif
MENU_ITEMS FileM[]=
{
  {ind_chk, DoChk},
  {ind_chkall, DoChkAll},
  {ind_unchall, DoUnCAll},
  {ind_invchk, DoInvChk}
};

MENU_ITEMS OperM[]=
{
  {ind_past, DoPaste},
  {ind_cancel, DoCancel},
  {ind_copy, DoCopy},
  {ind_move, DoMove},
  {ind_del, DoDel},
  {ind_rename, DoRen},
  {ind_natexpl, DoNatExpl},
  {ind_find, Menu_DoSeach}
};

MENU_ITEMS ViewM[]=
{
  {ind_sort_m, DoOpenSSab},
  {ind_filter, Menu_DoFilter},
  {ind_refresh, Menu_DoRefresh}
};

MENU_ITEMS SortM[]=
{
  {ind_sortn, DoSortN},
  {ind_sorte, DoSortE},
  {ind_sorts, DoSortS},
  {ind_sortd, DoSortD},
  {ind_sortr, DoSortR}
};

MENU_ITEMS MiskM[]=
{
  {ind_bm_m, DoOpenSSab},
  {ind_drvinf, Menu_DoDrvInf},
  {ind_settings, Menu_DoOptions},
  {ind_about_m, Menu_DoAboutDlg}
};

MENU_ITEMS BM_M[]=
{
  {ind_bml, DoBMList},
  {ind_add, DoBMAdd}
};

int y_pos[5];

void DoMenu()
{
 allclose(); 
 ClearScreen();
 DoScreen();
 if(cursave)
 {
   cur_sab_save=cur_sab;
   sab_save=sab;
 }else
    cur=0; 
 
 showmenu=1;
 sab=0;
 cur_sab=0;
 ssab=0;
 cur_ssab=0;
 Busy1=1; 
 
 if (cursave)
 {
  if(Isdirect && cur>1)
    cur++;

  if(scfile->attr & FA_DIRECTORY || IsInArchive())
  {
    Isdirect=1;
    if(cur>0)cur--;
  }
  else
    Isdirect=0;
 }
 
#ifdef NEWSGOLD
 GetType();
#endif
 
 y_pos[0]=scr_h-MMenuH1*(menu_txt+1);
 y_pos[1]=y_pos[0]+(menu_txt+1)*3-(menu_txt+1)/2;
 y_pos[2]=y_pos[0]+(menu_txt+1)*7-(menu_txt+1)/2;
 y_pos[3]=y_pos[2]-(menu_txt+1)/2;
 y_pos[4]=y_pos[2]-(menu_txt+1)-(menu_txt+1)/2;   
}

void menuclose()
{
  ClearScreen();
  Busy1=0;
  showmenu=0;
  PeReDraw();
}

void DoMenuENTER()
{
  switch (sab)
  {
  case 0:
    if (!Isdirect)
    {
#ifdef NEWSGOLD
    if(filetype)
    {
      if (MMenu1[cur].act) MMenu1[cur].act();
    }else
      if (MMenu2[cur].act) MMenu2[cur].act();
#else
    if (MMenu2[cur].act) MMenu2[cur].act();
#endif
    }else
      if (MMenu[cur].act) MMenu[cur].act();
    break;
    
  case 1:
    menuclose();
    CreateM[cur_sab].act();
    break; 
    
  case 2:
    menuclose();
    FileM[cur_sab].act();
    break;
    
  case 3:
    if(cur_sab!=OperMH-1)menuclose();
    OperM[cur_sab].act();
    break;
    
  case 4:
    if (ssab==1)
    {
      menuclose();
      SortM[cur_ssab].act();
    }else
    {
      ViewM[cur_sab].act();
    }
    break;
    
  case 5:
    if (ssab==1)
    {
      menuclose();
      BM_M[cur_ssab].act();
    }else
      MiskM[cur_sab].act();
    break;
#ifdef NEWSGOLD    
  case 6:
    if (MSetPict[cur_sab].act) MSetPict[cur_sab].act();
    break;
    
  case 7:
    if (MSetSound[cur_sab].act) MSetSound[cur_sab].act();
    break;
#endif
  }
}

void DoNumKey(int key)
{
  int open=0;
  switch(sab)
  {
  case 0:
    if (key>='1' && ((key<='9' && !Isdirect) || (key<'9' && Isdirect)))
    {
      cur=key-1-'0';
      open=1;
    }else
      if (key=='0' && filetype)
      {
        cur=9;
        open=1;
      }else
        open=0;  
    break;
    
  case 1:
    if (key>='1' && key<='3')
    {
      cur_sab=key-1-'0';
      open=1;
    }else
      open=0;  
    break;

  case 2:
    if (key>='1' && key<='4')
    {
      cur_sab=key-1-'0';
      open=1;
    }else
      open=0;  
    break;

  case 3:
    if (key>='1' && key<='7')
    {
      cur_sab=key-1-'0';
      open=1;
    }else
      open=0;  
    break;
 
  case 4:
    if (ssab)
    {
      if (key>='1' && key<='5')
      {
        cur_ssab=key-1-'0';
        open=1;
      }else
        open=0; 
    }else
    {    
      if (key>='1' && key<='3')
      {
        cur_sab=key-1-'0';
        open=1;
      }else
        open=0; 
    }
    break;
    
  case 5:
    if (ssab)
    {
      if (key>='1' && key<='2')
      {
        cur_ssab=key-1-'0';
        open=1;
      }else
        open=0; 
    }else
    {    
      if (key>='1' && key<='4')
      {
        cur_sab=key-1-'0';
        open=1;
      }else
        open=0; 
    }
    break;
    
  case 6:
    if (key>='1' && key<='3')
    {
      cur_sab=key-1-'0';
      open=1;
    }else
      open=0;  
    break;

  case 7:
    if (key>='1' && key<='4')
    {
      cur_sab=key-1-'0';
      open=1;
    }else
      open=0;  
    break;

  }
  
  if (open)
    DoMenuENTER();
}

void DoMenuUP()
{
  switch(sab)
  {
    case 0:
      if (!Isdirect)
      {
        if(filetype)
          cur=(cur?cur--:MMenuH1-1);
        else
          cur=(cur?cur--:MMenuH2-1);
      }else
        cur=(cur?cur--:MMenuH-1);
    break;
    case 1:
      cur_sab=(cur_sab?cur_sab--:CreateMH-1);
    break;
    case 2:
      cur_sab=(cur_sab?cur_sab--:FileMH-1);
    break;
    case 3:
      cur_sab=(cur_sab?cur_sab--:OperMH-1);
    break;
    case 4:
      if (ssab)
        cur_ssab=(cur_ssab?cur_ssab--:SortMH-1);
      else
        cur_sab=(cur_sab?cur_sab--:ViewMH-1);
    break;
    case 5:
      if (ssab)
        cur_ssab=(cur_ssab?cur_ssab--:BM_MH-1);
      else
        cur_sab=(cur_sab?cur_sab--:MiskMH-1);
    break;
    
    case 6:
      cur_sab=(cur_sab?cur_sab--:PictMH-1);
    break;
    case 7:
      cur_sab=(cur_sab?cur_sab--:SoundMH-1);
    break;
  }
}
 
void DoMenuDOWN()
{
  switch(sab)
  {
    case 0:
      if (!Isdirect)
      {
        if(filetype)
          cur=(cur!=MMenuH1-1?cur++:0); 
        else
          cur=(cur!=MMenuH2-1?cur++:0); 
      }else
        cur=(cur!=MMenuH-1?cur++:0); 
      break;
      
    case 1:
      cur_sab=(cur_sab!=CreateMH-1?cur_sab++:0); 
      break;
      
    case 2:
      cur_sab=(cur_sab!=FileMH-1?cur_sab++:0); 
      break;
      
    case 3:
      cur_sab=(cur_sab!=OperMH-1?cur_sab++:0); 
    break;
    
    case 4:
      if (ssab)
        cur_ssab=(cur_ssab!=SortMH-1?cur_ssab++:0); 
      else
        cur_sab=(cur_sab!=ViewMH-1?cur_sab++:0); 
      break;
      
    case 5:
      if (ssab)
        cur_ssab=(cur_ssab!=BM_MH-1?cur_ssab++:0);
      else
        cur_sab=(cur_sab!=MiskMH-1?cur_sab++:0); 
      break;
      
    case 6:
      cur_sab=(cur_sab!=PictMH-1?cur_sab++:0); 
    break;
    
    case 7:
      cur_sab=(cur_sab!=SoundMH-1?cur_sab++:0); 
    break;
  }
}

void menu_key(int key)
{
  switch (key)
  {
    case RIGHT_BUTTON:
    case ENTER_BUTTON: 
      DoMenuENTER();
      PeReDraw();
      return;
      
    case UP_BUTTON: 
      DoMenuUP();
      break;
      
    case DOWN_BUTTON:
      DoMenuDOWN();
      break;
      
    case LEFT_BUTTON:
    case LEFT_SOFT:
      if (sab==0)
      {
        menuclose();
        return;
      }
      else 
      {
        if (ssab==0)
        {
          sab=0;
          cur_sab=0;
         }else
         {
          ssab=0;
          cur_ssab=0;
         }
      }
     break;
   case RIGHT_SOFT:
     menuclose(); 
     return;

   }
  if((key>='1' && key<='9') || (key=='0') || (key=='*') || (key=='#'))
    DoNumKey(key);
}

int MenuH[12];

int MenuWid(int count, MENU_ITEMS *ItemsName)
{
  WSHDR *ws = AllocWS(64);
  int wi=0;
  for(int i=0; i<count; i++)  
  {
    wsprintf(ws, _t, muitxt(ItemsName[i].name));
    if (Get_WS_width(ws,FONT_MENUV)>wi)
      wi=Get_WS_width(ws,FONT_MENUV);
  }
  FreeWS(ws);
  
  return wi;
}

void crtmenu()
{
  FSTATS fs;
  char z[128];
  
  sprintf(z, "%smenu_bg.png",graf_folder);
  if (GetFileStats(z,&fs,&err)==0)
  {
    mn_fon=CreateIMGHDRFromPngFile(z, 0); 
    mnf=1;
  }
  
  sprintf(z, "%smenu_cursor.png",graf_folder);
  if (GetFileStats(z,&fs,&err)==0)
  {
    mn_cur=CreateIMGHDRFromPngFile(z, 0); 
    mnc=1;
  }
  menu_txt =  GetFontYSIZE(FONT_MENUNV);
  
////////////////////////////////////////////////////////////////////////////////
  if (mnc==0 || mnf==0)
  {
#ifdef NEWSGOLD
    MenuH[0]=MenuWid(MMenuH1, MMenu1)+8;
#endif
    MenuH[10]=MenuWid(MMenuH2, MMenu2)+8;
    
    MenuH[11]=MenuWid(MMenuH, MMenu)+8;
    
    MenuH[1]=MenuWid(CreateMH, CreateM)+8;
    MenuH[2]=MenuWid(FileMH, FileM)+8;
    MenuH[3]=MenuWid(OperMH, OperM)+8;
    MenuH[4]=MenuWid(ViewMH, ViewM)+8;
    MenuH[5]=MenuWid(SortMH, SortM)+menu_txt+8;
    MenuH[6]=MenuWid(MiskMH, MiskM)+8;
    MenuH[7]=MenuWid(BM_MH, BM_M)+8;
#ifdef NEWSGOLD
    MenuH[8]=MenuWid(PictMH, MSetPict)+10;
    MenuH[9]=MenuWid(SoundMH, MSetSound)+8;
#endif
  }
////////////////////////////////////////////////////////////////////////////////  
}

#ifdef ELKA
  #define sdvig 35
#else
  #define sdvig 23
#endif

static DrawCheckBox(int x,int y,int flag)
{
  RECT rc;
  StoreXYXYtoRECT(&rc,x,y,x+menu_txt-3,y+menu_txt-3);
  
  if (flag!=4)
    DrawRoundedFrame(rc.x,rc.y,rc.x2,rc.y2,6,6,0,(char*)&Colors[clCheckBD],NULL);
  else
    DrawRectangle   (rc.x,rc.y,rc.x2,rc.y2,0,(char*)&Colors[clCheckBD],NULL);
  
  if ((flag==4 && _CurTab->sort>=ST_REVERS) || (_CurTab->sort>=ST_REVERS && _CurTab->sort==flag+ST_REVERS) || (_CurTab->sort==flag))
    DrawRoundedFrame(rc.x+2,rc.y+2,rc.x2-2,rc.y2-2,6,6,0,(char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
  if (flag==4 && _CurTab->sort>=ST_REVERS)
    DrawRectangle(rc.x+2,rc.y+2,rc.x2-2,rc.y2-2,0,(char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
}      

void DrawMenu(int count, MENU_ITEMS *ItemsName, int ots, int idx, int cur_pos, int drwcb, int wi)
{  
  int i=0, ot=drwcb?ots+menu_txt:ots; 
  int font;
  int tc;
  if (mnf==1)
    DrwImage(mn_fon, ots, idx-2, 0, 0, mn_fon->w, count*(menu_txt+1)+4,NULL);
  else
  {
    DrwImage(background, ots, idx-1, 
             ots, idx-1-YDISP1, wi, count*(menu_txt+1)+2, 0);    
    DrawRoundedFrame(ots, idx-1, ots+wi, idx-2+count*(menu_txt+1)+3,
                     0,0,0,(char*)&Colors[clMenuBD],(char*)&Colors[clMenuBG]);
  }
  
  if (mnc==1)
    DrwImage(mn_cur, ots+2, idx+cur_pos*(menu_txt+1), 0, 0, mn_cur->w, mn_cur->h, NULL);
  else
    DrawRoundedFrame(ots+1, idx+cur_pos*(menu_txt+1), ots+wi-1, idx+cur_pos*(menu_txt+1)+(menu_txt+1),
                     0,0,0,(char*)&Colors[clMenuCurBD],(char*)&Colors[clMenuCurBG]);  
  
  
  do{
    font=(cur_pos==i? FONT_MENUV: FONT_MENUNV);
    tc=(cur_pos==i? clMenuTXTSelect: clMenuTXTNormal);
    
    wsprintf(guibuf, _t, muitxt(ItemsName[i].name));
    DrawString(guibuf, ot+4, idx, scr_w, idx+menu_txt, font, TEXT_ALIGNLEFT,
               (char*)&Colors[tc],0);
    
    if (drwcb)  DrawCheckBox(ots+4,idx+2,i);  

    i++;
    idx+=(menu_txt+1);
  }while(i<count);
  
}

void MenuVide()
{
  DrawScreen();
  
  if (!Isdirect)
  {
#ifdef NEWSGOLD
    if(filetype)
      DrawMenu(MMenuH1, MMenu1, 1, y_pos[0], cur, 0, MenuH[0]);
    else
#endif
      DrawMenu(MMenuH2, MMenu2, 1, y_pos[0]+(menu_txt+1), cur, 0, MenuH[10]);
  }else
    DrawMenu(MMenuH, MMenu, 1, y_pos[0]+(menu_txt+1)*2, cur, 0, MenuH[11]);
  
  switch (sab)
  {
  case 1:
    DrawMenu(CreateMH, CreateM, sdvig, y_pos[1], cur_sab, 0,MenuH[1]);
    break;
    
  case 2:
    DrawMenu(FileMH, FileM, sdvig, y_pos[1], cur_sab, 0,MenuH[2]);
    break;
    
  case 3:
    DrawMenu(OperMH, OperM, sdvig, y_pos[1], cur_sab, 0,MenuH[3]);
    break;
    
  case 4:
    DrawMenu(ViewMH, ViewM, sdvig, y_pos[2], cur_sab, 0,MenuH[4]);
    if (ssab)
      DrawMenu(SortMH, SortM, sdvig*2-5, y_pos[4], cur_ssab, 1,MenuH[5]);
    break; 
    
  case 5:
    DrawMenu(MiskMH, MiskM, sdvig, y_pos[2], cur_sab, 0,MenuH[6]);  
    if (ssab)
      DrawMenu(BM_MH, BM_M, sdvig*2, y_pos[3], cur_ssab, 0,MenuH[7]);
    break;
#ifdef NEWSGOLD    
  case 6:
    DrawMenu(PictMH, MSetPict, sdvig, y_pos[1]-menu_txt, cur_sab, 0,MenuH[8]);
    break;
    
  case 7:
    DrawMenu(SoundMH, MSetSound, sdvig, y_pos[1]-menu_txt, cur_sab, 0,MenuH[9]);
    break;
#endif
  }
}
 
