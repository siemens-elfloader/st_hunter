/*
 * File:   main.c
 * Author: Tonich
 *
 * Created on 27 Октябрь 2011 г., 3:00
 */

#include <stdio.h>
#include <stdlib.h>
#include <swilib.h>


#include "main.h"
#include "IPCExtD.h"
#include "parser.h"
#include "extsdata.h"

const int minus11=-11;
unsigned short maincsm_name_body[128];
int CSM_ID=0;
int GlobalError=0;
int State=0;
const char *elfname="GExtD";

CSM_DESC icsmd;
void (*old_icsm_onClose)(CSM_RAM*);

char *exe=NULL,*param=NULL;
char extfile[]="?:\\ZBin\\etc\\extension.cfg";  //Указатель на загруженный extension.cfg
char *maincfg=NULL;             //Содержимое этого файла
SSIM *mainstrs=NULL;            //Содержимое разобранное по строком с отсевом комментов

const char *tonich="Tonich";
const int version=0x0100;

IPC_CHECK ipccheck;

char *EXTerrors[]={
    "Невозможно открыть \nконфиг файл!",
    "Ошибка,возможно файл пуст!",
    "Не могу выделить \nосновную память!",
    "Не могу прочитать файл!",
    "Нет полезных данных \nв конфиге!"
};

char *EMESSAGES[]={
	"Реконфигу-\nрировано!",
	"Реконфигу-\nрировано с \nпараметрами!"

};

/*
 *
 */

void clean_env()
{
    unsetenv(elfname);
}

void DoRunExt(char *extcfg){
    FILE *fh;
    unsigned int cfg_size=0;
    if((fh=fopen(extcfg,"rb"))==NULL){
        ShowMSG(1,(int)EXTerrors[ERR_OPEN_CFG]);
        GlobalError=ERR_OPEN_CFG;
        //CloseCSM(CSM_ID);
    }else{
        if(fseek(fh,0,SEEK_END)!=0){
            fclose(fh);
            ShowMSG(1,(int)EXTerrors[ERR_FILE_LEN]);
            GlobalError=ERR_FILE_LEN;
//            CloseCSM(CSM_ID);
        }else{
        	cfg_size=ftell(fh);
            fseek(fh,0,SEEK_SET);
            if((maincfg=(char *)malloc(cfg_size+1))==NULL){
                fclose(fh);
                ShowMSG(1,(int)EXTerrors[ERR_ALLOC_MEM]);
                GlobalError=ERR_ALLOC_MEM;
//                CloseCSM(CSM_ID);
            }else{
                if((fread(maincfg,cfg_size,1,fh))!=1){
                    free(maincfg);
                    maincfg=NULL;
                    fclose(fh);
                    ShowMSG(1,(int)EXTerrors[ERR_FILE_READ]);
                    GlobalError=ERR_FILE_READ;
//                    CloseCSM(CSM_ID);
                }else{
						maincfg[cfg_size]=0;
                        mainstrs=DoParse(maincfg);  //Все прочиталось теперь парсим
                        if(!mainstrs){
                            free(maincfg);
                            maincfg=NULL;
                            ShowMSG(1,(int)EXTerrors[ERR_DATA_COUNT]);
                            GlobalError=ERR_DATA_COUNT;
 //                           CloseCSM(CSM_ID);
                        }else{
                                                        //Пускаем его работать
                            AddExtFromStrs(mainstrs);
                            RegExts(ES_INFO.root);
                        }
                        fclose(fh);
                }
            }
        }
    }
}

void DoRun(){
    extfile[0]=exe[0];
    DoRunExt(extfile);
}


int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  if(msg->msg == MSG_IPC)
  {
  	IPC_REQ *ipc;
  	if ((ipc=(IPC_REQ*)msg->data0)){
  		if (!strcmp(ipc->name_to,elfname)){			//Это нам
		IPC_CHECK *data=ipc->data;
		switch(msg->submess){
          case IPC_RECONFIG:{								//Пришел запрос обновление
          	if(State){												//И пришел по адресу
          	if(maincfg)free(maincfg);
          	UnregAll();
			ParseClear(mainstrs);
			mainstrs=NULL;
			GlobalError=0;
			if(data){
				if(data->param){
					ShowMSG(1,(int)EMESSAGES[EMSG_RECONFWPARM]);
					DoRunExt(param);
          	}else{
					ShowMSG(1,(int)EMESSAGES[EMSG_RECONFIGURE]);
					DoRun();
          	}
			}else{
					ShowMSG(1,(int)EMESSAGES[EMSG_RECONFIGURE]);
					DoRun();
          	}
          	if(GlobalError){
          	ipccheck.err=GlobalError;
          	ipccheck.param=(char *)EXTerrors[GlobalError];
          	}else{
          	ipccheck.err=0;
			ipccheck.param=data->param;
          	}
			IPCsend.name_from=elfname;
			IPCsend.name_to=ipc->name_from;
          SendIpcData(IPC_CONFASK,&ipccheck);	//Сразу шлем ответ,именно после него надо действовать дальше в параллельном эльфе....
          	}
          break;
      }
      case IPC_CONFASK:{								//Ответ по поводу обновления
			if(!State)CloseCSM(CSM_ID);
			break;
      }
      case IPC_GETEXT:{									//Запрос инфы о расширении
			if(data){
				ipccheck.param=(char *)FindByExt(data->param);
				ipccheck.err=ipccheck.param?0:1;
				IPCsend.name_from=elfname;
				IPCsend.name_to=ipc->name_from;
				SendIpcData(IPC_GETEXT,&ipccheck);
			}
			break;
      }
		case IPC_SETEXT:{									//Установка расширения
			if(data){
				AddToES_INFO((ES *)data->param,data->err);
				ipccheck.err=RegExt(ES_INFO.cur);
				ipccheck.param=NULL;
				IPCsend.name_from=elfname;
				IPCsend.name_to=ipc->name_from;
				SendIpcData(IPC_SETEXT,&ipccheck);
			}
			break;
      }
//      case ЧЕНИТЬ ИСЧО
  }

  		}

  	}
//  if(msg->msg);
}
return 1;
}
static void maincsm_oncreate(CSM_RAM *data)
{
const char *env = getenv(elfname);
if(!env){
	setenv(elfname, (const char*)CSM_ID, 1);
	atexit(clean_env); // при убивании проги сотрем переменную окружения
	State=1;
	if(param){
		DoRunExt(param);
	}else{
		DoRun();
	}
}else{
          	ipccheck.err=0;
          	ipccheck.param=param;
			IPCsend.name_from=elfname;
			IPCsend.name_to=elfname;
          SendIpcData(IPC_RECONFIG,&ipccheck);
}
}

static void Killer(void)
{
    kill_elf();
}
static void maincsm_onclose(CSM_RAM *csm)
{
    if(maincfg)free(maincfg);
    if(exe)free(exe);
    if(param)free(param);
    UnregAll();
    ParseClear(mainstrs);
    mainstrs=NULL;
  SUBPROC((void *)Killer);
}

static const struct
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
    127
  }
};

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"%t",elfname);
}

int main(char *exename, char* filename) {

    exe=(char *)malloc(strlen(exename)+1);
    strcpy(exe,exename);
    if(filename){
    param=(char *)malloc(strlen(filename)+1);
    strcpy(param,filename);
    }
  CSM_RAM *save_cmpc;
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  CSM_ID=CreateCSM(&MAINCSM.maincsm,dummy,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
					//Передать параметры запущенному
					//(int)env	-ID
    return (EXIT_SUCCESS);
}

