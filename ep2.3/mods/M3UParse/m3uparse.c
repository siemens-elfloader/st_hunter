#include "../inc/swilib.h"
#include "m3uparse.h"

const char *NEW_LINE = "\r\n";
const char per_t[] = "%t";
M3U *m3utop;

#define LOG

#ifdef LOG
void log(const char *s)
{
  volatile int hFile;
  unsigned int io_error = 0;
  hFile = fopen("4:\\m3udeb.txt",A_ReadWrite+A_Create+A_Append,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
   {
     fwrite(hFile, s, strlen(s), &io_error);
     fwrite(hFile, "\n", 1, &io_error);
     fclose(hFile, &io_error);
   }
}
#endif
//

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

//из исходника SPlayer
void fix(char* p)  // ”бираем странный символ (всв€зи с переходом на WSHDR)   AAA
{
  unsigned short j=0;
  char* p1=malloc(256);
  for(unsigned short i=0;i<256;i++)
  {
    if(p[i]!=0x1f) {p1[j++]=p[i];}
  }
  strcpy(p,p1);
  mfree(p1);
}

const char error[]="Error";

static void AddM3UItem(char *track,char *path,TTime time)
{
#ifdef LOG1
    log("Add item");
#endif     
    
   fix(path);
   fix(track);
   
   M3U *bmk=malloc(sizeof(M3U));
   bmk->next = 0;
   
  /* zeromem(bmk->track,64);
   zeromem(bmk->path,128);
   snprintf(bmk->track,64, (strlen(track) ? track : error) );
   snprintf(bmk->path,128, (strlen(path) ? path : error) );
*/

   bmk->track = AllocWS(strlen(track)+2);
   bmk->path = AllocWS(strlen(path)+2);
   CutWSTR(bmk->track,0);
   CutWSTR(bmk->path,0);
   wsprintf(bmk->track,per_t,(strlen(track) ? track : error));
   wsprintf(bmk->path,per_t,(strlen(path) ? path : error));
   
   bmk->time = time;
   
   if(!m3utop)//первый элемент
   {
      bmk->index = 0;//начало отсчета
      bmk->prev = 0;//нет пути назад
      m3utop = bmk; 
   }
   else
   {
      M3U *bm=(M3U *)m3utop;
      while(bm->next) bm=bm->next;
      bmk->index = bm->index+1;
      bmk->prev = bm;
      bm->next=bmk;   
   } 
}

void FreeM3UList()
{  
  LockSched();
  M3U *bmk=(M3U *)m3utop;
  m3utop=0;
  UnlockSched();
  while(bmk)
  {
    M3U *bmk_prev=bmk;
    bmk = bmk->next;
    FreeWS(bmk_prev->track);
    FreeWS(bmk_prev->path);
    mfree(bmk_prev);
  }
}

M3U *GetM3UByItem(int curitem)
{
  M3U *bmk;
  bmk = (M3U *)m3utop;
  while(bmk)
  {
    if(bmk->index == curitem) return bmk;
    bmk = bmk->next;  
  }
  return 0;
}

int TotalM3UItem()
{
  if(!m3utop) return 0;
  M3U *bmk;
  bmk=(M3U *)m3utop;
  int i=1; 
  while(bmk=bmk->next) i++;
  return i;
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
        {*d = 'ђ'; d++;}

    if ((ub != 208) && (ub != 209) && (lb != 91) && (lb != 81))
    {
      *d = ub;
      d++;
      s--;
    }
  }
  *d = 0;
}

void find_eof(char *path,char *s)
{
  int j = 0;
  while(*s && j<sizeof(path)) { path[j++] = *s; s++; }         
}

/*
#define find_eof(path,s)\
  {\
         int j = 0;\
         while(*s && j<sizeof(path)) { path[j++] = *s; s++; }\
  }
*/

int ParseM3UFile(char *fname)
{
  int cnt = 0;
  unsigned int ul;
  char *buf = NULL;
  int fsize = 0;
#ifdef LOG
  unlink("4:\\m3udeb.txt",&ul);
#endif

  int f = fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul);
  if (f!=-1)
  {
    char *tmp = malloc((fsize=get_file_size(fname)));
//    fsize=get_file_size(fname);
    buf = malloc(fsize);
    fread(f,tmp/*buf*/,fsize,&ul);
    if((tmp[0]==0xEF && tmp[1]==0xBB && tmp[2]==0xBF) || strchr(tmp,'0x1f'))//is BOM
    {
      utf82win(buf,tmp);
      #ifdef LOG
       log("Open utf8 file");
      #endif
    }
    else
    {
      strcpy(buf,tmp);
      #ifdef LOG
       log("Open win1251 file");
      #endif
    }
    mfree(tmp);
    #ifdef LOG
      log("Open file");
    #endif
  }
  else 
  {
    ShowMSG(1,(int)"Error open file!");
    #ifdef LOG
      log("Error open file");
    #endif
    goto END;
  }
  
  if(!buf)
  {
    #ifdef LOG
     log("Data not read!");
    #endif
    ShowMSG(1,(int)"Empty buffer!");
    goto END;
  }
  
  char track[64];
  char path[128];
  TTime time;
  unsigned int i;
  char *s,*tmp;
  s = buf;
  tmp = buf;

/*  
 *   #EXTM3U
 *   #EXTINF:181,Egypt Central - Over and Under
 *   C:\Documents and Settings\pasha\–абочий стол\225ddab7d46e.mp3
 *   4:\Sounds\034 le vent, le cri.mp3
 *   4:\Sounds\10 ƒорожка 10.mp3         - символ 0x1f
 */
  
#ifdef LOG
    log("start while");
#endif
  while(*s)
  {
    tmp = s;
    zeromem(path,sizeof(path));//сразу почистим,чтобы мусора не было
    zeromem(track,sizeof(track));
    
    if(!strncmp(s,"#EXTM3U",strlen("#EXTM3U")))//пропускаем
    {
      s+=strlen("#EXTM3U")+2;//длинна + \r\n
      tmp = s;
    }

    if(!strncmp(s,"#EXTINF:",strlen("#EXTINF:")))
    {
      //парсим первую строку
      sscanf(s,"#EXTINF:%u",&i);
      time.min = i/60;
      time.sec = i%60;
      
      s=strstr(s,",")+1;  
      tmp = s;
      s = strstr(tmp,NEW_LINE);
      if(s)
        memcpy(track,tmp,s-tmp);//им€ трека, после зап€той
      else
        strcpy(track,"Track error");

      tmp = s + 2;
      s = strstr(tmp,NEW_LINE);//втора€ лини€, с путем
      if(s)
        memcpy(path,tmp,s-tmp);
      else//не нашли перевод строк, тобишь считаем что конец файла
      {
        s = tmp;
        if(*s)//если данные еще есть
          find_eof(path,s);
        else
          strcpy(path,"Path error");
      }      
      cnt++;
      AddM3UItem(track,path,time);
      tmp = s + 2;
    }
    else
      if((*s == '4' || *s == '0') && *(s+1) == ':')//если без #EXTINF, берем в расчет что начинаетс€ с 4:\\ или 0:
      {
        s = strstr(tmp,NEW_LINE);
        if(s)
        {
          memcpy(path,tmp,s-tmp);
          #ifdef LOG
            log("4:  path, find eol");
            log(path);
          #endif
        }
        else//не нашли перевод строк, тобишь считаем что конец файла
        {
        #ifdef LOG
          log("4: eol not found");
        #endif

          s = tmp;
          if(*s)//если еще остались данные
          {
            find_eof(path,s);
          #ifdef LOG
            log(path);
          #endif
          }
          else
          {
            strcpy(path,"Path error");
            #ifdef LOG
              log("4: path error");
            #endif
          }
        }

        fix(path);
        char ptr[128]/* = malloc(strlen(path))*/, *p, *p2;
        zeromem(ptr,sizeof(ptr));
        strcpy(ptr,path);
        if(strlen(ptr))
        {
          p = strrchr(ptr,'\\')+1;
          if(p) //если нашли
          {
            p2 = strrchr(p,'.');
            if(p2) p[strlen(p)-strlen(p2)] = 0;//отрезаем расширение
          }
        }
        else p = "";
        
        time.min = 0;
        time.sec = 0;

        cnt++;
        AddM3UItem(p,path,time);
        tmp = s + 2;
      }
    s++;
  }

END:
 fclose(f,&ul);
 if(buf) mfree(buf);

#ifdef LOG
  log("closed, mfree");
#endif
 return cnt;
}
