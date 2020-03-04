#include "include.h"
#include "List.h"

#define FUNC_PT_START	0xA085DEA4
#define FUNC_PT_END	0xA0865BA3
#define FUNC_PT_PSIZE	0x80
#define FUNC_PT_OFFSET	0x10
#define MAX_FUNC	0xFA

#pragma inline=forced
int toupper(int c)
{
  if((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}


//Взято с китайского свн sieelf.googlecode.com/svn (r)BingK

#ifdef NEWSGOLD
unsigned int GetFunctionPointByName(char *name)
{
  char *p=(char *)FUNC_PT_START;
  int len=strlen(name);
  if(len==0)
     return 0;
  while((char *)FUNC_PT_END-p>0)
   {
     if(!strncmpNoCase(p, name, len))
	return (*(unsigned int *)(p+FUNC_PT_OFFSET));
     p+=FUNC_PT_PSIZE;
   }
  return 0;
}
#endif


void run_address(unsigned int address) 
{
  void (*runaddr)(void);
  runaddr=(void (*)())address;
  if (runaddr) 
  {
    runaddr();
  }
}

void run_shortcut(char *shortcut) 
{
#ifdef NEWSGOLD
   run_address(GetFunctionPointByName(shortcut));
#else
   run_address((unsigned int)GetFunctionPointer(shortcut));
#endif
}

int chr2num(char chr)
{
  if(chr>='0'&&chr<='9')
     return (chr-('0'-0));
  if(chr>='A'&&chr<='Z')
     return (chr-('A'-0xA));
  if(chr>='a'&&chr<='z')
     return (chr-('a'-0xA));
  return 0;
}

extern CList *list;

int RunFunc(int bm)
{
 const char *s = list->Items(bm)->name;//bmfile(bm);
 if((s)&&strlen(s))
 {
   
  if(strstr(s,"SetProfile"))//SetProfile(1;3)
  {
    (GetProfile()!=2) ? SetProfile(2) : SetProfile(0);
    return 1;
  }
  //MakeVoiceCall(const char *number,int _0x10, int _0x20C0);
  
  if(s[0]=='8' || s[0]=='+' || strstr(s,"call:"))
  {
    const char *p;
    if(strstr(s,"call:")) p=strrchr(s,':')+1;
    else p=s;

    MakeVoiceCall(p,0x10,0x20C0); 
    return 1;
  }
  
  if ((s[2]=='\\')&&((s[(strlen(s))-3]=='.')||//Проверяем строку на наличие символов '\\' и '.'
     (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
       {
        WSHDR *ws;
        ws=AllocWS(256);
        str_2ws(ws,s,256);
        if(ExecuteFile(ws,0,0))
        {
          FreeWS(ws);
          return(1);
        }
        else
        {
          FreeWS(ws);
          return 0;
        }
       }
    
    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       ((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))  
        {
          int i;
          unsigned int addr=0;
          char *p=(char*)s;
          for(i=0;i<strlen(s);i++)
          {
             addr=chr2num(p[i])+addr*0x10;
          }
          run_address(addr);
          return 1;
        }
    else
    {
          char ShortCut[32];
          char *p=(char*)s;
          int i=0;
          int j=0;
          for(;j<strlen(s);j++)
          {
            if(p[j]>='A')
            {
              ShortCut[i]=p[j];
              i++;
            }
          }
          ShortCut[i]=0;
          run_shortcut(ShortCut);     
          return 1;
    }  
  }
 return(0);
}
