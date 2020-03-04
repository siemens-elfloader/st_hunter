#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <swilib.h>
#include "encode.h"
#include <zlib.h>
#include <unzip/unzip.h>
#include "main.h"




const int minus11=-11;
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0,i_count=1,readed_bytes=1,pixel=1,
		scr_w, scr_h,
		f_h; // высота шрифта
unsigned int t_count=0,tpixel=0,tbuflen=1,tsizelen=1,center=0;

WSHDR *message,*percents;
int winx,winy,winw,winh,
        allfiles=1;
int progbx,progby,progbw,progbh,progbmax;
int tprogbx,tprogby,tprogbw,tprogbh,tmaxpb=0;
int textmx=0,textmy=0,textmx2=0,textmy2=0;

char State=STATE_NOTHING;
const char
		black[]={0,0,0,100},
		white[]={0xff,0xff,0xff,100},
		white0[]={0xff,0xff,0xff,0},
		green[]={0x0,0xff,0x0,100},
		red[]={0xff,0x0,0x0,100},
		blue[]={0x0,0x0,0xff,100};

char *zipfile=0,*unpackdir=0;
unzFile uff=0;



void ElfKiller(void)
{
kill_elf();
}

#ifdef __LOG_FILE
char temp[500];

void logfile(char *str){
int f=0;
unsigned int terr=0;
if((f=_open("0:\\MiniZip_test.log",A_ReadWrite+A_Append+A_Create+A_BIN,P_WRITE+P_READ,&terr))!=-1){
    _write(f,str,strlen(str),&terr);
    _close(f,&terr);
    }
}
#endif

void RecalcGui(int x,int y,int x2,int y2){
winx= x;
winw= x2;
winy= y;
winh=y2;
progbx=winx+1;
progbmax=winw-progbx-1;
pixel=(i_count*progbmax)/allfiles;
progbw=progbx+pixel;
progby=winy+1;
progbh=progby+(f_h+3);
tprogbx=progbx;
tprogby=progbh+1;
tmaxpb=progbmax;
tpixel=(readed_bytes*tmaxpb)/tsizelen;
tprogbw=tprogbx+tpixel;
tprogbh=tprogby+5;      // прогрессбар в 5 пикселев толщиной



textmx=progbx;
textmy=tprogbh+1;
textmy2=textmy+f_h*2+1;
textmx2=winw-1;
if(winh<(textmy2+1))
winh=textmy2+1;

center=progby+2;
}

//из DOS в Windows
char* Decode_DOS_to_Win(char * str)
{
    unsigned char *cstr=(unsigned char*)str;//"unsigned" - чтоб избежать предупреждений
    // "сдвигаем" код символов
    for(; *cstr; cstr++)
    {
        if(*cstr>=128 && *cstr<=175)
            *cstr+=64;
        else if(*cstr>=224 && *cstr<=239)
            *cstr+=16;
        else if(*cstr==252)
            *cstr=185;
    }
    return str;
}


char *GetDirFromFile(char *file){
    if(!file)return 0;
char *retdir=0,*tempDir=0,*symbol=0;
int slash=0,lenfile=0;
lenfile=strlen(file);
tempDir=(char*)malloc(lenfile+1);
memcpy(tempDir,file,lenfile+1);
if((symbol=strrchr(tempDir,'\\'))!=0){
    *symbol='\0';                          //Вернуть без слеша,--так можно?
    slash=strlen(tempDir);
retdir=(char *)malloc(slash+1);
memcpy(retdir,tempDir,slash+1);
}
free(tempDir);
return retdir;
}

char *GetFileName(char *file){
    char *temp=0;
char *ffname=0,*ffwoext=0;
int lenname=0;

if((ffname=strrchr(file,'\\'))>0){
    ffname++;
}else ffname=file;

lenname=strlen(ffname);
temp=(char *)malloc(lenname+1);
memcpy(temp,ffname,lenname+1);

if((ffwoext=strrchr(temp,'.'))>0){
*ffwoext='\0';
}
return temp;
}


void mymkdir(char *fdir){
    int dirlen=strlen(fdir);
    unsigned int mkerr=0;
    char *dirtocreate=fdir;
    if(fdir[dirlen]=='\\'){
        dirtocreate=(char *)malloc(dirlen);
        strncpy(dirtocreate,fdir,dirlen-1);
        _mkdir(dirtocreate,&mkerr);
        free(dirtocreate);
    }else _mkdir(dirtocreate,&mkerr);
  #ifdef __LOG_FILE
    if(mkerr){
        sprintf(temp,"Error:%d CreateDir:%s\n",mkerr,dirtocreate);
        logfile(temp);
    }
    #endif
}




int do_extract_currentfile(uf,popt_extract_without_path,popt_overwrite,password)
    unzFile uf;
    const int popt_extract_without_path;
    int popt_overwrite;
    const char* password;
{
    char filename_inzip[256];
    char *file_path=0;
    int file_pathl=0;
    char* filename_withoutpath;
    char* p;
    int err=UNZ_OK;
    FILE *fout=NULL;
    void* buf;
    uInt size_buf;
    char *utf8name;
    int fnamelen=0;

	readed_bytes=0;

    unz_file_info64 file_info;
//    uLong ratio=0;
    err = unzGetCurrentFileInfo64(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);

    if (err!=UNZ_OK)
    {
        #ifdef __LOG_FILE
        sprintf(temp,"error %d with zipfile in unzGetCurrentFileInfo\n",err);
        logfile(temp);
        #endif
        return err;
    }

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
        #ifdef __LOG_FILE
        sprintf(temp,"Error allocating memory\n");
        logfile(temp);
        #endif
        return UNZ_INTERNALERROR;
    }
//===============

//===============
    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\')){
            *p='\\';
            filename_withoutpath = p+1;
        }
        p++;
    }

fnamelen=strlen(filename_inzip);
file_pathl=strlen(unpackdir)+fnamelen*2;
file_path=malloc(file_pathl+1);
utf8name=(char *)malloc(fnamelen*2+1);
//Здесь должно быть определение кодировки в имени файла в архиве и,если нужно,
//---------------------------
//Преобразование в UTF8
dos2utf8_2(filename_inzip,utf8name);


sprintf(file_path,"%s%s",unpackdir,utf8name);

    if ((*filename_withoutpath)=='\0')
    {
        #ifdef __LOG_FILE
            sprintf(temp,"creating directory: %s\n",file_path);
            logfile(temp);
            #endif
            wsprintf(message,"%t: %t","Создание папки",file_path);
            mymkdir(file_path);
    }else
    {
        const char* write_filename;
        int skip=0;

    wsprintf(message,"%t: %t","Распаковка",filename_withoutpath);

        write_filename = file_path;

        err = unzOpenCurrentFilePassword(uf,password);
        #ifdef __LOG_FILE
        if (err!=UNZ_OK)
        {
            sprintf(temp,"error %d with zipfile in unzOpenCurrentFilePassword\n",err);
            logfile(temp);
        }
        #endif
        if ((skip==0) && (err==UNZ_OK))
        {
            fout=fopen64(write_filename,"wb");

#ifdef __LOG_FILE
            if (fout==NULL)
            {
                sprintf(temp,"error opening %s\n",write_filename);
                logfile(temp);
            }
            #endif
        }

        if (fout!=NULL)
        {
            tsizelen=file_info.uncompressed_size;
            REDRAW();
            #ifdef __LOG_FILE
            sprintf(temp," extracting: %s\n",write_filename);
            logfile(temp);
            #endif
            do
            {
                err = unzReadCurrentFile(uf,buf,size_buf);
                if (err<0)
                {
                    #ifdef __LOG_FILE
                    sprintf(temp,"error %d with zipfile in unzReadCurrentFile\n",err);
                    logfile(temp);
                    #endif
                    break;
                }
                if (err>0){
                    readed_bytes+=err;
                tbuflen=err;
                tpixel=(readed_bytes*tmaxpb)/(tsizelen);
                tprogbw=tprogbx+tpixel;
                REDRAW();
                    if (fwrite(buf,err,1,fout)!=1)
                    {
                        #ifdef __LOG_FILE
                        sprintf(temp,"error in writing extracted file\n");
                        logfile(temp);
                        #endif
                        err=UNZ_ERRNO;
                        break;
                    }
                    }

            }while (err>0);
            if (fout)
                    fclose(fout);
        }

        if (err==UNZ_OK)
        {
            err = unzCloseCurrentFile (uf);
            #ifdef __LOG_FILE
            if (err!=UNZ_OK)
            {
                sprintf(temp,"error %d with zipfile in unzCloseCurrentFile\n",err);
                logfile(temp);
            }
            #endif
        }
        else
            unzCloseCurrentFile(uf); /* don't lose the error */
    }

REDRAW();
    free(utf8name);
    free(file_path);
    free(buf);
    return err;
}


int do_extract(uf,password)
    unzFile uf;
    const char* password;
{
    uLong i;
    unz_global_info64 gi;
    int err;
//    FILE* fout=NULL;

    err = unzGetGlobalInfo64(uf,&gi);
    #ifdef __LOG_FILE
    if (err!=UNZ_OK){
        sprintf(temp,"error %d with zipfile in unzGetGlobalInfo \n",err);
        logfile(temp);
    }
    #endif

State=STATE_PROCESS;
    allfiles=gi.number_entry;
    for (i=0;i<gi.number_entry&&(State==STATE_PROCESS);i++)
    {
        i_count=i;
        wsprintf(percents,"%d\\%d",i_count,allfiles);
        pixel=(i_count*progbmax)/allfiles;
        progbw=progbx+pixel;
        REDRAW();
        if (do_extract_currentfile(uf,0,1,password) != UNZ_OK)
            break;

        if ((i+1)<gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err!=UNZ_OK)
            {
                #ifdef __LOG_FILE
                sprintf(temp,"error %d with zipfile in unzGoToNextFile\n",err);
                logfile(temp);
                #endif
                break;
            }
        }
    }
    State=STATE_OK;
i_count++;
    wsprintf(percents,"%d\\%d",i_count,allfiles);
        pixel=(i_count*progbmax)/allfiles;
        progbw=progbx+pixel;
        wsprintf(message,"%t","Готово.");
        REDRAW();
    return 0;
}


void OnRedraw(MAIN_GUI *data)
{
    if (data->gui.state==2){
    DrawRectangle(winx,winy,winw,winh,0,white,black);
    DrawRectangle(progbx,progby,progbw,progbh,0,black,white);
    DrawRectangle(tprogbx,tprogby,tprogbw,tprogbh,0,black,white);

    DrawString(message,textmx,textmy,textmx2,textmy2,FONT_SMALL,TEXT_ALIGNLEFT,white,0);
    DrawString(percents,progbx,center,textmx2,center+f_h,FONT_SMALL,TEXT_ALIGNMIDDLE,black,white);

}
}


void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->ws1=AllocWS(256);
  message=AllocWS(256);
  percents=AllocWS(64);
  wsprintf(message,"%t","Подготовка...");
  	scr_w=ScreenW();
	scr_h=ScreenH();
	f_h=GetFontYSIZE(FONT_SMALL); // высота мелкого шрифта
RecalcGui(scr_w/16,(scr_h-YDISP)/3,scr_w-scr_w/16,0);
SUBPROC((void *)do_extract,uff,0);
  data->gui.state=1;
}

void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  FreeWS(message);
  FreeWS(percents);
  if(unpackdir)
    free(unpackdir);
  data->gui.state=0;
}

void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  REDRAW();
  DisableIDLETMR();
}

void OnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  RestartIDLETMR();
  data->gui.state=1;
}


int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI();
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
    case RIGHT_SOFT:{
//        ShowMSG(1,(int)temp);
if(State==STATE_OK)CloseCSM(MAINCSM_ID);
if(State==STATE_PROCESS)State=STATE_CANCEL;

break;
//      return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
    }
    }
  }
  return (0);

}

extern void kill_data(void *p, void (*f)(void*));

int method8(void){return(0);}

int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)OnRedraw,	//Redraw
  (void *)OnCreate,	//Create
  (void *)OnClose,	//Close
  (void *)OnFocus,	//Focus
  (void *)OnUnfocus,	//Unfocus
  (void *)OnKey,	//OnKey
  0,
  (void *)kill_data, //method7,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};


RECT Canvas={0,0,0,0};

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  Canvas.x2=ScreenW()-1;
  Canvas.y2=ScreenH()-1;
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;

  uff = unzOpen(zipfile);
if(!uff){
    CloseCSM(MAINCSM_ID);
}else csm->gui_id=CreateGUI(main_gui);
}

void maincsm_onclose(CSM_RAM *csm)
{
    if(uff)
    unzClose(uff);
    else{
        if(zipfile)
        ShowMSG(1,(int)"Невозможно открыть файл.");
        else ShowMSG(1,(int)"Передайте файл \nв качестве параметра.");
    }
    if(zipfile)
    free(zipfile);
  SUBPROC((void *)ElfKiller);
}


int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
  }
  return(1);
}



const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
    maincsm_onmessage,
    maincsm_oncreate,
#ifdef NEWSGOLD
0,
0,
0,
0,
#endif
maincsm_onclose,
sizeof(MAIN_CSM),
1,
&minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};


void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"UnZipper 0.9");
}


int main(char *exe,char *filename)
{
    char *thisdir=0,*dirname=0;
    int thisdirl=0,dirnamel=0;
    if(filename){
    zipfile=malloc(strlen(filename)+1);
    memcpy(zipfile,filename,strlen(filename)+1);
    thisdir=GetDirFromFile(filename);
    if(thisdir){
    dirname=GetFileName(filename);
    dirnamel=strlen(dirname);
    thisdirl=strlen(thisdir);
    unpackdir=malloc(thisdirl+dirnamel+3);
    sprintf(unpackdir,"%s%c%s%c",thisdir,'\\',dirname,'\\');
    mymkdir(unpackdir);
    free(dirname);
    free(thisdir);
    }else return 1;
    }
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  LockSched();
  MAINCSM_ID=CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched();
}



