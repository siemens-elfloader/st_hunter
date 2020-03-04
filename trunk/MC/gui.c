#include "inc/color.h"
#include "inc/mc.h"
#include "inc/gui.h"
#include "inc/lng.h"
#include "inc/mui.h"
#include "inc/ColorMap.h"
#include "inc/zslib.h"
#include "inc/newmenu.h"
#include "inc/popup.h"
#include "inc/ScreenMake.h"
#include <swilib.h>

char stzip[128],st_bm[128], st_seach[128];

IMGHDR *cursor, *background, *iinfo, *iprop;

int ICO_X, ICO_Y, TXT_X1, TXT_X2, CUR_X, ITM_X2;
int inarh=0;

CHKDS_PARAM SelParam;

GBSTMR sctm;
FILEINF* scfile=0;
int scind;
int sctpos;
int sctdir;
int scwait;

int scr_w;
int scr_h,scr_h1;
int txt_h,
    soft_h,
    head_h,
    popup_h,
    prop_h;

int itm_ch;
int itms_max;
int itms_bs;

int NormalFont;
int BoldFont;
int IMGHDRW;
int font;

int items_vis, cur_h, cur_w, fol_h;
int y_at;

void InitScr()
{
 scr_h1 = ScreenH();
 fol_h = GetImgHeight(ext_dir.ico);
 menugrafic();
 scr_w = ScreenW();
 scr_h = scr_h1-soft_h;

 if(cursor)
 {
   cur_h = cursor->h;
   cur_w = cursor->w;
   ITM_X2 = (scr_w-cur_w)/2-4+cur_w;
 }
 else
 {
   cur_h = (fol_h>txt_h)?fol_h+2:txt_h+2;
   ITM_X2 = (scr_w-MAIN_B-SB_HS-1);
   cur_w = ITM_X2 - ITM_X1;
 }

 items_vis = (cur_h>=2*txt_h)?1:0;

 if (!txt_h)
 {
  DoShowMsg("Не удалось определить высоту шрифта.\nGetFontYSIZE()=0");
  txt_h = 16;
 }

 CUR_X=(scr_w-cur_w)/2-1;

 ICO_X = CUR_X + (cur_h - GetImgWidth(ext_dir.ico)) / 2 + 1;
 ICO_Y = (cur_h - fol_h) / 2;

 TXT_X1=ICO_X + fol_h + 1;
 TXT_X2=CUR_X + cur_w - 2;

 itms_max = ((FLS_H - ITM_S * 2) / (ITM_FH+1));
 itms_bs = (FLS_H  - ITM_S * 2 - ITM_FH * itms_max) / itms_max;

 y_at = (cur_h - 2 * txt_h - 2) + txt_h;
}

void menugrafic()
{
 char z[128];
 main_gui_free();

 font=FONT_SPISOK;
 txt_h   =  GetFontYSIZE(font);
 soft_h  =  GetFontYSIZE(FONT_SOFT);
 head_h  =  GetFontYSIZE(FONT_HEAD);
 popup_h =  GetFontYSIZE(FONT_MSG);
 prop_h  =  GetFontYSIZE(FONT_PROP);

 ////////////////////////////////////////////////////
 sprintf(Drives[3].icon, "%smmc.png",graf_folder);
 sprintf(Drives[2].icon, "%sdisks.png",graf_folder);
 sprintf(Drives[1].icon, "%sdisks.png",graf_folder);
 sprintf(Drives[0].icon, "%sdata.png",graf_folder);

 sprintf(stzip, "%szip.png",graf_folder);
 sprintf(st_bm, "%sbm.png",graf_folder);
 sprintf(st_seach, "%ssearch.png",graf_folder);
 ////////////////////////////////////////////////////
#ifdef NEWSGOLD
 if(CONFIG_LOAD_GRAF)
 {
    cursor=GetIMGHDRFromThemeCache(SELECTION_1_LINE);
    if(fol_h>cursor->h) cursor=GetIMGHDRFromThemeCache(SELECTION_2_LINE);
 }else
#endif
 {
  sprintf(z, "%scursor.png",graf_folder);
  cursor=CreateIMGHDRFromPngFile(z, !CONFIG_PP_CUR);
 }

#ifdef NEWSGOLD
 if(CONFIG_LOAD_GRAF)
 {
    IMGHDR *temp;
    background = malloc(sizeof(IMGHDR));
    #ifdef ELKA
      background->h=296;
      background->w=240;
    #else
      background->h=176;
      background->w=132;
    #endif

    temp=GetIMGHDRFromThemeCache(HEADLINE_STANDART);
    background->bpnum=temp->bpnum;
    char *map=malloc(background->h*background->w*background->bpnum);
    zeromem (map, background->h*background->w*background->bpnum);
    memcpy(map, temp->bitmap, CalcBitmapSize(temp->w,temp->h, temp->bpnum));

    int c=CalcBitmapSize(temp->w,temp->h, background->bpnum);
    temp=GetIMGHDRFromThemeCache(BODY_STANDART);
    memcpy(map+c, temp->bitmap, CalcBitmapSize(temp->w,temp->h, temp->bpnum));

    c+=CalcBitmapSize(temp->w,temp->h, background->bpnum);
    temp=GetIMGHDRFromThemeCache(BOTTOM_STANDART);
    memcpy(map+c, temp->bitmap, CalcBitmapSize(temp->w,temp->h, temp->bpnum));

    background->bitmap=map;
 }else
#endif
 {
    sprintf(z, "%sbackground.png",graf_folder);
    background=CreateIMGHDRFromPngFile(z, 0);
 }



 sprintf(z, "%sinfo.png",graf_folder);
 iinfo=CreateIMGHDRFromPngFile(z, 2);
 if(!iinfo)
 {
   iinfo = malloc(sizeof(IMGHDR));
   iinfo->bitmap = malloc(1);
   iinfo->bitmap = '\0';
   #ifdef ELKA
   iinfo->h=prop_h*9+4;
   #else
   iinfo->h=prop_h*7+4;
   #endif
   if(iinfo->h>scr_h1)
   {
     while(iinfo->h>scr_h1)
       iinfo->h-=prop_h;
   }
#ifdef ELKA
   iinfo->w=190;
#else
   iinfo->w=125;
#endif
 }

 sprintf(z, "%sproperties.png",graf_folder);
 iprop=CreateIMGHDRFromPngFile(z, 2);
 if(!iprop)
 {
   iprop = malloc(sizeof(IMGHDR));
   iprop->bitmap = malloc(1);
   iprop->bitmap = '\0';

   iprop->h=prop_h*13+2;
   if(iprop->h>scr_h1)
   {
     while(iprop->h>scr_h1)
       iprop->h-=prop_h;
   }
#ifdef ELKA
   iprop->w=190;
#else
   iprop->w=129;
#endif
 }

 IMGHDRW=GetImgWidth((int)Drives[0].icon);
}

void main_gui_free()
{
  if(cursor && !CONFIG_LOAD_GRAF)
  {
    if(cursor->bitmap)mfree(cursor->bitmap);
    mfree(cursor);
  }
  if(background)
  {
    if(background->bitmap)mfree(background->bitmap);
    mfree(background);
  }

  if(iprop)
  {
    if(iprop->bitmap)mfree(iprop->bitmap);
    mfree(iprop);
  }

  if(iinfo)
  {
    if(iinfo->bitmap) mfree(iinfo->bitmap);
    mfree(iinfo);
  }
}

void DrawCursor(int pos, int frag)
{
  if(frag)
  {
    if(background)
      DrwIMGHDR(background,TXT_X+1,pos,TXT_X+1,pos-YDISP1,TXT_X2-TXT_X,txt_h-1);
    else
      DrawRoundedFrame(TXT_X+1,pos,TXT_X2,pos+txt_h-1,0,0,0,0,(char*)&Colors[clListBG]);

    if(cursor)
      DrwIMGHDR(cursor,TXT_X+1,pos,TXT_X-((scr_w-cur_w)/2)+2,frag,(TXT_X2)-TXT_X1,txt_h);

  }else
  {
    if(cursor)
      DrwIMGHDR(cursor, CUR_X, pos, 0, 0, cursor->w, cursor->h);
    else
      DrawRoundedFrame(ITM_X1,pos,ITM_X2,pos+cur_h-1,2,2,0,(char*)&Colors[clCurBD],(char*)&Colors[clCurBG]);
  }
}

void DrawFon()
{
  if(background)
    DrwImage(background, 0, YDISP1, 0, 0, background->w, background->h, 0);
  else
  {
    DrawRoundedFrame(0,YDISP1,scr_w-1,scr_h1-1,0,0,0,
                     (char*)&Colors[clBD],(char*)&Colors[clBG]);

    DrawRoundedFrame(FLS_B,FLS_Y,scr_w-FLS_B-1,FLS_Y+FLS_H-1,0,0,0,
                     (char*)&Colors[clListBD],(char*)&Colors[clListBG]);
  }
}

int sm=0;
int forward=1;
int halt=0;
void scrooltext(WSHDR *ws)
{
  if (scwait<=0)
  {
    if(forward==1) sm+=CONFIG_SCROLL_STEP;
    if(forward==0) sm-=CONFIG_SCROLL_STEP;
    if(forward==2)
    {
      if(halt) halt--;
      else forward=3;
    }
    if ((sm>=(Get_WS_width(ws,font)-(TXT_X2-TXT_X))&&(forward==1)))
    {
      forward=2;
      halt=5;
      scwait = CONFIG_SCROLL_TEXT_WAIT/2;
    }
    if (forward==3)
    {
      if (sm==0)
      {
        forward=4;
        halt=5;
      }
      else
        sm-=CONFIG_SCROLL_STEP;
    }
    if(forward==4)
    {
      if(halt)
        halt--;
      else
      {
        forward=1;
        scwait = CONFIG_SCROLL_TEXT_WAIT/2;
      }
    }
  }else scwait--;
}

void DrawClock()
{
    if (Clock)
    {
      TTime time;
      GetDateTime(0,&time);
      wsprintf(guibuf, "%02i:%02i",time.hour,time.min);
      DrawString(guibuf,0,VISOTA-1,scr_w-1,VISOTA+soft_h,FONT_SOFT,2+32,
                (char*)&Colors[clSoft],(char*)&Colors[clSoftBD]);
    }
}

void DrwName()
{
 WSHDR* ws = ((curtab == systab || !CONFIG_SHOW_EXT) && scfile->ws_showname ? scfile->ws_showname : scfile->ws_name);
 if (!Busy && !Busy1 && !progr_start && scfile && IsGuiOnTop(MAINGUI_ID) && (Get_WS_width(ws,font)>(TXT_X2-TXT_X)))
 {
   int y = FLS_Y + 2 + scind * (ITM_FH + itms_bs);
   int tc;
   if (scfile->attr & (FA_READONLY | FA_SYSTEM))
    tc=clSelFileReadOnly;
   else if (scfile->attr & FA_HIDDEN )
    tc=clSelFileHidden;
   else
    tc=clSelFileNormal;

  int y_cur;
  if (items_vis==1 && CONFIG_SOFT_BUTTON)
    y_cur=(cur_h-(items_vis+1)*txt_h-2);
  else
    y_cur=(cur_h-txt_h)/2;

  int y2 = y+y_cur;

  DrawCursor(y2, y_cur);

  scrooltext(ws);
  DrawScrollString(ws,TXT_X+1,y2,TXT_X2,y2+txt_h,sm+1,font,1,(char*)&Colors[tc],(!cursor?(char*)&Colors[clCurBG]:0));
 }
 GBS_StartTimerProc(&sctm,CONFIG_SCROLL_TEXT_SPEED,DrwName);
}

void DrwFile(int ind, FILEINF* file)
{
 int y = FLS_Y + 2 + ind * (ITM_FH + itms_bs);

 int sm1=1;
 if (ind == _CurIndex-_CurBase)
 {
  if (scfile != file)
  {
   scfile = file;
   scind = ind;
   sctpos = 0;
   sctdir = 1;
   sm=0;
   sm1=1;
   scwait = CONFIG_SCROLL_TEXT_WAIT;
  }else
    sm1=sm+1;

  DrawCursor(y, 0);
//////////////////////////////////////////////////////
  if (items_vis && CONFIG_SOFT_BUTTON)
  {
    if (file->ws_attr)
      DrawString(file->ws_attr,TXT_X1,y+y_at,TXT_X2,y+y_at+txt_h,font,4+32,
                 (char*)&Colors[clInfoTxt],0);

    if (!(file->attr & FA_DIRECTORY))
    {
      if (file->ws_size)
        DrawString(file->ws_size,TXT_X1,y+y_at,TXT_X2,y+y_at+txt_h,font,TEXT_ALIGNLEFT+32,
                   (char*)&Colors[clInfoTxt],0);

      if (file->ftype == TYPE_ZIP_FILE && file->ws_ratio)
        DrawString(file->ws_ratio,TXT_X1,y+y_at,TXT_X2,y+y_at+txt_h,font,2+32,
                   (char*)&Colors[clInfoTxt],0);
    }else
    {
      wsprintf(guibuf,_t,"DIR");
      DrawString(guibuf,TXT_X1,y+y_at,TXT_X2,y+y_at+txt_h,font,TEXT_ALIGNLEFT+32,
                 (char*)&Colors[clInfoTxt],0);
    }
  }else
  {
    if(!CONFIG_SOFT_BUTTON)
    {
      if(SelParam.gsize == 0)
      {
        if (!(file->attr & FA_DIRECTORY))
        {
          if (file->ftype == TYPE_ZIP_FILE && file->ws_ratio)
            wsprintf(guibuf, "[%w][%w][%w]", file->ws_size, file->ws_attr, file->ws_ratio);
          else
          {
            if (file->ftype == TYPE_7Z_FILE)
              wsprintf(guibuf, "[%w][%w]", file->ws_size, file->ws_attr);
            else
              wsprintf(guibuf, "[%w][%w][T:%d]", file->ws_size, file->ws_attr, curtab+1);
          }
        }else
        {
          if (file->ftype == TYPE_ZIP_DIR || file->ftype == TYPE_7Z_DIR)
            wsprintf(guibuf, "[DIR][%w]", file->ws_attr);
          else
            wsprintf(guibuf, "[DIR][%w][T:%d]", file->ws_attr, curtab+1);
        }
      }else
      {
        sz2s(SelParam.gsize, msgbuf);
        if (file->ftype == TYPE_ZIP_DIR || file->ftype == TYPE_ZIP_FILE)
        {
          sz2s(SelParam.gcsize, pathbuf_fn);
          wsprintf(guibuf, "[%t][%t]", msgbuf, pathbuf_fn);
        }
        else
          wsprintf(guibuf, "[%t][T:%d]", msgbuf, curtab+1);
      }

      DrawString(guibuf,SMECHENIE,VISOTA-2,scr_w-3,VISOTA+soft_h,FONT_SOFT,1+32,
                (char*)&Colors[clSoft],(char*)&Colors[clSoftBD]);

    }
  }
 }
 DrawImg(ICO_X, y+ICO_Y, file->ext->ico);

 if (file->attr & FA_CHECK)
   DrawImg(ICO_X, y+ICO_Y, ext_chk.ico);

 int tc;
 if (file->attr & (FA_READONLY | FA_SYSTEM))
  tc=ind==_CurIndex-_CurBase?clSelFileReadOnly:clFileReadOnly;
 else if (file->attr & FA_HIDDEN)
  tc=ind==_CurIndex-_CurBase?clSelFileHidden:clFileHidden;
 else
  tc=ind==_CurIndex-_CurBase?clSelFileNormal:clFileNormal;

 WSHDR* fn = ((curtab == systab || !CONFIG_SHOW_EXT) && file->ws_showname ? file->ws_showname : file->ws_name);

 int y2;

 if ((items_vis==1) && (ind == _CurIndex-_CurBase) && CONFIG_SOFT_BUTTON)
   y2=y+(cur_h-(items_vis+1)*txt_h-2);
 else
   y2=y+(cur_h-txt_h)/2;

 DrawScrollString(fn,TXT_X+1,y2,TXT_X2,y2+txt_h,sm1,font,1,(char*)&Colors[tc],0);
}

int count_x1;

void DrwDrvAc()
{
  int x=DRV_X;
  int icon = 0;
  int ind=_CurDrv;

  if (curtab == systab)
  {
    if (isseach==1)
    {
      wsprintf(guibuf, _t, muitxt(ind_mcsearch));
      icon=(int)st_seach;
    }
    else
    {
      wsprintf(guibuf, _t, muitxt(ind_mcvol));
      icon=(int)st_bm;
    }
  }else
  if (curtab < MAX_TABS)
  {
    icon=inarh?(int)stzip:(int)&Drives[ind].icon;
    if (CONFIG_PUT_IN_HEAD)
      str_2ws(guibuf,CurPath(),MAX_PATH);
    else
    {
      if (inarh)
      {
        if(strlen(_CurTab->zipInfo->szCurDir)==0)
          wsprintf(guibuf, _t, muitxt(ind_arch));
        else
          str_2ws(guibuf, strrchr(_CurTab->zipInfo->szCurDir,'\\')+1, MAX_PATH);
      }else
        str_2ws(guibuf,strlen(_CurPath)==2?Drives[ind].name:strrchr(_CurPath,'\\')+1,MAX_PATH);
    }
  }

  DrawImg(DRV_X,TOP_Y+TOP_B-2,icon);
  if(IMGHDRW!=0)
    x = DRV_X+IMGHDRW+1;
  else
    x = SMECHENIE;
  DrawScrollString(guibuf,x+1,TOP_Y+TOP_B-1,count_x1,TOP_Y+TOP_B+TOP_H-1,1,FONT_HEAD,1+32,
                  (char*)&Colors[clHeadTxt],(char*)&Colors[clHeadTxtBD]);
}

void DrwSoftButton(char *Lbut,char *Rbut)
{
    wsprintf(guibuf, _t, Lbut);
    DrawString(guibuf,SMECHENIE,VISOTA-1,scr_w/2-16,VISOTA+soft_h,FONT_SOFT,1+32,
               (char*)&Colors[clSoft],(char*)&Colors[clSoftBD]);

    wsprintf(guibuf, _t, Rbut);
    DrawString(guibuf,scr_w/2+16,VISOTA-1,scr_w-1-SMECHENIE,VISOTA+soft_h,FONT_SOFT,4+32,
               (char*)&Colors[clSoft],(char*)&Colors[clSoftBD]);

    DrawClock();
}

void DrwIndex()
{
  int chkcnt = EnumChk(0, 0);

  if (chkcnt)
    wsprintf(guibuf,"%d(%d)", chkcnt, _CurCount);
  else
  {
    SelParam.gsize = SelParam.gcsize = 0;
    wsprintf(guibuf,"%d/%d", _CurIndex+1, _CurCount);
  }

  count_x1=(TAB_X+TAB_W-2)-Get_WS_width(guibuf,FONT_HEAD)-3;

  DrawString(guibuf,
             count_x1-3,
             TOP_Y+TOP_B-1,
             TAB_X+TAB_W-2,
             TOP_Y+TOP_B+TOP_H-1,
             FONT_HEAD,
             TEXT_ALIGNRIGHT+TEXT_OUTLINE,
             (char*)&Colors[clHeadTxt],
             (char*)&Colors[clHeadTxtBD]);
}

int GetSBY(int i, int c)
{
  return (c<=1)?0:((SB_H-SB_VS+1)*i/(c-1));
}

void DrwSB()
{
 int sby = FLS_Y+GetSBY(_CurIndex, _CurCount);
#ifdef ELKA
 DrawLine(scr_w-3, FLS_Y, scr_w-3, FLS_Y+FLS_H-1, 0,(char*)&Colors[clScBar]);
 DrawLine(scr_w-2, FLS_Y, scr_w-2, FLS_Y+FLS_H-1, 0,(char*)&Colors[clScBar]);
 DrawRectangle(scr_w-4,sby,scr_w-1,sby+SB_VS, 0, 0, (char*)&Colors[clScBarBG]);
#else
 DrawLine(scr_w-3, FLS_Y, scr_w-3, FLS_Y+FLS_H-1, 0,(char*)&Colors[clScBar]);
 DrawRectangle(scr_w-4,sby,scr_w-2,sby+SB_VS, 0, 0, (char*)&Colors[clScBarBG]);
#endif
}

void NULLSoft()
{
  if (inarh)
    wsprintf(guibuf, "[---][----]");
  else
    wsprintf(guibuf, "[---][----][T:%d]", curtab+1);

  DrawString(guibuf,SMECHENIE,VISOTA-2,scr_w-3,VISOTA+soft_h,FONT_SOFT,1+32,
                (char*)&Colors[clSoft],(char*)&Colors[clSoftBD]);
}

void ShowFiles()
{
 if (!guibuf) return;
 inarh=IsInArchive();

 DrawFon();

 DrwIndex();
 DrwDrvAc();

 if (CONFIG_SOFT_BUTTON)
   DrwSoftButton(muitxt(ind_mcmenu),muitxt(ind_back));

 int ibase, icount;
 if (_CurCount<=itms_max)
 {
  icount=_CurCount;
  ibase=0;
 }
 else
 {
  icount=itms_max;
  ibase=_CurBase;

  int idist = _CurIndex - _CurBase;

  if (idist >= itms_max-1) ibase += idist - itms_max + 2;
  else if (_CurIndex <= _CurBase) ibase += idist - 1;

  if (ibase < 0) ibase = 0;
  else if (ibase > _CurCount - itms_max) ibase = _CurCount - itms_max;
 }

 _CurBase = ibase;

 FILEINF *file = FileListBase[curtab];

 for(int ii=0; ii<ibase; ii++)
   file=file->next;

 if (icount==0)
 {
   wsprintf(guibuf, _t, muitxt(ind_empty));
   DrawString(guibuf,0,FLS_Y+12+SB_B,scr_w,FLS_Y+12+SB_B+5+txt_h,font,2+TEXT_OUTLINE,(char*)&Colors[clSelFileNormal],0);
   if (!CONFIG_SOFT_BUTTON)
     NULLSoft();
 }else
 {
  for(int ii=0; ii<icount; ii++)
  {
    file = file->next;
    FillFileInfo(file);
    DrwFile(ii, file);
  }
  DrwSB();
 }
}

void ShowProgr()
{
 RECT rc = DrawINF();
 int y_pr=rc.y+(rc.y2-rc.y)/2-5;

// Отрисовка состояния прогресса (%, завершонных, всего)
 int p = progr_cur * 100 / progr_max;

 if (progr_act)
 {
  wsprintf(guibuf, "%d%c - %t", p, 37, muitxt(progr_act));
  DrawString(guibuf,rc.x+3,rc.y+3,rc.x2-3,rc.y+popup_h+5,FONT_MSG,2+32,
             (char*)&Colors[clInfo],0);
 }

 DrawScrollString(progr_name,rc.x+3,y_pr-popup_h-5,rc.x2-3,y_pr-5,1,FONT_MSG,
            TEXT_ALIGNMIDDLE+TEXT_OUTLINE,(char*)&Colors[clInfo],0);

//---------------------------------------------------------------------

 p = progr_cur * ((rc.x2-3)-(rc.x+3)-2) / progr_max;

 DrawRectangle(rc.x+3, y_pr, rc.x2-3, y_pr+PRGP_H, 0, (char*)&Colors[clProgrBD],( char*)&Colors[clProgrBG]);
 DrawRectangle(rc.x+4, y_pr+1, rc.x+4+p, y_pr+PRGP_H-1, 0, 0, (char*)&Colors[clCurProgrBG]);

 if (progrsp_start)
 {
  DrawRectangle(rc.x+3, y_pr+PRGP_H+3, rc.x2-3, y_pr+PRGP_H+3+PRGP_H, 0, (char*)&Colors[clProgrBD],(char*)&Colors[clProgrBG]);

  if (progrsp_cur>0)
  {
    double po = progrsp_cur * ((rc.x2-3)-(rc.x+3)-2) / progrsp_max;
    DrawRectangle(rc.x+4, y_pr+PRGP_H+4, rc.x+po+4, y_pr+PRGP_H*2+2, 0, 0,(char*)&Colors[clCurProgrBG]);
  }
  if(progrsp_max > 0)
  {
    char sbuff[32], sbuff2[32];
    sz2s(progrsp_cur,  sbuff);
    sz2s(progrsp_max, sbuff2);
    wsprintf(guibuf, "%t / %t", sbuff, sbuff2);
    DrawString(guibuf,rc.x+3,y_pr+PRGP_H*2+7,rc.x2-3,rc.y2-5,FONT_MSG,2,(char*)&Colors[clInfo],0);
  }

 }
 wsprintf(guibuf, _t, muitxt(ind_cncl));
 DrawString(guibuf,rc.x+4,rc.y2-5-popup_h,rc.x2-6+smes,rc.y2-5,FONT_MSG,4,(char*)&Colors[clInfo],0);

 wsprintf(guibuf,"%d / %d", progr_cur, progr_max);
 DrawString(guibuf,rc.x+4,rc.y2-5-popup_h,rc.x2-6+smes,rc.y2-5,FONT_MSG,
            1,(char*)&Colors[clInfo],0);
}

RECT DrawINF()
{
  RECT rc;
  rc.x=(scr_w-iinfo->w)/2;
  rc.y=(scr_h1-iinfo->h)/2;
  rc.x2=rc.x+iinfo->w;
  rc.y2=rc.y+iinfo->h;

  DrawScreen();

  if(iinfo->bitmap != '\0')
    DrwImage(iinfo, rc.x, rc.y, 0, 0, iinfo->w, iinfo->h,0);
  else
  {
    if(background)
      DrwImage(background, rc.x, rc.y, rc.x, rc.y-YDISP1, iinfo->w, iinfo->h,0);

    DrawRoundedFrame(rc.x, rc.y, rc.x2, rc.y2,
                     0,0,0,(char*)&Colors[clInfoBD],(char*)&Colors[clInfoBG]);
  }
  return rc;
}

RECT DrawPROP()
{
  RECT rc;
  rc.x=(scr_w-iprop->w)/2;
  rc.y=(scr_h1-iprop->h)/2;
#ifdef ELKA
  if(/*iconbar && */rc.y <= 24) rc.y = rc.y + 24;
#endif
  rc.x2=rc.x+iprop->w;
  rc.y2=rc.y+iprop->h;
  DrawScreen();

  if(iprop->bitmap != '\0')
    DrwImage(iprop, rc.x, rc.y, 0, 0, iprop->w, iprop->h,0);
  else
  {
    if(background)
      DrwImage(background, rc.x, rc.y, rc.x, rc.y-YDISP1, iprop->w, iprop->h, 0);

    DrawRoundedFrame(rc.x, rc.y, rc.x2, rc.y2,
                     0,0,0,(char*)&Colors[clInfoBD],(char*)&Colors[clInfoBG]);
  }
  return rc;
}

