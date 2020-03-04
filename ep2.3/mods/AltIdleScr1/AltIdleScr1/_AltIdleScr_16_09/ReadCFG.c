/*****************
ôàéë âèäà:       
Êîîğäèíàòû:
coord.ini
[ms coord] 
Cap x 2         
Cap y 120       

Ôàéë äëÿ ïàíåëè....panel.ini
Èìÿ|Ïóòü ê ôàéëó|Ïóòü ê êàğòèíêå|
mc|0:\zbin\mc\mc.elf|0:\shell\icons\app.png|
           
******************/ 

#include "main.h"

int j=0; 
int k=0;
int i;
int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (k=0; ; k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A) {ret=ret*10+attr[k]-0x30;} 
    else { if ( attr[k]=='-') {neg=-1;} 
           else {return(ret*neg);}
         }
  }
  
}
void LoadParamsCrd()
{ 
  unsigned int err; 
  int crdcfg; 
  char mem[9999]; 
  char *name;
  crdcfg = fopen( "0:\\coord.ini", A_ReadOnly + A_BIN, P_READ, & err ); 
 if ( crdcfg==-1 ) 
 {  ShowMSG(1,(int)"Can't open coord.ini"); 
    fclose( crdcfg, & err ); 
 } 
     fread(crdcfg,mem,9999,&err); 
     

 for(j=0;j<3;j++)
 {
     name=strstr(mem,"Cap x")+6;
     crd[BattCap][j].x=atoi(name);
     
     name=strstr(mem,"Cap y")+6;
     crd[BattCap][j].y=atoi(name);
     
     name=strstr(mem,"ShowCap")+8;
     crd[BattCap][j].show=atoi(name);

     name=strstr(mem,"Volt x")+7;
     crd[BattVolt][j].x=atoi(name);
     
     name=strstr(mem,"Volt y")+7;
     crd[BattVolt][j].y=atoi(name);
     
     name=strstr(mem,"ShowVolt")+9;
     crd[BattVolt][j].show=atoi(name);

     name=strstr(mem,"Clock x")+8;
     crd[clock][j].y=atoi(name);
    
     name=strstr(mem,"Clock y")+8;
     crd[clock][j].y=atoi(name);
     
     name=strstr(mem,"ShowClock")+10;
     crd[clock][j].show=atoi(name);

     name=strstr(mem,"NetAdv x")+9;
     crd[NetAdv][j].y=atoi(name);
     
     name=strstr(mem,"NetAdv y")+9;
     crd[NetAdv][j].y=atoi(name);
     
     name=strstr(mem,"ShowNetAdv")+11;
     crd[NetAdv][j].show=atoi(name);
     
     name=strstr(mem,"NetDB x")+8;
     crd[NetDB][j].y=atoi(name);
     
     name=strstr(mem,"NetDB y")+8;
     crd[NetDB][j].y=atoi(name);
     
     name=strstr(mem,"ShowNetDB")+10;
     crd[NetDB][j].show=atoi(name);
     
     name=strstr(mem,"Dialogs x")+10;
     crd[Dialogs][j].x=atoi(name);
     
     name=strstr(mem,"Dialogs y")+10;
     crd[Dialogs][j].y=atoi(name);
     
     name=strstr(mem,"ShowDialogs")+12;
     crd[Dialogs][j].show=atoi(name);
     
     name=strstr(mem,"FreeRam x")+10;
     crd[FreeRam][j].x=atoi(name);
     
     name=strstr(mem,"FreeRam y")+10;
     crd[FreeRam][j].y=atoi(name);
     
     name=strstr(mem,"ShowFreeRam")+12;
     crd[FreeRam][j].show=atoi(name);
    }

fclose( crdcfg, & err ); 
}

void process(char *str, int number)//×èòàåì êîíôèã è ñîõğàíÿåì ïóòè
{ 
  j=0; 
  k=0;
  while (str[j]!='|') {name[number].filename[k++]=str[j]; j++;}    
  j++;
  k=0;
  while (str[j]!='|') {name[number].file[k++]=str[j];j++;}    
  j++;
  k=0;
  while (str[j]!='|') 
  {
    name[number].pic[k++]=str[j]; 
    j++;
  }
  //(str[j]!='\r') ğàáîòàåò òîëüêî äëÿ äâóõ ñòğîê ïî÷åìó òî:(
  //êòî ğàçáåğåòñÿ òîìó ïğåìèÿ :D
}

void SaveNamePanel()//Ñîõğàíÿåì èìåíà äëÿ ïàíåëüêè:)
{
  for(i=0;i<=name[0].NumItems;i++)
    NAME[i]=name[i].filename;
}

void LoadParamsPanel()//Äëÿ ïàíåëè...
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  int size; 
  j=0; 
  char *str; 
   plhandle = fopen( "0:\\panel.ini", A_ReadOnly + A_BIN, P_READ, & err ); 
 if ( plhandle==-1 ) 
  { ShowMSG(1,(int)"Can't open panel.ini");
    fclose( plhandle, & err ); 
    return; 
  }
  mem=malloc(10000); 
  str=malloc(256); 
 if ((mem!=0)&&(str!=0)) 
  { 
    size=fread(plhandle,mem,9999,&err); 
    i=0; 
    while (i<size) //ïîêà íå êîíåö ôàéëÀ 
    { 
      strcpy(str,""); 
      j=0; 
      while (((*(mem+i)!='\r'))&&(i<size)) {*(str+j)=*(mem+i); j++;i++;}    //÷èòàåì ñòğîêó èç ôàéëà 
      //îáğàáàòûâàåì åíòó ñòğî÷êó 
      process(str,NumOfItems);//Ñîõğàíÿåì èìåíà è ïóòè
      i++; 
      NumOfItems++;
      name[0].NumItems++;
    }
  }
mfree(mem); 
mfree(str); 
fclose( plhandle, & err ); 
}

void LoadParamsConfig()
{ 
  unsigned int err; 
  int crdcfg; 
  char mem[9999]; 
  char *name;

  crdcfg = fopen( "0:\\config.ini", A_ReadOnly + A_BIN, P_READ, & err ); 
 if ( crdcfg==-1 ) 
 {  ShowMSG(1,(int)"Can't open config.ini"); 
    fclose( crdcfg, & err ); 
 } 
     fread(crdcfg,mem,9999,&err); 
     
     name=strstr(mem,"cursor_speed")+13;
     speed_cur=atoi(name);   
     
     name=strstr(mem,"cursor_img")+11;
     typecur=atoi(name);
fclose( crdcfg, & err ); 
}
void SaveParamsConfig()//Â ïğîåêòå...
{ 
  unsigned int err; 
  int cfg; 
  char mem[9999]; 
  char *name;
  cfg = fopen( "0:\\coor2.ini", A_ReadWrite + A_Create + A_Append, P_READ, & err ); 
 if ( cfg==-1 ) 
 {  ShowMSG(1,(int)"Can't open coor2.ini"); 
    fclose( cfg, & err ); 
 } 
     char *s=malloc(5);
     name=strstr(mem,"Clock x")+8;
     lseek(cfg,(int)name, S_SET, &err, &err);
     sprintf(s,per_s,crd[clock][0].x);
     fwrite(cfg,s,0x2,&err); 
  
  /* name=strstr(mem,"Clock y")+8;
     lseek(cfg, pos, S_SET, &err, &err);
     fwrite(cfg,crd[clock].y,2,&err); 
*/
//mfree(s);
fclose( cfg, & err ); 
}
  

void processmenu(char *str, int number)//×èòàåì êîíôèã è ñîõğàíÿåì ïóòè
{ 
  j=0; 
  k=0;
  while (str[j]!='|') {mnu[number].name[k++]=str[j]; j++;}    
  j++;
  k=0;
  while (str[j]!='|') {mnu[number].file[k++]=str[j];j++;}    
  j++;
  k=0;
  while (str[j]!='|'){mnu[number].big[k++]=str[j]; j++;}
  j++;
  k=0;
  while (str[j]!='\r'){mnu[number].small[k++]=str[j]; j++;}
}

void SaveNameMENU()//Ñîõğàíÿåì èìåíà äëÿ ïàíåëüêè:)
{
  for(i=0;i<=mnu[0].NumItems;i++)
    NAME2[i]=name[i].filename;
}

void LoadParamsMENU()//Äëÿ ïàíåëè...
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  int size; 
   j=0; 
  char *str; 
   plhandle = fopen( "0:\\menu.ini", A_ReadOnly + A_BIN, P_READ, & err ); 
 if ( plhandle==-1 ) 
  { ShowMSG(1,(int)"Can't open menu.ini");
    fclose( plhandle, & err ); 
    return; 
  }
  mem=malloc(10000); 
  str=malloc(256); 
 if ((mem!=0)&&(str!=0)) 
  { 
    size=fread(plhandle,mem,9999,&err); 
    i=0; 
    while (i<size) //ïîêà íå êîíåö ôàéëÀ 
    { 
      strcpy(str,""); 
      j=0; 
      while (((*(mem+i)!='\r'))&&(i<size)) {*(str+j)=*(mem+i); j++;i++;}    //÷èòàåì ñòğîêó èç ôàéëà 
      //îáğàáàòûâàåì åíòó ñòğî÷êó 
      process(str,NumOfItems2);//Ñîõğàíÿåì èìåíà è ïóòè
      i++; 
      NumOfItems2++;
      mnu[0].NumItems++;
    }
  }
mfree(mem); 
mfree(str); 
fclose( plhandle, & err ); 
}

void LoadAllConfig()
{
  LoadParamsCrd();
  LoadParamsPanel();
  LoadParamsConfig();
  //LoadParamsMENU();
}

