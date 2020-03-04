#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "main.h"
#define BUFSIZE 16383

//#define LOG
//#define DEBUG

char buf[BUFSIZE+1];
static WEATHER weath[12];
char Error[]="Error!";
int total_item = 0;

int sock;
int connect_state=0;
int pbuf=0;
int traf=0;

const char per_d[]="%d";
const char per_s[]="%s";

extern const unsigned int refresh_time;
extern const char logpath[];
extern const char id_tower[];

GBSTMR mytmr;

void start_again(void);

void create_connect(void)
{
  zeromem(buf,sizeof(buf));
  SOCK_ADDR sa;
  connect_state=0;
  pbuf=0;
  sock=socket(1,1,0);
  if (sock!=-1)
  {
    sa.family=1;
    sa.port=htons(80);
    sa.ip=htonl(IP_ADDR(94,100,178,193));
    if (connect(sock,&sa,sizeof(sa))!=-1)
    {
      connect_state=1;
    }
    else
    {
      closesocket(sock);
      GBS_StartTimerProc(&mytmr, 12960, start_again);
    }
  }
  else
    GBS_StartTimerProc(&mytmr, 12960, start_again);
}

void start_again()
{
  if(IsGPRSConnected())
  {
   SUBPROC((void *)create_connect);
  }
  else
   GBS_StartTimerProc(&mytmr, 2160, start_again);
}

void send_req(void)
{
  char *req_buf=malloc(1200);//forecast3.xhtml?
  sprintf(req_buf,"GET /forecast3.xhtml?&city=%s HTTP/1.1\n"
            "Accept-Language: en-us\n"
              "Accept-Encoding: gzip, deflate\n"
                "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\n"
                  "Host: wap.weather.mail.ru\n"
                    "Connection: Close\n"
                      "\n"
                        , id_tower);
  send(sock,req_buf,strlen(req_buf),0);
  connect_state=2;
  mfree(req_buf);
}

void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
    connect_state=0;
  }
}

void get_answer(void)
{
  int i=pbuf;
  if (i==BUFSIZE)
    end_socket();
  else
  {
    i=recv(sock,buf+i,BUFSIZE-i,0);
    if (i>=0)
    {
      pbuf+=i;
    }
    else
    {
      end_socket();
    }
  }
}

#ifdef LOG
void save_to_file()
{
  volatile int hFile;
  unsigned int io_error = 0;
  unsigned int ul;
  FSTATS stat;
  GetFileStats("4:\\debug_weather.txt",&stat,&ul);
  hFile = fopen("4:\\debug_weather.txt",A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
   {
     fwrite(hFile, buf, strlen(buf), &io_error);
     fclose(hFile, &io_error);
   }
}
#endif

void log(const char *s)
{
#ifndef LOG//мля, напрягает в коде кучу #ifdef LOG #endif...пусть будет так
  return;
#else
  volatile int hFile;
  unsigned int io_error = 0;
  unsigned int ul;
  hFile = fopen("4:\\weather1.txt",A_ReadWrite+A_Create+A_Append,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
   {
     fwrite(hFile, s, strlen(s), &io_error);
     fwrite(hFile, "\n", 1, &io_error);
     fclose(hFile, &io_error);
   }
#endif
}

void utf82win(char*d,const char *s)
{
  for (; *s; s+=2)
  {
    unsigned char ub = *s, lb = *(s+1);
    if (ub == 208)
      if (lb != 0x81)
        {*d = lb + 48; d++;}
      else
        {*d = 'и'; d++;}

    if (ub == 209)
      if (lb != 0x91)
        {*d = lb + 112; d++;}
      else
        {*d = '¬'; d++;}

    if ((ub != 208) && (ub != 209) && (lb != 91) && (lb != 81))
    {
      *d = ub;
      d++;
      s--;
    }
  }
  *d = 0;
}


/*
<div class="inner">

<h1>Новочеркасск&#032;<a href="/citychange.xhtml?city=1956">Изменить</a></h1>

<div class="forecastblock">
<h3>6.04, понедельник</h3>
<div class="forecast">
  Сейчас<img src="/img/new/pict_weather_med_7.gif" width="41" height="41" alt="ясно" /><span class="spec">+9&deg;</span>
</div>
<div class="forecasttext">
  <div><strong>757</strong>&#032;mmHg, <strong>42</strong>%, <strong>Ю 1</strong>&#032;м/с</div>
  ясно
</div>
*/

void AddItem(WEATHER w)
{
  if(total_item>12) return;//тока 12 элементов
  strcpy(weath[total_item].town,      strlen(w.town)     ? w.town     : Error);
  strcpy(weath[total_item].date,      strlen(w.date)     ? w.date     : Error);
  strcpy(weath[total_item].daystate,  strlen(w.daystate) ? w.daystate : Error);
  strcpy(weath[total_item].temp,      strlen(w.temp)     ? w.temp     : Error);
  strcpy(weath[total_item].speed,     strlen(w.speed)    ? w.speed    : Error);
  strcpy(weath[total_item].davl,      strlen(w.davl)     ? w.davl     : Error);
  strcpy(weath[total_item].vlazn,     strlen(w.vlazn)    ? w.vlazn    : Error);
  strcpy(weath[total_item].picname,   strlen(w.picname)  ? w.picname  : Error);
  strcpy(weath[total_item].prognoze,  strlen(w.prognoze) ? w.prognoze : Error);
  
//  snprintf(weath[total_item]., sizeof(weath[total_item].), strlen(weath[total_item].) ? weath[total_item]. : Error);
  total_item++;
}

void SaveData()
{
  volatile int f;
  unsigned int ul;
  int i=0;
  unlink(logpath,&ul);
  f = fopen(logpath,A_ReadWrite+A_Create+A_Append+A_BIN,P_READ+P_WRITE, &ul);
  if(f!=-1)
   {
    for(i=0;i<total_item;i++)
    {
      lseek(f,sizeof(WEATHER)*i,0,&ul,&ul);
      fwrite(f,&weath[i],sizeof(WEATHER),&ul);
    }
   }
  fclose(f, &ul);
}

void find_symb(char *s,char *ptr,int c)
{
  int i=0;
  zeromem(ptr,sizeof(ptr));
  while(*s!=c/* && i<sizeof(ptr)*/)//ищем символ и следим чтоб не убежали за предел массива
  {
    ptr[i] = *s;
    i++;
    *s++;
  }
  ptr[i] = 0;
}

char forecast[]="forecast";
char forecasttext[]="forecasttext";
char forecastblock[]="forecastblock";
char *daystate[]={"Ночь","Утро","День","Вечер"};

void ParseBuf(char *s1, char *s2,WEATHER *w)
{
  if(s1)
  {
    if(s1=strstr(s1,"<img src"))//картинка
     {
       log("pictures");
       s1+=strlen("<img src=")+1+strlen("/img/new/");
       find_symb(s1, (char *)w->picname, '.');
       log(w->picname);
     }
    
     if(s1=strstr(s1,"><span class="))//температура
     {
       log("temperature");
       s1+=strlen("><span class=")+strlen("spec")+3;
       find_symb(s1, (char *)w->temp, '&');
       log(w->temp);
     }
  }
  
  if(s2)
  {
     //<div class="forecasttext">
     //<div><strong>757</strong>&#032;mmHg, <strong>42</strong>%, <strong>Ю 1</strong>&#032;м/с</div>ясно
    if(s2=strstr(s2,"<strong>"))//давление
    {
      log("davlenie");
      s2+=strlen("<strong>")+1;
      find_symb(s2,(char*)w->davl,'<');
      strcat((char*)(w->davl+strlen(w->davl))," mmHg");
      log(w->davl);
    }
    
    if(s2=strstr(s2,"<strong>"))//влажность
    {
      log("vlaznost");
      s2+=strlen("<strong>")+1;
      find_symb(s2, (char *)w->vlazn, '<');
      strcat((char*)(w->vlazn+strlen(w->vlazn))," %");
      log(w->vlazn);
    }
    
    if(s2=strstr(s2,"<strong>"))//скорость ветра
    {
      log("speed veter");
      s2+=strlen("<strong>")+2;
      find_symb(s2, (char *)w->speed, '<');
      strcat((char*)(w->speed+strlen(w->speed))," м/с");
      w->speed[strlen(w->speed)] = 0;
      log(w->speed);
    }
    
    if(s2=strstr(s2,"</div>"))//погода
    {
      log("prognoze");
      s2+=strlen("</div>")+3;//+3 ибо \r\n + пробел
      find_symb(s2, (char *)w->prognoze, '\n');
      log(w->prognoze);
    }
  }//if(s2)  
}

void Parsing()
{
   log("start parsing");
   WEATHER w;
   int block = 0;//все ли 3 блока приняли?
   char *fblock;
   char *s1,*s2;
   char *buf2 = malloc(strlen(buf));
   strcpy(buf2,buf);
   utf82win(buf,buf2);//с сайта принимается в utf8, перегоняем в вин1251
   mfree(buf2);
   char *buf1 = (char*)buf;   
#ifdef LOG
   save_to_file();
#endif   
   
   if(strlen(buf))
   {
     fblock = (char*)buf;
     for(int i=0;i<3;i++)//если нашли три блока "forecastblock" то все гуд
     {
       if(fblock=strstr(fblock,forecastblock))
       {
         fblock+=strlen(forecastblock)+2;
         block++;
       }
     }
     
     if(s1=strstr(buf1,"<h1>")) //город
     {
       s1+=strlen("<h1>");
       log("town");
       find_symb(s1,w.town,'&');
       log(w.town);
     }
  
     {
      for(int j=0 ; j<block && j<3 ; j++)//3 блока погоды
      {
        log("------block start------");
        if(s1=strstr(buf1,"<h3>"))//дата
        {
          s1+=strlen("<h3>");
          find_symb(s1,w.date,'<');
          log(w.date);
        }
        
        for(int i=0;i<4;i++)//4 подблока
        {
           strcpy(w.daystate,daystate[i]);
           log(w.daystate);
           if((s1=strstr(buf1,forecast)) && (s2=strstr(buf1,forecasttext)))
           {
             buf1 = s2; //переходим дальше по буферу
             buf1 += strlen(forecasttext); //+пропускаем "forecasttext" чтоб избежать неприятности =)
             ParseBuf(s1,s2,&w);          
             AddItem(w);
           }
        }
        log("------ block end ------");
      }
    }
    
    SaveData();
    ShowMSG(1,(int)"All data recv!");
    GBS_StartTimerProc(&mytmr, (60*216)*refresh_time, start_again);
  }
  else
  {
    ShowMSG(1,(int)"Wrong reciev data! Waiting 60 second!");
    GBS_StartTimerProc(&mytmr, 60*216, start_again);
  }
} 


#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"WeatherD v3.0 config updated!");
      InitConfig();
      GBS_DelTimer(&mytmr);
      SUBPROC((void *)create_connect);
    }
  }
  
  if (msg->msg==MSG_HELPER_TRANSLATOR)
  {
    if ((int)msg->data1==sock)
    {
      //Если наш сокет
      switch((int)msg->data0)
      {
      case ENIP_SOCK_CONNECTED:
        if (connect_state==1)
        {
          //Если посылали запрос
          SUBPROC((void *)send_req);        
        }
        else
          {
            SUBPROC((void *)create_connect); 
          }
        break;
      case ENIP_SOCK_DATA_READ:
        if (connect_state==2)
        {
          //Если посылали send
          SUBPROC((void *)get_answer);
        }
        else
        {
          SUBPROC((void *)create_connect);
        }
        break;
      case ENIP_SOCK_REMOTE_CLOSED:
        //Закрыт со стороны сервера
        if (connect_state) SUBPROC((void *)end_socket);
        break;
      case ENIP_SOCK_CLOSED:
        //Закрыт вызовом closesocket
        if (connect_state) SUBPROC((void *)end_socket);
        connect_state=3;
    //    GBS_StartTimerProc(&delaytmr, 262*10,Parsing);
        Parsing();
        sock=-1;
        break;
      }
    }
  }    
 return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{
  
}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
  GBS_DelTimer(&mytmr);
  SUBPROC((void *)Killer);
}

static unsigned short maincsm_name_body[140];

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
    139
  }
};

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"WeatherD v3.0 - (c)kluchnik");
}

int main(char *filename)
{ 
#ifndef DEBUG
  
  InitConfig();
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  UpdateCSMname(); 
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
  
  start_again();//стартуем
  
#else

  //если нуна отладить
  volatile int hFile;
  unsigned int io_error = 0;
  unlink("4:\\weather1.txt",&io_error);
  hFile = fopen("4:\\forecast3.txt",A_ReadOnly,P_READ, &io_error);
  if(hFile!=-1)
   {
     fread(hFile, buf, sizeof(buf), &io_error);
     fclose(hFile, &io_error);
   }
  else
  {
    ShowMSG(1,(int)"Err file");
    return 0;
  }
  Parsing();
#endif
  
  return 0;
}
