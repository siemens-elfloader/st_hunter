#include <stdlib.h>
#include <stdio.h>
#include <swilib.h>
#include "config_loader.h"
/*
                                  SAVE FLASH
							(c)Tonich (c)Titron
эльф сохраняет указанную часть флеша в файл с названием:
Axxxxxxx-yyyyy.fbk //
где "xxxxxxx" -адрес откуда сливать,
"yyyyy"-размер сколько сливать(в HEX!!!)
.fbk-расширение файла :D


Я например им оперативу сливаю...т.к на много быстрей всякой иной штуки...
*/

 // параметры "State"
#define PROCESS		0 // в процессе
#define RES_OK		1 // удачно закончено
#define FAIL_WRITE	2 // ошибка записи
#define FAIL_SPASE	3 // недостаточно места на диске
#define FAIL_BUFSZ	4 // ошибка в размере буфера записи
#define FAIL_CHIP       5 //ошибка чтения (не использовал,т.к в любом случае будет пик)
#define FAIL_OPEN       6 //Ошибка открытия файла
#define FAIL_NAME       7 //Ошибка в названии файла

WSHDR *ws;
unsigned short maincsm_name_body[140];
char mes[32]="Хренли ты пялишься?";
extern const RECT POS;
extern const int BUFER;
char stopflag=0;
		//InitConfig(); //Конфиг тож надо объявить!
const int minus11=-11;
unsigned int err=0;
unsigned int MAINCSM_ID = 0,MAINGUI_ID=0,i,pixel,
		scr_w, scr_h,
		f_h; // высота шрифта
extern const char fon[], pb[];
char State;
const char perc_t[]="%t",
		black[]={0,0,0,100},
		white[]={0xff,0xff,0xff,100},
		white0[]={0xff,0xff,0xff,50};

struct{
	char *fl; // записываемый файл
	unsigned int from, to,
		sz, // размер
		wrsz, // записанная часть
		bsz; // размер буфера
}SF;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
}MAIN_GUI;

void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}


void ElfKiller(void)
{
kill_elf();
}


//===============================================================================================
int getval(char *s)
{
	int val=0;
	--s;
	while(*++s && *s!='-')
	{
     if(*s>='0' && *s<='9') val=(val<<4)+*s-'0';
     else
	  if((*s|=0x20)>='a' && *s<='f') val=(val<<4)+*s-'a'+10;
      else break;
	}
	return val;
}

void fwrite32(int fh, void *buf, int len, unsigned int *err)
{
  int clen, rlen;
  while(len&&!stopflag)
  {
	if (len>SF.bsz) clen=SF.bsz; else clen=len;
	SF.wrsz+=(rlen=_write(fh, buf, clen, err));
	if (rlen!=clen){ State=FAIL_WRITE; break;}
	buf=(char*)buf+rlen;
	len-=clen;
	 i++;
	REDRAW();//а это шоб экран обновлялся четко после шага прогрессбара!
  }
}

void writemem ()
{
  unsigned long disk = GetFreeFlexSpace(SF.fl[0]-'0', &err);//-выципает свободное место на исп.диске
  int f;
  char *s;
  FSTATS fst;
  unsigned int fflags=0;
    s=SF.fl+strlen(SF.fl);
    while(*--s!='\\');
    SF.from=getval(++s);
    while(*++s && *s!='-');
    SF.sz=getval(s+1);
    if(GetFileStats(SF.fl,&fst,&err)==-1){
    	fflags=A_WriteOnly+A_Create+A_BIN;
    }else{
    	if(!fst.size)fflags=A_WriteOnly+A_BIN;
    	else fflags=A_WriteOnly+A_Truncate+A_BIN;
    }
    if((f=_open(SF.fl,fflags,P_WRITE,&err))!=-1)
      {
        if(SF.from>=0&&SF.sz>0){
          if (disk>SF.sz){
       SF.wrsz=0;
       	LockSched();
       	loopback2();
       	fwrite32(f,(char*)SF.from,SF.sz,&err);
        UnlockSched();
       State=RES_OK;
                         }else State=FAIL_SPASE;
                          }else State=FAIL_NAME;
        _close(f,&err);
      }else State=FAIL_OPEN;
	REDRAW();
}

int WriteBuf()//тута вынес в конфиг значения буфера записи,просто >тел не потянет
{
	switch(BUFER)
	{
		case 0: SF.bsz=16384; break;
		case 1: SF.bsz=16384*2; break;
		case 2: SF.bsz=16384*4; break;
		case 3: SF.bsz=16384*8; break;
		default:
		 State=FAIL_BUFSZ;
		 return(0);
	}
    return(1);
}
//----------------------------------------------------------------------


 //Redraw
static void OnRedraw(MAIN_GUI *data)
{
  if (data->gui.state==2)
  {
	DrawRectangle(0,YDISP,scr_w,scr_h,0,white,black);
	DrawRectangle(0,scr_h-f_h-2,scr_w,scr_h,0,white,black);
        pixel=(i*((POS.x2-5)-(POS.x+5)))/(SF.sz/SF.bsz);
	switch(State)
	{
        case RES_OK:{
		wsprintf(ws, perc_t, "Готово");
		DrawString(ws,2,YDISP+10,scr_w-2,scr_h,FONT_LARGE,2,white,NULL);
		wsprintf(ws, "Ok");
		DrawString(ws,2,scr_h-f_h-1,scr_w-2,scr_h,FONT_SMALL,4,white0,NULL);
                break;
        }
        case PROCESS:{
		DrawRectangle(POS.x,POS.y,POS.x2,POS.y2+f_h,0,white0,fon);
		DrawRectangle(POS.x+5, POS.y+5, (POS.x+5)+pixel, POS.y2-5, 0,pb,pb);
                wsprintf(ws, perc_t, "Запись...");
                DrawString(ws,2,YDISP+10,scr_w-2,scr_h,FONT_LARGE,2,white,NULL);
		wsprintf(ws, "%X/%X", SF.wrsz,SF.sz);
		DrawString(ws,2,POS.y2-2,scr_w-2,scr_h,FONT_SMALL,2,white,NULL);
		wsprintf(ws, "Cancel");
                DrawString(ws,2,scr_h-f_h-1,scr_w-2,scr_h,FONT_SMALL,4,white0,NULL);

                break;
        }
        case FAIL_WRITE:{
		wsprintf(ws, "%t%d", "Ошибка записи файла!!!\nerr=",err);
	        goto DR_MES;
        }
        case FAIL_CHIP:{
                wsprintf(ws, "%t%d" "Ошибка чтения!!!\nerr=",err);
                goto DR_MES;
        }
        case FAIL_SPASE:{
		wsprintf(ws, perc_t, "Недостаточно места на диске!!!");
                goto DR_MES;
        }
        case FAIL_BUFSZ:{
		wsprintf(ws, perc_t, "Ошибка буфера записи!!!");
                goto DR_MES;
        }
        case FAIL_OPEN:{
                wsprintf(ws, "%t%d", "Ошибка открытия файла!!!\nerr=",err);
//                ShowMSG(1,(int)SF.fl);
                goto DR_MES;
        }
        case FAIL_NAME:{
                wsprintf(ws, perc_t, "Неверное название файла!!!");
                goto DR_MES;
        }
        default:{
		wsprintf(ws, perc_t, "Неизвестная ошибка!!!");
                goto DR_MES;
        }

          DR_MES:
		DrawString(ws,2,YDISP+10,scr_w-2,scr_h,FONT_LARGE,2,white,NULL);
		wsprintf(ws, "Ok");
                DrawString(ws,2,scr_h-f_h-1,scr_w-2,scr_h,FONT_SMALL,4,white0,NULL);
                break;

	}
  }
}

 //Create
void OnCreate(MAIN_GUI *data,void *(*malloc_adr)(int))
{
	scr_w=ScreenW()-1;
	scr_h=ScreenH()-1;
	f_h=GetFontYSIZE(FONT_SMALL); // высота мелкого шрифта
	if(WriteBuf())SUBPROC((void *)writemem); //иначе прогрессбара не увидишь! :)
	data->gui.state=1;
}

 //Close
void OnClose(MAIN_GUI *data,void (*mfree_adr)(void *))
{
	data->gui.state=0;
}

 //Focus
void OnFocus(MAIN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
	DisableIDLETMR();
	REDRAW();                              //...и на скорость записи это не влияет!
	data->gui.state=2;
}

 //Unfocus
void OnUnfocus(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

 //OnKey
int OnKey(MAIN_GUI *data,GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  if ((m==KEY_DOWN)||(m==LONG_PRESS))
  {
	switch(msg->gbsmsg->submess)
	{
	 case RED_BUTTON:
	 case RIGHT_SOFT:
	 case ENTER_BUTTON:
		if(State>=RES_OK) return 1;
		else stopflag=1;
	}
  }
	REDRAW();
  return 0;
}

void onDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  // Это оставить как есть ;)
  mfree_adr(data);
}


 // фигня какая-то
int met0(void){return(0);}

const void * const gui_methods[11]={
  (void *)OnRedraw,
  (void *)OnCreate,
  (void *)OnClose,
  (void *)OnFocus,
  (void *)OnUnfocus,
  (void *)OnKey,
  0,
  (void *)onDestroy, //method7, //Destroy
  (void *)met0,
  (void *)met0,
  0
};

void maincsm_oncreate(CSM_RAM *data)
{
  const RECT Canvas={0,0,0,0};
  MAIN_CSM *csm=(MAIN_CSM*)data;
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  ws=AllocWS(256);
  MAINGUI_ID=csm->gui_id=CreateGUI(main_gui);
}

void maincsm_onclose(CSM_RAM *csm)
{
	FreeWS(ws);
	mfree(SF.fl);
  SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
	{
		csm->csm.state=-3;
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"SaveFlasher");
}

int main(char *exename, char *fname)
{

  if (!fname)//Если тыкаешь не по файлу то показывает Модель тела и прошу.
  {
	sprintf(mes,"%s SW%s",Get_Phone_Info(PI_MODEL),Get_Phone_Info(PI_SW_NUMBER)); ShowMSG(1,(int)mes);
  }else
  {
  	InitializateConig();
	SF.fl=malloc(strlen(fname)+1);
	sprintf(SF.fl,"%s",fname);
    UpdateCSMname();
    LockSched();
	char dummy[sizeof(MAIN_CSM)];
    MAINCSM_ID=CreateCSM(&MAINCSM.maincsm,dummy,0);
    UnlockSched();
  }
  return 1;
}
