#include <swilib.h>
#include <alib/img.h>
#include <alib/sysdraw.h>
#include "krnl.h"


#define MAX_EXT			8

#define TMR_SECOND 216

#define CFG_NAVIG_ZOOM 0

extern char CFG_BORDER_COLOR[4];
extern char CFG_BG_COLOR[4];
extern unsigned int CFG_SLIDE_TIMEOUT;
extern unsigned int CFG_MOVE_STEP;
extern unsigned int CFG_SAVE_FORMAT;
extern unsigned int CFG_SMART_ROTATE;
extern unsigned int CFG_MAX_RESOLUTION;

int zoomMode=0;
bool showInfo=1;
int zoomPer=0;
int maxSize=0;
int smesh_y=0;
int smesh_x=0;

int zoom=1;
int zoomK=2;

int curPos=0;

int IsOriginal=0;

char *mas[MAX_PIC];
int ind=0;

GBSTMR mytmr;

int GetImgCount (){ return ind; }
char *GetImgName (int id){ return mas[id]; }
char *GetCurImgName (){ return mas[curPos]; }

WSHDR *ws;


char *GetFileExt(char* fname){
  char *s1, *s2;
  s1=strrchr(fname,'.');
  if (s1)
  {
    int i=0;
    s2=++s1;
    while(*s2++) i++;
    if (i>MAX_EXT) s1=0;
  }
  return (s1);
}
char *GetFileName(char* fname)
{
	int len = strlen(fname);
	int ii;
	for(ii = len-1; ii >= 0; ii--) {
		if (fname[ii] == '\\' || fname[ii] == '/') break;
	}
	if (ii>=0) return fname+ii+1;
	return fname;
}

char *GetFileDir(char* fname, char* buf)
{
	int ii;
	int len = strlen(fname);

	for(ii = len-2; ii > 0; ii--)
		if (fname[ii] == '\\' || fname[ii] == '/') break;
	len = ii;

	if (buf)
	{
		for(ii=0; ii<len; ii++)
			buf[ii] = fname[ii];
		buf[len] = 0;
	}
	return buf;
}
#define NUM_SUPPORT_FORMATS 3
int IsPicture (char* file){

    char *picture[NUM_SUPPORT_FORMATS]={"jpg","jpeg","png"};

    char* sz = GetFileExt(file);

    for (int i=0; i<NUM_SUPPORT_FORMATS; i++)
        if (!strcmpi(sz, picture[i])) return 1;

    return 0;
}

void SearchIMG (char *GlobalFolder){
  unsigned int err;
  DIR_ENTRY de;

  char name[256];
  //char file[256];

  strcpy(name, GlobalFolder);
  strcat(name, "\\*.*");

  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      if (IsPicture(de.file_name))
      {
         mas[ind]=new char [128];
         strcpy(mas[ind], de.file_name); ind++;
      }

      }

    while(FindNextFile(&de,&err));
#ifdef NEWSGOLD
    FindClose(&de, &err);
#endif
   }
#ifndef NEWSGOLD
   FindClose(&de, &err);
#endif
}

void FreeMas (){
    for (int i=0; i<ind; i++) delete mas[i];
}

void Sort(){
  char name[128];

  for (int i=0; i<ind-1; i++)
  {
    for (int j=i+1; j<ind; j++)
    {
      if (strcmp(mas[i], mas[j])>0)
      {
        strcpy(name, mas[i]);
        strcpy(mas[i], mas[j]);
        strcpy(mas[j], name);
      }
    }
  }
}


int FileExists (char *fname){
  int fl=0;
  unsigned int err;
  fl = _open(fname, A_ReadOnly, P_READ, &err);
  if (fl!=-1) _close(fl, &err);
  return fl;
}


void DoConfig (){
    WSHDR *ws=AllocWS (128);
    extern const char *successed_config_filename;
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws, NULL, NULL);
    FreeWS(ws);
}

///////////////////////////
int screenW=0;
int screenH=0;

static const char per[]="\\";
Image oImage;
Image zImage;
char pathbuf[256];
char dir[256];

#define png_ext "png"
#define jpg_ext "jpg"
#define jpeg_ext "jpeg"

#include <stdlib.h>


#include <stm.h>

//в тиках 1 с = 1300/6 = 216,6 тиков
int tick2ms (int tick){ return tick*60/13; }
void Log (char *txt)
{
  unsigned int ul;
  int f=_open("0:\\test.txt",A_ReadWrite+A_Create+A_Append+A_BIN,P_READ+P_WRITE,&ul);
  if (f!=-1)
  {
    _write(f,txt,strlen(txt),&ul);
    _close(f,&ul);
  }
}

int is_err=0;
void SmartScale (int k){
    //50, 100, 150...
    IsOriginal=0;

    if (is_err==1) is_err=0;

    int w=oImage.GetW();
    int h=oImage.GetH();

    int n_w=min (w, h);
    int n_h=max (w, h);

    int fw=n_w/screenW;
    int fh=n_h/screenH;
    int r_k= min (fw, fh);
    if (r_k<1) r_k=1;

    int transform=0;
    if (CFG_SMART_ROTATE){
        if (w>h && w>screenW && h>screenH){
            transform=ROT90;
        }
    }

    int err=oImage.ResizeWithTransform (&zImage, k, r_k*100, transform);
    if (err==1) IsOriginal=1;
    else if (err){ is_err=1; return;}

/*
    if (CFG_SMART_ROTATE){
        if (w>h && w>screenW && h>screenH){
            int err=zImage.Rotate (90);
            if (err){ is_err=1; return;}
            //zImage.Rotate (270);
        }
    }
    */

}

void PrepareToNext (){
    is_err=0;

    UpdateCSMname();

    strcpy(pathbuf, dir);
    strcat(pathbuf, per);
    strcat(pathbuf, mas[curPos]);
    oImage.FreeBitmap ();
    zImage.FreeBitmap ();
    char *ext=GetFileExt(mas[curPos]);
    if (!strcmpi(png_ext, ext)){
        if (oImage.CreateFromPNG (pathbuf)) is_err=2;
    }
    else{
        if (!strcmpi(jpg_ext, ext) || !strcmpi(jpeg_ext, ext) ){
            if (oImage.CreateFromJPEG (pathbuf)) is_err=2;
        }
        else is_err=2;
    }

    IsOriginal=0;

    zoom=100;
    if (!is_err) SmartScale (zoom);

    //DrawWindow ();
}


void EnterButton(){
  if (zoomMode){

    if (!maxSize) zImage.FreeBitmap ();

    zoomMode=0;
    smesh_y=0;
    smesh_x=0;
    zoom=100;
    maxSize=0;

    SmartScale (zoom);
  }
  else zoomMode=1;
}

void ImgAViewOnCreate (){
    ws=AllocWS (128);
}

void ImgAViewOnClose (){
    GBS_DelTimer(&mytmr);
    oImage.FreeBitmap ();
    zImage.FreeBitmap ();
    FreeMas ();

    FreeWS (ws);
}

int x_centered=0;
int y_centered=0;
char clrWhite[]= {0xFF,0xFF,0xFF,0x64};

void DrawWindow (){
    //DrawRoundedFrame
    DrawRectangle (0, 0, screenW, screenH, 0, CFG_BG_COLOR, CFG_BG_COLOR);

    if (is_err==1){
        wsprintf(ws, "%s" , "ERROR: Out of memory.\nPress Enter...\nor back to previous zoom level.");
        DrawString(ws,  0,  screenH/2, screenW, screenH, FONT_SMALL_BOLD,TEXT_ALIGNMIDDLE, clrWhite,0);
        return;
    }else if (is_err==2){
        wsprintf(ws, "%s" , "ERROR: Load image");
        DrawString(ws,  0,  screenH/2, screenW, screenH, FONT_SMALL_BOLD,TEXT_ALIGNMIDDLE, clrWhite,0);
        return;
    }

    int w=oImage.GetW();
    int h=oImage.GetH();

    if (maxSize==1){
        x_centered=(screenW-w)/2;
        y_centered=(screenH-h)/2;
        DrawImage_LCDLayer (&oImage, x_centered+smesh_x, y_centered+smesh_y);
    }
    else{
        if (IsOriginal!=1){
            x_centered=(screenW-zImage.GetW())/2;
            y_centered=(screenH-zImage.GetH())/2;
            DrawImage_LCDLayer (&zImage, x_centered+smesh_x, y_centered+smesh_y);
        }
        else{
            //if (w<screenW || h<screenH) DrawRoundedFrame(0, 0, screenW, screenH, 0,0,0,clrGray, clrGray);
            x_centered=(screenW-w)/2;
            y_centered=(screenH-h)/2;
            DrawImage_LCDLayer (&oImage, x_centered+smesh_x, y_centered+smesh_y);

            if (w<screenW && h<screenH){
                char bdclr[4];
                bdclr[0]=CFG_BG_COLOR[0]/2;
                bdclr[1]=CFG_BG_COLOR[1]/2;
                bdclr[2]=CFG_BG_COLOR[2]/2;
                DrawRectangle (x_centered+smesh_x, y_centered+smesh_y, x_centered+smesh_x+w, y_centered+smesh_y+h,0,bdclr,0);
            }
        }
    }

    if (showInfo){
        int FontSize=GetFontYSIZE(FONT_SMALL_BOLD);

        DrawRectangle (0, 0, screenW-1, FontSize, 0,CFG_BORDER_COLOR, CFG_BORDER_COLOR);
        DrawRectangle (0, screenH-FontSize-1, screenW-1, screenH-1, 0,CFG_BORDER_COLOR, CFG_BORDER_COLOR);

        str_2ws (ws, mas[curPos], strlen(mas[curPos]));

#ifdef ELKA
        DrawString(ws,  0,  0, screenW, FontSize, FONT_SMALL_BOLD, TEXT_ALIGNMIDDLE, clrWhite,0);
#else
        DrawString(ws,  0,  0, screenW, FontSize, FONT_SMALL_BOLD, TEXT_ALIGNLEFT, clrWhite,0);
#endif

        wsprintf(ws, "%d/%d", curPos+1, ind);
        DrawString(ws,  screenW/2,  0, screenW-2, FontSize, FONT_SMALL_BOLD, TEXT_ALIGNRIGHT, clrWhite,0);

        if (zoomMode){
            if (!maxSize){
                if (IsOriginal!=1){
                    zoomPer=100*zImage.GetW()*zImage.GetH()/(w*h);
                    wsprintf(ws, "zoom: %d%\n%dx%d[%dx%d]" , zoomPer, zImage.GetW(), zImage.GetH(), w, h);
                }
                else{
                    zoomPer=100;
                    wsprintf(ws, "zoom: %d%\n%dx%d" , zoomPer, w, h);
                }
            }
            else{
                zoomPer=100;
                wsprintf(ws, "zoom: %d%\n%dx%d" , zoomPer, w, h);
            }

            DrawString(ws,  0,  screenH-2*FontSize, screenW, screenH, FONT_SMALL_BOLD,TEXT_ALIGNMIDDLE, clrWhite,0);
        }
        else{
            /*if (slideActive){
            str_2ws (ws, "SlideShow", 64);
            DrawString(ws,  0,  FontSize, screenW, 2*FontSize, FONT_SMALL_BOLD, TEXT_ALIGNMIDDLE, clrWhite,0);
            }*/
            if (w>screenW && h>screenH){
                wsprintf(ws, "%dx%d" ,zImage.GetW(), zImage.GetH());
                DrawString(ws,  0,  screenH-FontSize, screenW, screenH, FONT_SMALL_BOLD,TEXT_ALIGNMIDDLE, clrWhite,0);
            }
            else{
                wsprintf(ws, "%dx%d" , w, h);
                DrawString(ws,  0,  screenH-FontSize, screenW, screenH, FONT_SMALL_BOLD,TEXT_ALIGNMIDDLE, clrWhite,0);
            }

        }
    }

}



int GetCurPos (){ return curPos; }
void SetCurPos (int id){ curPos=id; }

void _DownKey(){
   if (curPos<GetImgCount()-1) curPos++;
       else curPos=0;

   PrepareToNext();
}

void _UpKey(){
  if (curPos>0) curPos--;
       else curPos=GetImgCount()-1;

  PrepareToNext();
}

int slideActive=0;

__inl
void TempLightOn (int x, int y)
__defn( 0x044, x, y)

void SlideShow(){
    if (slideActive){
        _DownKey();
        DrawWindow();

        TempLightOn (3, 0x7FFF);  //чтобы подсветка не гасла
        GBS_StartTimerProc (&mytmr, CFG_SLIDE_TIMEOUT*TMR_SECOND/10, (void(*)(GBSTMR*))&SlideShow);
    }
}



void SaveImg (){
    strcpy(pathbuf, dir);
    strcat(pathbuf, per);
    strcat(pathbuf, mas[curPos]);

    switch (CFG_SAVE_FORMAT){
        case 1:
        strcat(pathbuf, "_new.png");
        if (maxSize) oImage.SaveInPNG (pathbuf);
        else{
            if (IsOriginal!=1) zImage.SaveInPNG (pathbuf);
            else oImage.SaveInPNG (pathbuf);
        }
        break;

        case 0:
        strcat(pathbuf, "_new.jpg");
        if (maxSize) oImage.SaveInJPEG (pathbuf, 100);
        else{
            if (IsOriginal!=1) zImage.SaveInJPEG (pathbuf, 100);
            else oImage.SaveInJPEG (pathbuf, 100);
        }
        break;
    }
}

int ImgAViewOnKey (int mess, int key){
    if (mess==KEY_DOWN || mess==LONG_PRESS){

    if (zoomMode){

    switch(key)
    {
        case UP_BUTTON:
        if (CFG_NAVIG_ZOOM) smesh_y-=CFG_MOVE_STEP;
        else smesh_y+=CFG_MOVE_STEP;
        break;

        case LEFT_BUTTON:
        if (CFG_NAVIG_ZOOM) smesh_x-=CFG_MOVE_STEP;
        else smesh_x+=CFG_MOVE_STEP;
        break;

        case DOWN_BUTTON:
        if (!CFG_NAVIG_ZOOM) smesh_y-=CFG_MOVE_STEP;
        else smesh_y+=CFG_MOVE_STEP;
        break;

        case RIGHT_BUTTON:
        if (!CFG_NAVIG_ZOOM) smesh_x-=CFG_MOVE_STEP;
        else smesh_x+=CFG_MOVE_STEP;
        break;

        case LEFT_SOFT:
        //original=0;
        zoom-=50;

        if (zoom>0){
            zImage.FreeBitmap ();
            SmartScale (zoom);
        }else zoom+=50;
        break;

        case RIGHT_SOFT:
        if (!maxSize){
            //original=0;
            zoom+=50;

            zImage.FreeBitmap ();
            SmartScale (zoom);

          }else EnterButton();
        break;

            }
        }
        else{
            switch(key)
            {
                case UP_BUTTON:
                _UpKey ();
                break;

                case DOWN_BUTTON:
                _DownKey ();
                break;

                case '5' :
                if (!slideActive){
                    slideActive=1;
                    SlideShow();
                }
                else slideActive=0;
                break;

                case RIGHT_SOFT:
                return (1);
                break;

            }
        }

        switch(key){
            case '9':
            if (maxSize) oImage.Rotate (90);
            else{
                if (IsOriginal!=1) zImage.Rotate (90);
                else oImage.Rotate (90);
            }
            break;

            case '3':
            if (maxSize) oImage.FlipVertic ();
            else{
                if (IsOriginal!=1) zImage.FlipVertic ();
                else oImage.FlipVertic ();
            }
            break;

            case '6':
            if (maxSize) oImage.FlipHoriz ();
            else{
                if (IsOriginal!=1) zImage.FlipHoriz ();
                else oImage.FlipHoriz ();
            }
            break;

            case '7':
            SaveImg ();
            break;

            case '#' :
            DoConfig ();
            break;

            case '0':
            showInfo=!showInfo;
            break;

            case ENTER_BUTTON:
            if (slideActive) slideActive=0;
            else EnterButton();
            break;

            case '*' :
            if (!maxSize){
                maxSize=1; zoomMode=1;
                IsOriginal=1;
                zImage.FreeBitmap ();
            }
            else{
                IsOriginal=0;
                EnterButton ();
            }
            break;
        }

        DrawWindow ();
    }

    return 0;

}

void ShowFirstImg(char *fname){
  short fl=0;
  int count=GetImgCount ();
  for (int i=0; i<count && fl!=1; i++){
    if (!strcmp(fname, GetImgName(i))){ fl=1;  SetCurPos (i); }
  }
  if (fl) PrepareToNext();
}

void Init (char *fname){
    screenW=ScreenW();
    screenH=ScreenH();

    char *firstName=GetFileName(fname);

    GetFileDir(fname, dir);

    SearchIMG (dir);

    ShowFirstImg(firstName);


}


//////////////////////////////////
