int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (int k=0; ; k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A)
    {
      ret=ret*10+attr[k]-0x30;
    } 
    else
    {
      if( attr[k]=='-')
      {
        neg=-1;
      } 
      else
      {
        return(ret*neg);
      }
    }
  }
}
#pragma inline
int char_to_hex(int c)
{
  if (c>='0' && c<='9') return c-'0';
  if (c>='A' && c<='F') return c-'A'+10;
  if (c>='a' && c<='f') return c-'a'+10;
  return -1;
}

/*
void getcolor(int i,char *s)//FF FF FF 64
{
  int r=0x0,g,b,a;
  sscanf(s,"%02X%02X%02X%02X",&r,&g,&b,&a);
  char *ss=malloc(64);
  switch(i)
  {
   case 0:
    cfgPanBorderCol_r=r;
    cfgPanBorderCol_g=g;
    cfgPanBorderCol_b=b;
    cfgPanBorderCol_a=a;
    cfgPanBorderCol[4]={cfgPanBorderCol_r,cfgPanBorderCol_g,cfgPanBorderCol_b,cfgPanBorderCol_a};
    sprintf(cfgPanBorderCol,"%d,%d,%d,%d",r,g,b,a);
    //ShowMSG(2,(int)cfgPanBorderCol);
//    ShowMSG(2,(int)cfgPanBorderCol);
    return;
   case 1:
    cfgPanBGCol[0]=r;
    cfgPanBGCol[1]=g;
    cfgPanBGCol[2]=b;
    cfgPanBGCol[3]=a;
        sprintf(cfgPanBGCol,"%d,%d,%d,%d",r,g,b,a);
//    ShowMSG(1,(int)cfgPanBGCol);
    return;
  case 2:
    cfgBookBorderCol[0]=r;
    cfgBookBorderCol[1]=g;
    cfgBookBorderCol[2]=b;
    cfgBookBorderCol[3]=a;
    sprintf(cfgBookBorderCol,"%d,%d,%d,%d",r,g,b,a);
    return;
  case 3:
    cfgBookBGCol[0]=r;
    cfgBookBGCol[1]=g;
    cfgBookBGCol[2]=b;
    cfgBookBGCol[3]=a;
    sprintf(cfgBookBGCol,"%d,%d,%d,%d",r,g,b,a);
    return;
  }
}*/
/*int n=0;

void process(char *str)//„итаем конфиг
{ 
   if(*str==';') return;//комментарии
   if(n<12)//first panel
   {                                              //n-сколько прочитали настроек,str+=22 смещение
     if(!strncmp(str,"Display FastRun panel:",22)){ n++; str+=22; FASTRUN_ENA=strtoul(str,0,10); return; }     
     if(!strncmp(str,"key activation style",20))  { n++; str+=22; ENA_FR_LONGPRESS=strtoul(str,0,10); return; }
     if(!strncmp(str,"Enable key (DEC)",16))      { n++; str+=22; FR_CALL_BUTTON=strtoul(str,0,10); return; }
     if(!strncmp(str,"Disable key (DEC)",17))     { n++; str+=22; FR_EXIT_BUTTON=strtoul(str,0,10); return; }
     if(!strncmp(str,"Position on screen",18))    { n++; str+=22; DRAW_MODE=strtoul(str,0,10); return; }
     if(!strncmp(str,"Offset",6))                 { n++; str+=22; BEGIN=strtoul(str,0,10); return; }
     if(!strncmp(str,"Panel lenght",12))          { n++; str+=22; LEN=strtoul(str,0,10); return; }
     if(!strncmp(str,"Pictures size",13))         { n++; str+=22; fr_pic_size=strtoul(str,0,10); return; }   
     if(!strncmp(str,"Panel border col",16))      { n++; str+=22; getcolor(0,str); return; }
     if(!strncmp(str,"Panel BG col",12))          { n++; str+=22; getcolor(1,str); return; }
     if(!strncmp(str,"Bookmark border col",strlen("Bookmark border col"))){n++;str+=22;getcolor(2,str);;return;} 
     if(!strncmp(str,"Bookmark BG col",strlen("Bookmark BG col"))){n++;str+=22;getcolor(3,str); return; }  
   }
//-------------------------//
   if(n>=11 && n<=24)//second panel   
   {
     if(!strncmp(str,"Display FastRun panel:",22)){ n++; str+=22; FASTRUN_ENA_2=strtoul(str,0,10); return; }
     if(!strncmp(str,"key activation style",20))  { n++; str+=22; ENA_FR_LONGPRESS_2=strtoul(str,0,10); return; }
     if(!strncmp(str,"Enable key (DEC)",16))      { n++; str+=22; FR_CALL_BUTTON_2=strtoul(str,0,10); return; }
     if(!strncmp(str,"Disable key (DEC)",17))     { n++; str+=22; FR_EXIT_BUTTON_2=strtoul(str,0,10); return; }
     if(!strncmp(str,"Position on screen",18))    { n++; str+=22; DRAW_MODE_2=strtoul(str,0,10); return; }
     if(!strncmp(str,"Offset",6))                 { n++; str+=22; BEGIN_2=strtoul(str,0,10); return; }
     if(!strncmp(str,"Panel lenght",12))          { n++; str+=22; LEN_2=strtoul(str,0,10); return; }
     if(!strncmp(str,"Pictures size",13))         { n++; str+=22; fr_pic_size_2=strtoul(str,0,10); return; }   
     if(!strncmp(str,"Panel border col",16))      { n++; str+=22; getcolor(0,str); return; }
     if(!strncmp(str,"Panel BG col",12))          { n++; str+=22; getcolor(1,str);return; }
     if(!strncmp(str,"Bookmark border col",strlen("Bookmark border col"))){ n++; str+=22; getcolor(2,str);return; } 
     if(!strncmp(str,"Bookmark BG col",strlen("Bookmark BG col"))){ n++; str+=22; getcolor(3,str);return; }  
   }
//--------------------------//
   if(n>=24)//idle panel
   {
     if(!strncmp(str,"Display Idle panel",strlen("Display Idle panel"))){ n++; str+=22; IDLE_ENA=strtoul(str,0,10); return; }
     if(!strncmp(str,"Key activation style",strlen("Key activation style"))){ n++; str+=22; ENA_IDLE_LONGPRESS=strtoul(str,0,10); return; }
     if(!strncmp(str,"Enable key (DEC)",16)) { n++; str+=22; IDLE_CALL_BUTTON=strtoul(str,0,10); return; }
     if(!strncmp(str,"Disable key (DEC)",17)){ n++; str+=22; IDLE_EXIT_BUTTON=strtoul(str,0,10); return; }
     if(!strncmp(str,"x",1))                 { n++; str+=22; x1=strtoul(str,0,10); return; }
     if(!strncmp(str,"y",1))                 { n++; str+=22; y1=strtoul(str,0,10); return; }
     if(!strncmp(str,"Show Idle name",strlen("Show Idle name"))){ n++; str+=22; IDLE_NAME_MODE=strtoul(str,0,10); return; }
     if(!strncmp(str,"Panel lenght",strlen("Panel lenght"))) { n++; str+=22; LEN_ID=strtoul(str,0,10); return; }
     if(!strncmp(str,"Show is not active",strlen("Show is not active"))){ n++; str+=22; SHOW_NOT_ATC_IDLE_P=strtoul(str,0,10); return; }   
     if(!strncmp(str,"Pictures size",13))    { n++; str+=22; idle_pic_size=strtoul(str,0,10); return; }   
     if(!strncmp(str,"Panel border col",16)) { n++; str+=22; getcolor(0,str); return; }
     if(!strncmp(str,"Font Size",strlen("Font Size"))){ n++; str+=22; TEXT_FONT=strtoul(str,0,10); return; }
     if(!strncmp(str,"Panel BG col",12))     { n++; str+=22; getcolor(1,str); return; }
     if(!strncmp(str,"Bookmark border col",strlen("Bookmark border col"))){ n++; str+=22; getcolor(2,str);return; } 
     if(!strncmp(str,"Bookmark BG col",strlen("Bookmark BG col"))) { n++; str+=22; getcolor(3,str);return; }  
   }
}
*/
char *find_eol(char *s)
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

void LoadParamsPanel()
{
  unsigned int err; 
  int f; 
  char *mem;
  int size; 
  int i,j=0; 
  char *str; 
  f = fopen( config, A_ReadOnly + A_BIN, P_READ, & err ); 
 if ( f==-1 ) 
  { 
    ShowMSG(1,(int)"Can't open fastrun.cfg!!!");
    fclose( f, & err ); 
    return; 
  }
  mem=malloc(10000); 
  str=malloc(256); 
  n=0;
  size=fread(f,mem,9999,&err); 
  i=0; 
  while (i<size) //пока не конец файлј 
  { 
    strcpy(str,empty_str); 
    j=0;
    while ((*(mem+i)!='\n') && (i<size) && (*(mem+i)!=';')){ *(str+j)=*(mem+i); j++; i++; }
    *(str+j)='\0';
  //  process(str);
    if(*(mem+i)==';'){ while (*(mem+i)!='\n'){ i++; }}   
    i++;       
  }
mfree(mem); 
mfree(str); 
fclose(f,&err); 
}
