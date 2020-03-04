#include <alib/img.h>
#include <alib/io.h>
#include <alib/io_ui.h>
#include <alib/ui.h>
#include <alib/sysdraw.h>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "mymenu.h"

#include "radio.h"

extern unsigned int CFG_FONT_SIZE;
extern unsigned int  CFG_SNAME_X;
extern unsigned int  CFG_SNAME_Y;

extern unsigned int  CFG_FREQ_X;
extern unsigned int  CFG_FREQ_Y;

extern unsigned int  CFG_VOL_X;
extern unsigned int  CFG_VOL_Y;

extern unsigned int  CFG_LEVEL_X;
extern unsigned int  CFG_LEVEL_Y;

extern unsigned int  CFG_STATUS_X;
extern unsigned int  CFG_STATUS_Y;

extern unsigned int  CFG_ICON_X;
extern unsigned int  CFG_ICON_Y;

extern unsigned int  CFG_PLAY_ICON_X;
extern unsigned int  CFG_PLAY_ICON_Y;

extern const unsigned int CFG_DISABLE_ICONBAR;

int add_size=0;

bool IsWinStarted=0;
bool IsFontCfgUpdate=1;
Image *mybuffer;
//Image img;
//TFont font;
ft_font *ftf;

ft_font *GetFontFT (){ return ftf;}

const char *GetCurFont (){
    return "0:\\Zbin\\fonts\\Ubuntu-R.ttf";
}

char ibuf[128];
void MSG (int i){
    sprintf (ibuf, "%d", i);
    ShowMSG (1, (int)ibuf);
}
#define TEST_FONT

bool IsFocus=0;

UI myui;
UI mainui;

RadioMenu menu;
GBSTMR tmr;
int delay=10;
//ConfigInfo themeConfig;

#define MODE_LANDSCAPE

#define RADIO_LIST          "radio.txt"
#define BG_IMG_NAME          "bg.png"
#define PLAY_IMG_NAME        "play.png"
#define PAUSE_IMG_NAME       "pause.png"

Image bg;
Image play_img;
Image pause_img;

void UI_OnCreate (){

    int scrH=ScreenH ();
    int scrW=ScreenW ();

#ifdef TEST_FONT
    ftf = ft_open(GetCurFont (), CFG_FONT_SIZE);
#endif
    char tmp[256];

    sprintf (tmp, "%s\\%s", GetCurDir(), RADIO_LIST);
    //ShowMSG (1, (int)tmp);
    //if (CreateList (tmp)) return;

    CreateList (tmp);

    sprintf (tmp, "%s\\img\\%s", GetCurDir(), BG_IMG_NAME);
    bg.CreateFromPNG (tmp);

    sprintf (tmp, "%s\\img\\%s", GetCurDir(), PLAY_IMG_NAME);
    play_img.CreateFromPNG (tmp);

    sprintf (tmp, "%s\\img\\%s", GetCurDir(), PAUSE_IMG_NAME);
    pause_img.CreateFromPNG (tmp);

    myui.SetHeader ("Радиостанции");
    myui.SetSofts ("Выбор", "Назад");

    mainui.SetHeader ("Радио");
    mainui.SetSofts ("Список", "Выход");

    RadioItem item;
    for (int i=0; i<GetStationCount (); i++){
        menu.IncItemCount ();
        //if (i==5) sprintf (tmp, "Item_with_long_name_font_ubuntu%d", i);
        //else sprintf (tmp, "Item%d", i);

        RStation rs=GetStation (i);
        item.SetFreq (rs.freq);
        item.SetName (rs.name);

        menu.List.push_back (item);
    }

    mybuffer=GetImgBuffer();
    menu.SetMenuRect (0, get_color_from_set(PANEL_UP_SIZE)+CFG_HEADER_Y+(get_color_from_set(PANEL_HEADER_SIZE)-GetFontH (ftf))/2+GetFontH (ftf)+5, ScreenW(),  mybuffer->GetH()-get_color_from_set(PANEL_DOWN_SIZE));
    menu.SetCoordinates (10, 4, 0, 0, 0, 0);//10,4

    menu.SetAddKeyHook (OnEnterList);

    mainui.OnFocus ();

    RunRadio ();

    AddKeybMsgHook((void *)SmartHook);

    IsWinStarted=1;

}

void UI_OnRedraw (){
    bool need_redraw=0;
    if (IsFontCfgUpdate){
#ifdef TEST_FONT
        ftf = ft_open(GetCurFont (), CFG_FONT_SIZE+add_size);
        menu.ReInitShow ();
        IsFontCfgUpdate=0;

#endif
    }

    int FontH=GetFontH (ftf);

    if (myui.IsFocus ()){
        if (!menu.LongPress_ && !menu.ScroollActive_){
            DrawBG (mybuffer);
            DrawUpPanel (mybuffer);
            DrawDownPanel(mybuffer);
            DrawHeaderPanel (mybuffer);
            myui.DrawHeader(mybuffer, ftf);
            myui.DrawSofts (mybuffer, ftf);

            need_redraw=menu.DrawMenuList (&myui, mybuffer, ftf);

            DrawImage (mybuffer, 0, 0);
        }
        else{
            MyRECT rc=menu.MenuCoord;
            rc.y=get_color_from_set(PANEL_UP_SIZE)+CFG_HEADER_Y;

            DrawRectBG (mybuffer, rc);
            DrawHeaderPanel (mybuffer);
            myui.DrawHeader(mybuffer, ftf);

            need_redraw=menu.DrawMenuList (&myui, mybuffer, ftf);

            DrawImageRect (mybuffer, rc.x, rc.y, &rc);
        }

        if (need_redraw){
            if (myui.IsFocus())
            GBS_StartTimerProc(&tmr, 216/delay, (void(*)(GBSTMR*))&UI_OnRedraw);
        }
    }

    if (mainui.IsFocus ()){
        DrawBG (mybuffer);
        DrawUpPanel (mybuffer);
        DrawDownPanel(mybuffer);
        DrawHeaderPanel (mybuffer);
        mainui.DrawHeader(mybuffer, ftf);
        mainui.DrawSofts (mybuffer, ftf);

        mybuffer->DrawLayer (&bg, (mybuffer->GetW()-bg.GetW())/2, CFG_ICON_Y);

        if (!IsStop ()){
            mybuffer->DrawLayer (&play_img, CFG_PLAY_ICON_X, CFG_PLAY_ICON_Y);
        }
        else mybuffer->DrawLayer (&pause_img, CFG_PLAY_ICON_X, CFG_PLAY_ICON_Y);

        char text_buf[128];
        freq2str(text_buf, GetCurFreq ());
        DrawScrollString_UTF8(mybuffer, text_buf, ftf, 0, CFG_FREQ_Y, mybuffer->GetW(), mybuffer->GetH(), 0, TEXT_ALIGNMIDDLE, get_color_from_set(CLR_FONT_MENU));

        int id=SearchCurStation ();
        if (id!=-1){
            RStation rs=GetStation (id);
            DrawScrollString_ASCII(mybuffer, rs.name, ftf, 0, CFG_SNAME_Y, mybuffer->GetW(), mybuffer->GetH(), 0, TEXT_ALIGNMIDDLE, get_color_from_set(CLR_FONT_MENU));
        }

        UpdateInfo();

        if (GetStatus ()) sprintf (text_buf, "stereo");
        else sprintf (text_buf, "mono");
        DrawScrollString_UTF8(mybuffer, text_buf, ftf, 0, CFG_STATUS_Y, mybuffer->GetW(), mybuffer->GetH(), 0, TEXT_ALIGNMIDDLE, get_color_from_set(CLR_FONT_MENU));

        int level_per=100*GetLevel ()/MAX_LEVEL;
        sprintf (text_buf, "%d%%", level_per);
        DrawHeaderText (mybuffer, ftf, text_buf, TEXT_ALIGNRIGHT);

        int volume_per=100*GetVolume ()/MAX_VOLUME;
        sprintf (text_buf, "Volume: %d%%", volume_per);
        DrawScrollString_UTF8(mybuffer, text_buf, ftf, 0, CFG_VOL_Y, mybuffer->GetW(), mybuffer->GetH(), 0, TEXT_ALIGNMIDDLE, get_color_from_set(CLR_FONT_MENU));

        if (!CFG_DISABLE_ICONBAR){
            MyRECT rc={0, YDISP, mybuffer->GetW(), mybuffer->GetH()};
            DrawImageRect (mybuffer, 0, YDISP, &rc);
        }
        else{
            DrawImage (mybuffer, 0, 0);
        }
    }
}

void UI_OnFocus (){
    IsFocus=1;
}

void UI_UnFocus (){
    IsFocus=0;
}

void DrawWindow (){
    if (IsFocus){
        UI_OnRedraw ();
    }
}

void change_size (int val){
    add_size+=val;
    IsFontCfgUpdate=1;
}

int OnEnterList (int mess, int key, int id){
    if (mess==KEY_DOWN || mess==LONG_PRESS){

        switch(key){
            case ENTER_BUTTON:
            RStation rs=GetStation (id);
            SetFreq (rs.freq);
            myui.UnFocus ();
            mainui.OnFocus ();
            break;
        }
    }

}

int MyUIOnKey (int mess, int key){
    if (mess==KEY_DOWN || mess==LONG_PRESS){

        switch(key){

            case '*' :
            DoConfig ();
            break;

            case '#':
            OnKeyPress (mess, key, 1, '#', KeyLock);
            break;

            case RIGHT_SOFT:
            if (mainui.IsFocus ()){
                return (1);
            }else{
                myui.UnFocus ();
                mainui.OnFocus ();
            }
            break;

            /*
            case VOL_UP_BUTTON:
            //change_size (1);
            break;

            case VOL_DOWN_BUTTON:
            //change_size (-1);
            break;
            */
            case LEFT_SOFT:
            case GREEN_BUTTON:
            if (mainui.IsFocus ()){
                mainui.UnFocus ();
                myui.OnFocus ();
            }else{
                myui.UnFocus ();
                mainui.OnFocus ();
            }
            break;
        }

        if (mainui.IsFocus ()){
            switch(key){
                case LEFT_BUTTON: FreqDec ();
                break;

                case RIGHT_BUTTON: FreqInc ();
                break;

                case UP_BUTTON: PrevStation ();
                break;

                case DOWN_BUTTON: NextStation ();
                break;

                case ENTER_BUTTON: SetStop ();
                break;
            }
        }

        if (myui.IsFocus ()){ menu.MenuOnKey (mess, key);}

        DrawWindow ();
    }

    return 0;

}

void CloseTimer (){
  GBS_DelTimer (&tmr);
}


void UI_OnClose (){
    CloseTimer ();

    RemoveKeybMsgHook((void *)SmartHook);
    RadioStop ();

}


void OnCloseWin (){
#ifdef TEST_FONT
    ft_close(ftf);
#endif

    UI_OnClose ();
}




/////////////////////
#define MAX_STATION 128

int RIndex=0;
RStation RList[MAX_STATION];

RStation GetStation (int id){
    return RList[id];
}

int GetStationCount (){ return RIndex;}

int CreateList (char *fname){
    char tmp[64];
    SimpleParser file;
    file.Open ((char*)fname);

    if (file.GetCount ()>0){
        while (file.GetCur ()<file.GetCount ()){
            if (file.GoToSymb ('<')==-1) return 1;
            if (file.CopyToSymb (tmp, '>')==-1) return -1;
            RList[RIndex].freq=str2int (tmp);

            if (file.GoToSymb ('"')==-1) return -1;
            if (file.CopyToSymb ((char*)RList[RIndex].name, '"')==-1) return -1;

            RIndex++;

            if (RIndex>=MAX_STATION) break;
        }
        file.Close ();
    }

    return 0;
}

HObj rObj;
int volume=1;
int status=0;
int level=0;
bool mute=0;
bool stop=0;

int curRInd=0;

void Callback (){ return;}

int GetLevel (){ return level;}
int GetStatus (){ return status;}
int GetVolume (){ return volume;}
bool IsStop (){ return stop;}

void freq2str (char *str, int f){
  int freq=f/1000;
  int freq2=(f/100-freq*10);

  sprintf (str, "%d.%d", freq, freq2);
}

void StatusCB (int *arg1, int *arg){
  status=*arg;
}

void LevelCB (int *arg1, int *arg){
  level=*arg;
  fmdl_get_stereo_status((int)StatusCB);
}

void UpdateInfo (){
    fmdl_get_current_level((int)LevelCB);
}

int SearchStation (int freq){
   for (int i=0; i<RIndex; i++)
     if (freq==RList[i].freq) return i;

   return -1;
}

int SearchCurStation (){
   return SearchStation (GetCurFreq());
}

void KillObj(){
  if (rObj){
    Obs_DestroyObject(rObj);
    //rObj=NULL;
  }
}

void RadioStop (){
    //focus=0;
    fmdl_set_power (0);
    Obs_Stop (rObj);
    KillObj();
}
int curFreq=87500;
int mas[16]={0xADC3, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int GetCurFreq (){
    return curFreq;
}

void SetCurFreq (){
  fmdl_set_freq (curFreq, mas, (int)Callback);
}

void SetFreq (int freq){
  curFreq=freq;
  SetCurFreq ();
}

void FreqDec(){
  curFreq=curFreq-100;
  SetCurFreq ();
}

void FreqInc(){
  curFreq=curFreq+100;
  SetCurFreq ();
}

void RunRadio (){
  unsigned int err;

  //focus=1;

  fmdl_set_power (1);

  SetCurFreq ();

  KillObj ();

  rObj=Obs_CreateObject (0x4B, 0x34, 2, 0xB034, 1, 0, &err);

  Obs_Sound_SetLogSourceEx (rObj, 3);

  Obs_Sound_SetVolumeEx (rObj, volume, 1);

  Obs_Snd_SetPurpose (rObj, 0x21);

  Obs_Mam_SetPurpose (rObj, 0x21);

  Obs_Sound_SetHandsFree (rObj, 1);

  Obs_Prepare (rObj);

  Obs_Start (rObj);
}

void _VolUp(){
  if (!mute){
    if (volume<MAX_VOLUME) volume++;
    Obs_Sound_SetVolumeEx(rObj,volume,1);
  }
}

void _VolDown(){
  if (!mute){
    if (volume>0) volume--;
    Obs_Sound_SetVolumeEx(rObj,volume,1);
  }
}

void KeyLock (){
    SwitchToIdle();
    KbdLock();
}

int SmartHook (int key, int mess){
  OnKeyPress2 (mess, key, PTT_BUTTON, SetStop, Callback);

  OnKeyPress (mess, key, 0, VOL_UP_BUTTON, _VolUp);
  OnKeyPress (mess, key, 0, VOL_DOWN_BUTTON, _VolDown);

  return 0;
}

void SetStop (){
  if (!stop){ stop=1; RadioStop ();}
  else{ stop=0; RunRadio ();}
}

void NextStation (){
    int curRInd=SearchCurStation ();
    if (curRInd!=-1){
        if (curRInd<RIndex-1) curRInd++;
        SetFreq (RList[curRInd].freq);
    }
}

void PrevStation (){
    int curRInd=SearchCurStation ();
    if (curRInd!=-1){
        if (curRInd>0) curRInd--;
        SetFreq (RList[curRInd].freq);
    }
}
