#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\gui.h"
#include "inc\ColorMap.h"
#include "inc\NewMenu.h"
#include "inc\ScreenMake.h"
#include "inc\popup.h"
#include "..\inc\md5.h"
#include "..\inc\siemens_unicode.h"

int fa_attr,
    fa_ro, // Только чтение
    fa_hd, // Скрытый
    fa_sy, // Системный
    fa_ar, // Архивный 
    fa_sb; // Учитывать подкаталоги

int filepropshow=0;
WSHDR *Page_1, *Page_Print;
int pcur=1;
int cbc=0;
int strcount=0;
int maxVcount=0;
int maxch=0;
int ii1;

void FillInfo()
{
  Page_1=AllocWS(768);
  Page_Print=AllocWS(256);
  maxVcount=(iprop->h-11-prop_h*(3+HEADP))/prop_h;
}

void FreeInfo()
{
  FreeWS(Page_1);
  FreeWS(Page_Print);
}

int GetCount(WSHDR *prop,int width)
{
 strcount=1;
 int strw=0;
 int perii=0;
 int font_opt=0;
 
 for(int ii=1;ii<prop->wsbody[0];ii++)
 {
  if(prop->wsbody[ii]==UTF16_FONT_SMALL) 
    font_opt=0;
  
  if(prop->wsbody[ii]==UTF16_FONT_SMALL_BOLD) 
    font_opt=1;
   
  if(prop->wsbody[ii]=='\n' || prop->wsbody[ii]==UTF16_ENA_CENTER)
  {
    strcount++;
    strw=0;
    perii=0;
    
    continue;
    
  }else
    strw+=GetSymbolWidth(prop->wsbody[ii], FONT_PROP+font_opt);
  
  if(prop->wsbody[ii]==' ')
    perii=ii;
  
  if(strw>width)
  {
    if(perii!=0)
    {
      prop->wsbody[perii]='\n';
      if(font_opt==1)
        wsInsertChar(prop,UTF16_FONT_SMALL_BOLD,perii+1); 
    }
    else
    {
      if(font_opt==1)
        wsInsertChar(prop,UTF16_FONT_SMALL_BOLD,ii); 
      wsInsertChar(prop,45,ii);
      wsInsertChar(prop,'\n',ii);
    }
    
    strcount++;
    strw=0;
  }  
 }
 
 maxch=strcount-maxVcount;
 if(maxch<0) maxch=0;
 return 0;
}

void CPYINFO()
{
  int sctpos=0;
  int c=0;
  
  for(int i=1; i<Page_1->wsbody[0]; i++)
  {
    sctpos++;
    if(Page_1->wsbody[i]=='\n' || Page_1->wsbody[i]==UTF16_ENA_CENTER) c++;
    if(pcur==c) break;      
  }
  wstrcpybypos(Page_Print, Page_1, sctpos+1, Page_1->wsbody[0]-sctpos);
}

int str2int(const char *str)
{
  int n = 0;
  sscanf(str, _d, &n);
  return(n);
}

void GetTime(WSHDR *dur, WSHDR *buffws)
{
  int i;  
  for(i=1; i<dur->wsbody[0]; i++)
    if(dur->wsbody[i]=='\n') break;
  
  if(i!=dur->wsbody[0])
  {    
    if(dur->wsbody[dur->wsbody[0]-1]==115)
    {
      wstrcpybypos(guibuf, dur, i+3, dur->wsbody[0]-i-5);
      ws_2str(guibuf, msgbuf, dur->wsbody[0]-i-5); 
    
      int sec=str2int(msgbuf)+1;

      wsprintf(buffws,"%t: %c%02d:%02d (%d s)%c\n",
               muitxt(ind_time),UTF16_FONT_SMALL_BOLD,sec/60, sec-(sec/60)*60, sec,UTF16_FONT_SMALL); 
    }else
    {
      wstrcpybypos(guibuf, dur, i+3, dur->wsbody[0]-i-3);
      ws_2str(guibuf, msgbuf, dur->wsbody[0]-i-3); 

      wsprintf(buffws,"%t: %c%t%c\n",
               muitxt(ind_time),UTF16_FONT_SMALL_BOLD,msgbuf,UTF16_FONT_SMALL);  
    }
  }else
    wsprintf(buffws,_fullnull);
}

int GetID3_Tag()
{
#ifdef NEWSGOLD  
  if (IsMedia(scfile->sname) && !(IsMultiChk()>1))
  {
    CurFullPath(scfile);
    WSHDR *fol = AllocWS (strlen(_CurPath));
    WSHDR *fil = AllocWS (strlen(scfile->sname));
    WSHDR *ws1 = AllocWS (strlen(pathbuf));
    WSHDR *ext = AllocWS (4);
  
    str_2ws(ws1,pathbuf,MAX_PATH);

    str_2ws(fol,_CurPath,MAX_PATH);
    str_2ws(fil,scfile->sname,MAX_PATH);
    
    FILE_PROP wl;
    zeromem(&wl, sizeof(wl));
    
    wl.type=AUDIO_MP3;
    wl.filename=ws1;
    
    str_2ws(ext, EXT_MP3,3);
    wl.uid=GetExtUid_ws(ext);
     
    wl.tag_artist_ws = AllocWS (60);
    wl.tag_title_ws = AllocWS (60);
    wl.tag_album_ws = AllocWS (60);
    wl.tag_track_ws = AllocWS (4);
    wl.tag_year_ws = AllocWS (4);    
    wl.bitrate_ws = AllocWS (8);
    wl.audio_frequency_ws = AllocWS (8);   
    wl.duration_mp3_ws = AllocWS (24);
    wl.unk_tag_2_ws = AllocWS (30);
    
    GetFileProp(&wl,fil,fol);
    
    if(wstrlen(wl.tag_artist_ws)!=0   ||
       wstrlen(wl.tag_title_ws)!=0    ||
       wstrlen(wl.tag_album_ws)!=0    ||
       wstrlen(wl.tag_track_ws)!=0    ||
       wstrlen(wl.unk_tag_2_ws)!=0    ||
       wstrlen(wl.tag_year_ws)!=0     ||
       wstrlen(wl.duration_mp3_ws)!=0 ||
       wstrlen(wl.bitrate_ws)!=0      ||
       wstrlen(wl.audio_frequency_ws)!=0)
    wstrcatprintf(Page_1, "%c%c%t:%c%c",UTF16_DIS_CENTER, UTF16_ENA_UNDERLINE, "ID3", UTF16_DIS_UNDERLINE, UTF16_ENA_CENTER);

    // Артист
    if(wstrlen(wl.tag_artist_ws)!=0)
    {
      RusTeg(wl.tag_artist_ws);
      wstrcatprintf(Page_1,"%t:\n%c%w%c\n",
               muitxt(ind_artist),UTF16_FONT_SMALL_BOLD,wl.tag_artist_ws,UTF16_FONT_SMALL);
    }
    //  Название
    if(wstrlen(wl.tag_title_ws)!=0)
    {
      RusTeg(wl.tag_title_ws);
      wstrcatprintf(Page_1,"%t:\n%c%w%c\n",
               muitxt(ind_title),UTF16_FONT_SMALL_BOLD,wl.tag_title_ws,UTF16_FONT_SMALL);
    }

    // Альбом
    if(wstrlen(wl.tag_album_ws)!=0)
    {
      RusTeg(wl.tag_album_ws);
      wstrcatprintf(Page_1,"%t:\n%c%w%c\n",
               muitxt(ind_album),UTF16_FONT_SMALL_BOLD,wl.tag_album_ws,UTF16_FONT_SMALL);
    }
    
    // Трек
    if(wstrlen(wl.tag_track_ws)!=0)
      wstrcatprintf(Page_1,"%t: %c%w%c\n",
               muitxt(ind_treak),UTF16_FONT_SMALL_BOLD,wl.tag_track_ws,UTF16_FONT_SMALL);
    
    // Жанр
    if(wstrlen(wl.unk_tag_2_ws)!=0)
    {
      RusTeg(wl.unk_tag_2_ws);
      wstrcatprintf(Page_1,"%t: %c%w%c\n",
               muitxt(ind_coment),UTF16_FONT_SMALL_BOLD,wl.unk_tag_2_ws,UTF16_FONT_SMALL); 
    }
    // Год
    if(wstrlen(wl.tag_year_ws)!=0)
      wstrcatprintf(Page_1,"%t: %c%w%c\n",
               muitxt(ind_year),UTF16_FONT_SMALL_BOLD,wl.tag_year_ws,UTF16_FONT_SMALL);
    // Время
    if(wstrlen(wl.duration_mp3_ws)!=0)
    {
      GetTime(wl.duration_mp3_ws, wsbuf);
      if(wstrlen(wsbuf)!=0) wstrcat(Page_1, wsbuf);
    }
    // Битрейт
    if(wstrlen(wl.bitrate_ws)!=0)
      wstrcatprintf(Page_1,"%t: %c%w%c\n",
               muitxt(ind_bitrate),UTF16_FONT_SMALL_BOLD,wl.bitrate_ws,UTF16_FONT_SMALL);
    // Частота
    if(wstrlen(wl.audio_frequency_ws)!=0)
      wstrcatprintf(Page_1,"Freq: %c%w%c\n",UTF16_FONT_SMALL_BOLD,wl.audio_frequency_ws,UTF16_FONT_SMALL);
    
    /////////////////////
    FreeWS(wl.tag_artist_ws);
    FreeWS(wl.tag_title_ws);
    FreeWS(wl.tag_album_ws);
    FreeWS(wl.tag_year_ws);
    FreeWS(wl.tag_track_ws);
    FreeWS(wl.bitrate_ws);
    FreeWS(wl.audio_frequency_ws);
    FreeWS(wl.duration_mp3_ws);
    FreeWS(wl.unk_tag_2_ws);
    /////////////////////
    FreeWS(fol);
    FreeWS(fil);
    FreeWS(ws1);
    FreeWS(ext);
    
    return 1;
  }
#endif
  return 0;
}

int GetImgProp()
{
  if (IsPict(scfile->sname) && !(IsMultiChk()>1))
  {
    FILE_PROP wl;
    CurFullPath(scfile);
    WSHDR *fol = AllocWS (strlen(_CurPath));
    WSHDR *fil = AllocWS (strlen(scfile->sname));
    WSHDR *ws1 = AllocWS (strlen(pathbuf));
    WSHDR *ext = AllocWS (4);
    
    str_2ws(ws1,pathbuf,MAX_PATH);

    str_2ws(fol,_CurPath,MAX_PATH);
    str_2ws(fil,scfile->sname,MAX_PATH);

    zeromem(&wl, sizeof(wl));

    wl.filename=ws1;
    char* sz = GetFileExt(scfile->sname);
    char szext[MAX_EXT];
    strtolower(sz, szext, MAX_EXT);
     
    str_2ws(ext, szext,5);
    wl.uid=GetExtUid_ws(ext);
    wl.type=PICT;
    
    GetFileProp(&wl,fil,fol);

    if(wl.width!=0 && wl.height!=0)
    {
      wstrcatprintf(Page_1,"%t: %c%dx%d%c\n",
               muitxt(ind_resol),UTF16_FONT_SMALL_BOLD,wl.width,wl.height,UTF16_FONT_SMALL);
    }
    
    FreeWS(fol);
    FreeWS(fil);
    FreeWS(ws1);
    FreeWS(ext);
    return 1;
  }
  return 0;
}

int GetVideoProp()
{
#ifdef NEWSGOLD
 if(IsVideo(scfile->sname) && !(IsMultiChk()>1))
 {
    FILE_PROP wl;
    
    CurFullPath(scfile);
    WSHDR *fol = AllocWS (strlen(_CurPath));
    WSHDR *fil = AllocWS (strlen(scfile->sname));
    WSHDR *ws1 = AllocWS (strlen(pathbuf));
    WSHDR *ext = AllocWS (4);
    
    str_2ws(ws1,pathbuf,MAX_PATH);
    str_2ws(fol,_CurPath,MAX_PATH);
    str_2ws(fil,scfile->sname,MAX_PATH);

    zeromem(&wl, sizeof(wl));

    wl.filename=ws1;
    char* sz = GetFileExt(scfile->sname);
    char szext[MAX_EXT];
    strtolower(sz, szext, MAX_EXT);
     
    str_2ws(ext, szext,4);
    wl.uid=GetExtUid_ws(ext);
    wl.type=VIDEO;
    wl.audio_frequency_ws = AllocWS (8); 
    wl.duration_mp3_ws = AllocWS (24);

    GetFileProp(&wl,fil,fol);

    if(wl.width!=0 && wl.height!=0)
    {
      wstrcatprintf(Page_1,"%t: %c%dx%d%c\n",
               muitxt(ind_resol),UTF16_FONT_SMALL_BOLD,wl.width,wl.height,UTF16_FONT_SMALL);
    }
#ifdef ELKA
    if(wstrlen(wl.duration_mp3_ws)!=0)
    {
      GetTime(wl.duration_mp3_ws, wsbuf);
      wstrcat(Page_1, wsbuf);
    }
#endif
    FreeWS(wl.duration_mp3_ws);
    FreeWS(fol);
    FreeWS(fil);
    FreeWS(ws1);
    FreeWS(ext);
   return 1;
 }
#endif
 return 0;
}

void AddAttr()
{
  wstrcatprintf(Page_1, "%c%c%t:%c%c",UTF16_DIS_CENTER, UTF16_ENA_UNDERLINE, muitxt(ind_prop), UTF16_DIS_UNDERLINE, UTF16_ENA_CENTER);
    
  wstrcatprintf(Page_1, "%t:\n%t:\n%t:\n%t:\n",
                muitxt(ind_readonly),muitxt(ind_hidden),muitxt(ind_system),muitxt(ind_arh));
  ii1=3;
  if (scfile->ftype == TYPE_COMMON_DIR || IsMultiChk()>1)
  {
    wstrcatprintf(Page_1, "%t:\n",muitxt(ind_subdirs));
    ii1++;
  }
}

void GetMoreInfo()
{
  if(!IsInArchive() && !(IsMultiChk()>1))
  {
    GetID3_Tag();
    GetImgProp();
    GetVideoProp();
  }        
}
 
void GetProp()
{   
  char sbuff[128];
  char sbuff2[128];
  
  wsprintf(Page_1, _fullnull);  
  
  wstrcatprintf(Page_1, " %c%c%t:%c%c", UTF16_DIS_CENTER, UTF16_ENA_UNDERLINE, muitxt(ind_size), UTF16_DIS_UNDERLINE, UTF16_ENA_CENTER);
  if (IsMultiChk()>1 || scfile->attr & FA_DIRECTORY)
  {
    CHKDS_PARAM param;
    param.gsize=0;
    param.gcsize=0;
    param.count=0;
    EnumSel(M_DirSize, (int)&param);

    sz2s(param.gsize, sbuff);
    sz2s(param.gcsize, sbuff2);
    if(scfile->ftype == TYPE_ZIP_FILE || scfile->ftype == TYPE_ZIP_DIR)
      wstrcatprintf(Page_1, "%t: %c%t%c\n%t: %c%t%c\n%t: %c%d%c\n%t: %c%d%c\n" ,
                muitxt(ind_global), UTF16_FONT_SMALL_BOLD, sbuff ,UTF16_FONT_SMALL,
                muitxt(ind_cglobal), UTF16_FONT_SMALL_BOLD, sbuff2 ,UTF16_FONT_SMALL,
	        muitxt(ind_folders), UTF16_FONT_SMALL_BOLD, param.count>>16 ,UTF16_FONT_SMALL,
		muitxt(ind_files), UTF16_FONT_SMALL_BOLD, param.count & 0xffff ,UTF16_FONT_SMALL);
    else
      wstrcatprintf(Page_1, "%t: %c%t%c\n%t: %c%d%c\n%t: %c%d%c\n" ,
		muitxt(ind_global),UTF16_FONT_SMALL_BOLD,sbuff,UTF16_FONT_SMALL,
		muitxt(ind_folders),UTF16_FONT_SMALL_BOLD,param.count>>16,UTF16_FONT_SMALL,
		muitxt(ind_files),UTF16_FONT_SMALL_BOLD,param.count & 0xffff,UTF16_FONT_SMALL);
  }
  else
  {
    if (scfile->ftype == TYPE_ZIP_FILE)
      wstrcatprintf(Page_1, "%t: %c%u %t%c\n%t: %c%u %t%c\n",
		muitxt(ind_ncsize),UTF16_FONT_SMALL_BOLD,scfile->size,muitxt(ind_byte),UTF16_FONT_SMALL,
		muitxt(ind_csize),UTF16_FONT_SMALL_BOLD,scfile->csize,muitxt(ind_byte),UTF16_FONT_SMALL);
    else
      wstrcatprintf(Page_1, "%c%w (%u %t)%c\n",
	        UTF16_FONT_SMALL_BOLD,scfile->ws_size,scfile->size,muitxt(ind_byte),UTF16_FONT_SMALL);
  }

  //Date
  fd2s(scfile->time, sbuff);
  ft2s(scfile->time, sbuff2);
  wstrcatprintf(Page_1,"%c%c%t:%c%c%t %t %t\n", UTF16_DIS_CENTER, UTF16_ENA_UNDERLINE, muitxt(ind_date),UTF16_DIS_UNDERLINE, UTF16_ENA_CENTER, sbuff, muitxt(ind_in), sbuff2);
  
}

void DoFilePropNew()
{
  if(scfile==NULL) return;
  
  allclose();
  DoScreen();
  
  fa_ro=((scfile->attr & FA_READONLY)?1:0);
  fa_hd=((scfile->attr & FA_HIDDEN)?1:0);
  fa_sy=((scfile->attr & FA_SYSTEM)?1:0);
  fa_ar=((scfile->attr & FA_ARCHIVE)?1:0);
  fa_sb = 0;
  pcur=0;
  cbc=0;
    
  GetProp();
  GetMoreInfo();
  AddAttr();
  GetCount(Page_1, iprop->w-smes*2-3);
  CPYINFO();

  filepropshow=1;
  Busy1=1;
  fropon=1; 
}

void DoFilePropClose(int save)
{
  if (pathbuf && save && !IsInArchive() && isseach==0)
  {
    fa_attr=0;
    if (fa_ro) fa_attr|=FA_READONLY;
    if (fa_hd) fa_attr|=FA_HIDDEN;
    if (fa_sy) fa_attr|=FA_SYSTEM;
    if (fa_ar) fa_attr|=FA_ARCHIVE;

    int res = 1;
    EnumSel(M_SetAttr, (int)&res);
  
    if (!res)
      DoShowMsg(muitxt(ind_err_attrset));
    else
      DoRefresh();
  }

  ClearScreen();
  filepropshow=0;
  Busy1=0;
  fropon=0;
}

void UPKEY()
{
  cbc--;
#ifdef SCROOLLOOCK
  if(cbc==-1)
  {
    pcur=maxch;
    CPYINFO();
    cbc=ii1+1;
  }else
#endif
  {
    if(cbc<1 && pcur>0)
    {
      pcur--;
      CPYINFO();
      cbc=-1;
    }
    if(pcur==0 && cbc<1) cbc=0;
  }
}

void DOWNKEY()
{
  int left=(strcount-maxVcount-pcur); 
#ifdef SCROOLLOOCK
if(cbc==ii1+1)
{
  pcur=0;
  CPYINFO();
  cbc=0;
}else
#endif
{
  if((ii1<=left-1 || (ii1-cbc)==left-1) && left>0)
  {
    pcur++;
    CPYINFO();
  }
        
  if(ii1>=left-1)
  {
    if(cbc<ii1+1)
    cbc=(cbc>-1)?cbc++:1;
  }
  else
    cbc=-1;
}
}

void setkey(int pos)
{
  if(cbc>pos)
  {
    while(cbc!=pos)
    {
      cbc--;
      if(cbc<1 && pcur>0)
      {
        pcur--;
        cbc=-1;
      }
      if(pcur==0 && cbc<1) cbc=0;
    }
  }
  
  if(cbc<pos)
  {
    while(cbc!=pos)
    {
      int left=(strcount-maxVcount-pcur);
      if((ii1<=left-1 || (ii1-cbc)==left-1) && left>0)
        pcur++;
        
      if(ii1>=left-1)
      {
        if(cbc<ii1+1) cbc=(cbc>-1)?cbc++:1;
      }
      else
        cbc=-1;
    }
  }
  
  CPYINFO();
}

void fileprop_key(int key)
{  
  switch(key)
  {
    case(RIGHT_SOFT):
      DoFilePropClose(0);
      break;
    case(LEFT_SOFT):
      DoFilePropClose(1);
      break;
    case(UP_BUTTON):
      UPKEY();
      break;
    case(DOWN_BUTTON):
      DOWNKEY();
      break;
    case(ENTER_BUTTON):  
      switch(cbc)
      {
      case 0:
        if (IsMultiChk()>1)
          CurPath(scfile->sname);
        else
          CurFullPath(scfile);

        str_2ws(guibuf, pathbuf, MAX_PATH);
        FullPatch(guibuf);
        break;
        
      case 1:
        if(!IsInArchive())
          fa_ro = !fa_ro;
        break;
      case 2:
        if(!IsInArchive())
          fa_hd = !fa_hd;
        break;
      case 3:
        if(!IsInArchive())
          fa_sy = !fa_sy;
        break;
      case 4:
        if(!IsInArchive())
          fa_ar = !fa_ar;
        break;
      case 5:
        if ((scfile->ftype == TYPE_COMMON_DIR || IsMultiChk()>1) && !IsInArchive())
          fa_sb = !fa_sb;
        break;
      }
      if(cbc>0 && cbc<ii1+1) DOWNKEY();
      break;
      
    case '3':
      if(!IsInArchive())
      {
        fa_ro = !fa_ro;
        setkey(1);
      }
      break;
    case '6':
      if(!IsInArchive())
      {
        fa_hd = !fa_hd;
        setkey(2);
      }
      break;
    case '9':
      if(!IsInArchive())
      {
        fa_sy = !fa_sy;
        setkey(3);
      }
      break;
    case '#':
      if(!IsInArchive())
      {
        fa_ar = !fa_ar;
        setkey(4);
      }
      break;
    case '0':
      if (ii1==4 && !IsInArchive())
      {
        fa_sb = !fa_sb;
        setkey(5);
      }
      break;
    case '1':
      if (!IsInArchive() && scfile->ftype == TYPE_COMMON_FILE && !(IsMultiChk()>1))
      {
        CurFullPath(scfile);
        GetMD5(pathbuf);
      }
      break;         
  }
}

void DrawCheckBox(int x, int y, int iscur, int ischeck)
{
  RECT box;
  box.x  = x;
  box.y  = y;
  box.x2 = x+prop_h-2;
  box.y2 = y+prop_h-2;
  
  DrawRectangle(box.x, box.y, box.x2, box.y2,0, iscur?(char*)&Colors[clCheckSel]:(char*)&Colors[clCheckBD],NULL);
  if(ischeck)
    DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                  (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
}

int IsAttrEnter(int i)
{
  if((i==1 && fa_ro) ||
     (i==2 && fa_hd) ||
     (i==3 && fa_sy) ||
     (i==4 && fa_ar) ||
     (i==5 && fa_sb)) 
     return 1;
  
  return 0;
}

void DrwFileProp()
{
  RECT rc = DrawPROP();
  //////////////////////////////////////////////////////////////////////////////
#ifdef ELKA
  wsprintf(guibuf, _t, muitxt(ind_prop));
  DrawString(guibuf,rc.x+smes,rc.y+3,rc.x2-smes,rc.y+prop_h+5,FONT_PROP,2,
             (char*)&Colors[clInfo],NULL);
#endif
  
  /*// Форма для вывода имени файла      
  if (IsMultiChk()>1)
    wsprintf(guibuf, "%t:", muitxt(ind_fullpatch));
  else  
    wsprintf(guibuf, "%t:", muitxt(ind_name));
  */
  
  wsprintf(guibuf, "%c%c%t:%c%c", UTF16_DIS_CENTER, UTF16_ENA_UNDERLINE, IsMultiChk()>1?muitxt(ind_fullpatch):muitxt(ind_name), UTF16_DIS_UNDERLINE, UTF16_ENA_CENTER);
  
  DrawString(guibuf,rc.x+smes,rc.y+3+prop_h*HEADP,rc.x2-smes,rc.y2-5,FONT_PROP,1,(char*)&Colors[clInfo],NULL);
    
  DrawRoundedFrame(rc.x+2, rc.y+4+prop_h*(1+HEADP), rc.x2-2, rc.y+prop_h*(2+HEADP)+5,0,0,0,
                   !cbc?(char*)&Colors[clCheckSel]:(char*)&Colors[clCheckBD],(char*)&Colors[clCheckBG]);
      
  if (IsMultiChk()>1)
  {
    CurPath(scfile->sname);
    str_2ws(guibuf, pathbuf, MAX_PATH);
  }
  else
    str_2ws(guibuf, scfile->sname, MAX_PATH);
    
  DrawScrollString(guibuf,rc.x+smes, rc.y+5+prop_h*(1+HEADP), rc.x2-smes, rc.y+prop_h*(2+HEADP)+27,1,FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  //////////////////////////////////////////////////////////////////////////////
  // Вывод инфы
  DrawString(Page_Print,rc.x+smes-1,rc.y+prop_h*(2+HEADP)+7,rc.x2-smes-2,rc.y2-6-prop_h,FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  
  int left=(strcount-maxVcount-pcur);   
  if(ii1>=left)
  {
    int smesh=0, attribute=0;
    if(left<0){smesh=left; left=0;}
    for(int i=(ii1-left); i>=0; i--)
    {
      attribute++;
      DrawCheckBox(rc.x2-smes-prop_h, (rc.y+9+prop_h*(2+maxVcount+HEADP))-prop_h*(i+1-smesh), cbc==attribute, IsAttrEnter(attribute));      
    }
  }

  //Софты
  wsprintf(guibuf, _t, muitxt(ind_yes));
  DrawString(guibuf,rc.x+smes,rc.y2-3-prop_h,rc.x2-smes,rc.y2-3,FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, _t, muitxt(ind_no));
  DrawString(guibuf,rc.x+smes,rc.y2-3-prop_h,rc.x2-smes,rc.y2-3,FONT_PROP,4,(char*)&Colors[clInfo],NULL);
}

////////////////////////////////////////////////////////////////////////////////
/*                            MD5 Print                                       */
////////////////////////////////////////////////////////////////////////////////

int md5_inp_onkey(GUI *gui, GUI_MSG *msg)
{
  if (msg->keys==26)
  {
    EDITCONTROL ec;
 
    WSHDR *wbuff1 = AllocWS(32);
    WSHDR *wbuff2 = AllocWS(32); 
    ExtractEditControl(gui, 2, &ec);
    wstrcpy(wbuff1,ec.pWS);
      
    ExtractEditControl(gui, 4, &ec);
    wstrcpy(wbuff2,ec.pWS);
     
    if(wbuff2->wsbody[0])
    {      
      if(wstrcmpi(wbuff1, wbuff2)==0)
        ShowMSG(1,(int)muitxt(ind_corect));
      else
        ShowMSG(1,(int)muitxt(ind_no_corect));
    }
    FreeWS(wbuff1);
    FreeWS(wbuff2);
    
    if(!wbuff2->wsbody[0])
      return (0xFF);
    
    return(-1);
  }

  if (msg->keys==0xFFF)
    return (0xFF);

  return(0); //Do standart keys
}

HEADER_DESC md5_inp_hdr={0,0,0,0,NULL,0,LGP_NULL};
INPUTDIA_DESC md5_inp_desc=
{
 1,
 md5_inp_onkey,
 empty_ghook,
 (void *)empty_locret,
 0,
 &empty_menu_skt,
 {0,0,0,0},
 FONT_SMALL,
 100,
 101,
 0,
 0,
 0x40000000
};

int MD5Print(MD5_CTX *mdContext)
{  
 md5_inp_hdr.lgp_id = (int)muitxt(ind_md5);
 void *ma=malloc_adr();
 void *eq=AllocEQueue(ma,mfree_adr());
 EDITCONTROL ec;
 PrepareEditControl(&ec);
 
 wsprintf(guibuf, _tc, muitxt(ind_md5_file));
 ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,guibuf,32);
 AddEditControlToEditQend(eq,&ec,ma);

 wsprintf(guibuf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
                ,mdContext->digest[0],mdContext->digest[1],mdContext->digest[2]
                ,mdContext->digest[3],mdContext->digest[4],mdContext->digest[5]
                ,mdContext->digest[6],mdContext->digest[7],mdContext->digest[8]
                ,mdContext->digest[9],mdContext->digest[10],mdContext->digest[11]
                ,mdContext->digest[12],mdContext->digest[13],mdContext->digest[14]
                ,mdContext->digest[15]);
 ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_SET_CURSOR_END,guibuf,MAX_PATH);
 AddEditControlToEditQend(eq,&ec,ma);
 
 wsprintf(guibuf, _tc, muitxt(ind_cmd5));
 ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,guibuf,32);
 AddEditControlToEditQend(eq,&ec,ma);
 
 wsprintf(guibuf, "");
 ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_SET_CURSOR_END,guibuf,MAX_PATH);
 AddEditControlToEditQend(eq,&ec,ma);
 
 return CreateInputTextDialog(&md5_inp_desc,&md5_inp_hdr,eq,1,0);
}

void GetMD5(char *file)
{
  MD5_CTX mdContext;
  MD5Init(&mdContext);
  
  #define BUFSIZE (0x4000)   
  unsigned long size_buf = BUFSIZE;
  void *buf = malloc(BUFSIZE);
   
  int fin = fopen(file, A_ReadOnly + A_BIN, P_READ, &err);
  unsigned long size_read = 0;

  do
  {
    size_read = fread(fin, buf, size_buf, &err);
    if (size_read>0) MD5Update(&mdContext, buf, size_read);
  }while(size_read>0);

  if (fin) fclose(fin, &err);
  MD5Final(&mdContext);
  
  MD5Print(&mdContext);
  
  mfree(buf);
}
