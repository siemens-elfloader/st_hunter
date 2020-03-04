
extern int strcmp_nocase(const char *s, const char *d);

const char empty_str[]="";

void debug(int i,char *name,char *icon,char *action)
{
 unsigned int err;
 int f;
 f = fopen( "4:\\zfdebug.txt", A_ReadWrite+ A_Create + A_Append, P_READ+P_WRITE, & err ); 
 char *s;
 s=malloc(strlen(name)+strlen(icon)+strlen(action));
 sprintf(s,"%d.%s|%s|%s|\n",i,name,icon,action);
 if(f!=-1)
 {
  fwrite(f,s,strlen(s),&err);
 }
 fclose(f,&err);
}

char *find_eol(char *s)//ищем конец строки, может понадобится...а может и нет:)
{
  int c;
  s--;
  do
  {
    s++;
    c=*s;
  }
  while((c)&&(c!=10)&&(c!=13));
  return s;
}
//------------------------- First FastPanel -------------------------//

typedef struct
{
  void *next;
  char *name;
  char *icon;
  char *action;
}FASTRUN_1;

volatile FASTRUN_1 *fstop_1;

int GetFListN_1()
{
  int i=0;
  FASTRUN_1 *fl=(FASTRUN_1*)&fstop_1;
  while((fl=fl->next)) i++;
  return (i);
}

void Free_FS_1(void)
{
  LockSched();
  FASTRUN_1 *fl=(FASTRUN_1 *)fstop_1;
  fstop_1=0;
  UnlockSched();
  while(fl)
  {
    FASTRUN_1 *fl_prev;
    fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev->name);
    mfree(fl_prev->icon);
    mfree(fl_prev->action);
    mfree(fl_prev);
  }
}

FASTRUN_1 *AddTo_1(const char *name,const char *icon,const char *action)
{
  FASTRUN_1 *fl;
  FASTRUN_1 *pr;
  FASTRUN_1 *fn=malloc(sizeof(FASTRUN_1));
  fn->name=malloc(strlen(name)+1);
  fn->icon=malloc(strlen(icon)+1);
  fn->action=malloc(strlen(action)+1);
  strcpy(fn->name,name);
  strcpy(fn->icon,icon);
  strcpy(fn->action,action);
  
  fn->next=0;
  fl=(FASTRUN_1 *)&fstop_1;
  if (fl)
  {
    pr=(FASTRUN_1 *)fstop_1;
    pr=fl;
    fl=fl->next;
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fstop_1=fn;
  }
  return (fn);  
}

FASTRUN_1 *FindByN_1(int n)
{
  FASTRUN_1 *fl;
  int i=0;
  int j=0;
  int k=GetFListN_1();  
  fl=(FASTRUN_1 *)fstop_1;
  j=k-n-1;//Я видимо талпаеп,или сименс все таки сименс оч загадочный аппарат)) если написать так k-=n-1 то буит пикофф...
  if(j==-1) fl=fl->next;
  else
  {
    for(i=0;i<j;i++)
     {
      fl=fl->next;
     }
  }  
  return fl;
}

//------------------------- Second FastPanel -------------------------//

typedef struct
{
  void *next;
  char *name;
  char *icon;
  char *action;
}FASTRUN_2;

volatile FASTRUN_2 *fstop_2;

int GetFListN_2()
{
  int i=0;
  FASTRUN_2 *fl=(FASTRUN_2*)&fstop_2;
  while((fl=fl->next)) i++;
  return (i);
}
void Free_FS_2(void)
{
  LockSched();
  FASTRUN_2 *fl=(FASTRUN_2 *)fstop_2;
  fstop_2=0;
  UnlockSched();
  while(fl)
  {
    FASTRUN_2 *fl_prev;
    fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev->name);
    mfree(fl_prev->icon);
    mfree(fl_prev->action);
    mfree(fl_prev);
  }
}


FASTRUN_2 *AddTo_2(const char *name,const char *icon,const char *action)
{
  FASTRUN_2 *fl;
  FASTRUN_2 *pr;
  FASTRUN_2 *fn=malloc(sizeof(FASTRUN_2));
  fn->name=malloc(strlen(name)+1);
  fn->icon=malloc(strlen(icon)+1);
  fn->action=malloc(strlen(action)+1);
  strcpy(fn->name,name);
  strcpy(fn->icon,icon);
  strcpy(fn->action,action);
  
  fn->next=0;
  fl=(FASTRUN_2 *)&fstop_2;
  if (fl)
  {
    pr=(FASTRUN_2 *)fstop_2;
    pr=fl;
    fl=fl->next;
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fstop_2=fn;
  }
  return (fn);  
}

FASTRUN_2 *FindByN_2(int n)
{
  FASTRUN_2 *fl;
  int i=0;
  int j=0;
  int k=GetFListN_2();  
  fl=(FASTRUN_2 *)fstop_2;
  j=k-n-1;//Я видимо талпаеп,или сименс все таки сименс оч загадочный аппарат)) если написать так k-=n-1 то буит пикофф...
  if(j==-1) fl=fl->next;
  else
  {
    for(i=0;i<j;i++)
     {
      fl=fl->next;
     }
  }  
  return fl;
}

//------------------------- Third FastPanel -------------------------//

typedef struct
{
  void *next;
  char *name;
  char *icon;
  char *action;
}FASTRUN_3;

volatile FASTRUN_3 *fstop_3;

int GetFListN_3()
{
  int i=0;
  FASTRUN_3 *fl=(FASTRUN_3*)&fstop_3;
  while((fl=fl->next)) i++;
  return (i);
}
void Free_FS_3(void)
{
  LockSched();
  FASTRUN_3 *fl=(FASTRUN_3 *)fstop_3;
  fstop_3=0;
  UnlockSched();
  while(fl)
  {
    FASTRUN_3 *fl_prev;
    fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev->name);
    mfree(fl_prev->icon);
    mfree(fl_prev->action);
    mfree(fl_prev);
  }
}


FASTRUN_3 *AddTo_3(const char *name,const char *icon,const char *action)
{
  FASTRUN_3 *fl;
  FASTRUN_3 *pr;
  FASTRUN_3 *fn=malloc(sizeof(FASTRUN_3));
  fn->name=malloc(strlen(name)+1);
  fn->icon=malloc(strlen(icon)+1);
  fn->action=malloc(strlen(action)+1);
  strcpy(fn->name,name);
  strcpy(fn->icon,icon);
  strcpy(fn->action,action);
  
  fn->next=0;
  fl=(FASTRUN_3 *)&fstop_3;
  if (fl)
  {
    pr=(FASTRUN_3 *)&fstop_3;
    pr=fl;
    fl=fl->next;
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fstop_3=fn;
  }
  return (fn);  
}

FASTRUN_3 *FindByN_3(int n)
{
  FASTRUN_3 *fl;
  int i=0;
  int j=0;
  int k=GetFListN_3();  
  fl=(FASTRUN_3 *)fstop_3;
  j=k-n-1;//Я видимо талпаеп,или сименс все таки сименс оч загадочный аппарат)) если написать так k-=n-1 то буит пикофф...
  if(j==-1) fl=fl->next;
  else
  {
    for(i=0;i<j;i++)
     {
      fl=fl->next;
     }
  }  
  return fl;
}


//------------------------- IDLE Panel -------------------------//

typedef struct
{
  void *next;
  char *name;
  char *icon;
  char *action;
}FASTRUN_IDLE;

volatile FASTRUN_IDLE *fstop_idle;

int GetFListN_IDLE()
{
  int i=0;
  FASTRUN_IDLE *fl=(FASTRUN_IDLE*)&fstop_idle;
  while((fl=fl->next)) i++;
  return (i);
}

void Free_FS_IDLE(void)
{
  LockSched();
  FASTRUN_IDLE *fl=(FASTRUN_IDLE *)fstop_idle;
  fstop_2=0;
  UnlockSched();
  while(fl)
  {
    FASTRUN_IDLE *fl_prev;
    fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev->name);
    mfree(fl_prev->icon);
    mfree(fl_prev->action);
    mfree(fl_prev);
  }
}


FASTRUN_IDLE *AddTo_IDLE(const char *name,const char *icon,const char *action)
{
  FASTRUN_IDLE *fl;
  FASTRUN_IDLE *pr;
  FASTRUN_IDLE *fn=malloc(sizeof(FASTRUN_IDLE));
  fn->name=malloc(strlen(name)+1);
  fn->icon=malloc(strlen(icon)+1);
  fn->action=malloc(strlen(action));
  strcpy(fn->name,name);
  strcpy(fn->icon,icon);
  strcpy(fn->action,action);
  
  fn->next=0;
  fl=(FASTRUN_IDLE *)&fstop_idle;
  if (fl)
  {
    pr=(FASTRUN_IDLE *)&fstop_idle;
    pr=fl;
    fl=fl->next;
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fstop_idle=fn;
  }
  return (fn);  
}

FASTRUN_IDLE *FindByN_IDLE(int n)
{
  FASTRUN_IDLE *fl;
  int i=0;
  int j=0;
  int k=GetFListN_IDLE();  
  fl=(FASTRUN_IDLE *)fstop_idle;
  j=k-n-1;//Я видимо талпаеп,или сименс все таки сименс оч загадочный аппарат)) если написать так k-=n-1 то буит пикофф...
  if(j==-1) fl=fl->next;
  else
  {
    for(i=0;i<j;i++)
     {
      fl=fl->next;
     }
  }  
  return fl;
}

//------------------- Собственно чтение конфигов :) --------------------//

int num;
void parse(char *str,int i)//Читаем конфиг и сохраняем пути
{ 
  int j=0;
  int k=0;
  char *name;
  char *icon;
  char *action;
  name=malloc(64);
  icon=malloc(128);
  
  if(str[0]==';') return;//Комментарии,пригодится)
  
  //Если встречается надпись,то устанавливаем флаг для какой панели сохранять и выходим из фунции для чтения дальше
  if(!strncmp(str,"[FirstPanel]" ,strlen("[FirstPanel]" ))) { num=1; return; }
  if(!strncmp(str,"[SecondPanel]",strlen("[SecondPanel]"))) { num=2; return; }
  if(!strncmp(str,"[ThirdPanel]" ,strlen("[ThirdPanel]" ))) { num=3; return; }
  if(!strncmp(str,"[IdlePanel]"  ,strlen("[IdlePanel]"  ))) { num=4; return; }
     
  while (str[j]!='|') { name[k++]=str[j]; j++; }    
  name[k++]=0;
  j++;//пропускаем '|'
  k=0;
  while (str[j]!='|') { icon[k++]=str[j]; j++; }    
  icon[k++]=0;
  
  action=strrchr(str,'|');//Меньше циклов,меньше пиков)) проверенно....
  
  if(num==1) { AddTo_1(name,icon,action+1);   }/* debug(num,name,icon,action+1);*/
  if(num==2) { AddTo_2(name,icon,action+1);   }
  if(num==3) { AddTo_3(name,icon,action+1);   }
  if(num==4) { AddTo_IDLE(name,icon,action+1);}  
  mfree(name);
  mfree(icon);
}

int get_file_size(const char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

void LoadFastrunBmk()
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  int size; 
  int i; 
  char *str; 
  int j;
  int k=0;
  plhandle = fopen( config , A_ReadOnly + A_BIN, P_READ, & err ); 
  if ( plhandle==-1 ) 
   {
    extern int EditConfig();
    EditConfig();
    ShowMSG(1,(int)"Can't open config file!");
    fclose( plhandle, & err ); 
    return; 
   }
  mem=malloc(get_file_size(config)); 
  str=malloc(256); 
  if ((mem!=0)&&(str!=0)) 
   { 
    size=fread(plhandle,mem,get_file_size(config),&err); 
    i=0; 
    while (i<size) //пока не конец файлa
    { 
      j=0; 
      while (((*(mem+i)!='\n')) && (i<size) ) {*(str+j)=*(mem+i); j++; i++; }//читаем строку из файла 
      *(str+j)=0;
      parse(str,k);
      i++;  
    }
  }
fclose(plhandle,&err); 
mfree(mem); 
mfree(str); 
}

void free()
{
 Free_FS_1();
 Free_FS_2();
 Free_FS_3();
 Free_FS_IDLE();
}


