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

//#include "main.h"

const char coord[]="0:\\shell\\coord.ini";
const char panel[]="0:\\shell\\panel.ini";
const char config[]="0:\\shell\\config.ini";
const char menucfg[]="0:\\shell\\menu.ini";

int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (int k=0; ; k++)
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
  crdcfg = fopen( coord, A_ReadOnly + A_BIN, P_READ, & err ); 
 if ( crdcfg==-1 ) 
 {  ShowMSG(1,(int)"Can't open coord.ini"); 
    fclose( crdcfg, & err ); 
 } 
     fread(crdcfg,mem,9999,&err); 
     
   for(int j=0,k=12;j<3;j++,k+=4)
   {
     name=strstr(mem,"Cap x")+k;
     crd[BattCap][j].x=atoi(name);
     
     name=strstr(mem,"Cap y")+k;
     crd[BattCap][j].y=atoi(name);
     
     name=strstr(mem,"ShowCap")+k;
     crd[BattCap][j].show=atoi(name);

     name=strstr(mem,"Volt x")+k;
     crd[BattVolt][j].x=atoi(name);
     
     name=strstr(mem,"Volt y")+k;
     crd[BattVolt][j].y=atoi(name);
     
     name=strstr(mem,"ShowVolt")+k;
     crd[BattVolt][j].show=atoi(name);

     name=strstr(mem,"Clock x")+k;
     crd[clock][j].y=atoi(name);
    
     name=strstr(mem,"Clock y")+k;
     crd[clock][j].y=atoi(name);
     
     name=strstr(mem,"ShowClock")+k;
     crd[clock][j].show=atoi(name);

     name=strstr(mem,"NetAdv x")+k;
     crd[NetAdv][j].y=atoi(name);
     
     name=strstr(mem,"NetAdv y")+k;
     crd[NetAdv][j].y=atoi(name);
     
     name=strstr(mem,"ShowNetAdv")+k;
     crd[NetAdv][j].show=atoi(name);
     
     name=strstr(mem,"NetDB x")+k;
     crd[NetDB][j].y=atoi(name);
     
     name=strstr(mem,"NetDB y")+k;
     crd[NetDB][j].y=atoi(name);
     
     name=strstr(mem,"ShowNetDB")+k;
     crd[NetDB][j].show=atoi(name);
     
     name=strstr(mem,"Dialogs x")+k;
     crd[Dialogs][j].x=atoi(name);
     
     name=strstr(mem,"Dialogs y")+k;
     crd[Dialogs][j].y=atoi(name);
     
     name=strstr(mem,"ShowDialogs")+k;
     crd[Dialogs][j].show=atoi(name);
     
     name=strstr(mem,"FreeRam x")+k;
     crd[FreeRam][j].x=atoi(name);
     
     name=strstr(mem,"FreeRam y")+k;
     crd[FreeRam][j].y=atoi(name);
     
     name=strstr(mem,"ShowFreeRam")+k;
     crd[FreeRam][j].show=atoi(name);
   }

fclose( crdcfg, & err ); 
}

void process(char *str, int number)//×èòàåì êîíôèã è ñîõğàíÿåì ïóòè
{ 
  int j=0; 
  int k=0;
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
  for(int i=0;i<=name[0].NumItems;i++)
    NAME[i]=name[i].filename;
}

void LoadParamsPanel()//Äëÿ ïàíåëè...
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  int size; 
  int i,j=0; 
  char *str; 
   plhandle = fopen( panel, A_ReadOnly + A_BIN, P_READ, & err ); 
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

  crdcfg = fopen( config, A_ReadOnly + A_BIN, P_READ, & err ); 
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
  int j=0; 
  int k=0;
  while (str[j]!='|') {mnu[number].name[k++]=str[j]; j++;}    
  j++;
  k=0;
  while (str[j]!='|') {mnu[number].file[k++]=str[j];j++;}    
  j++;
  k=0;
  while (str[j]!='|'){mnu[number].big[k++]=str[j]; j++;}
  j++;
  k=0;
  while (str[j]!='|'){mnu[number].small[k++]=str[j]; j++;}
}

void SaveNameMENU()//Ñîõğàíÿåì èìåíà äëÿ ïàíåëüêè:)
{
  for(int i=0;i<=mnu[0].NumItems;i++)
    NAME2[i]=mnu[i].name;
}

void LoadParamsMENU()//Äëÿ ïàíåëè...
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  int size; 
  int i,j=0; 
  char *str; 
  plhandle = fopen( menucfg, A_ReadOnly + A_BIN, P_READ, & err ); 
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
   //   strcpy(str,""); 
    //  sprintf(str,per_s,"");
      j=0; 
      while (((*(mem+i)!='\r'))&&(i<size)) {*(str+j)=*(mem+i); j++;i++;}    //÷èòàåì ñòğîêó èç ôàéëà 
      //îáğàáàòûâàåì åíòó ñòğî÷êó 
      processmenu(str,NumOfItems2);//Ñîõğàíÿåì èìåíà è ïóòè
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
//  LoadParamsCrd();
//  LoadParamsPanel();
//  LoadParamsConfig();
//  LoadParamsMENU();
  
  
/*     crd[BattCap][j].x=;
     crd[BattCap][j].y=atoi(name);
     crd[BattCap][j].show=atoi(name);
     
     crd[BattVolt][j].x=atoi(name);
     crd[BattVolt][j].y=atoi(name);
     crd[BattVolt][j].show=atoi(name);

     crd[clock][j].y=atoi(name);
     crd[clock][j].y=atoi(name);
     crd[clock][j].show=atoi(name);
     
     crd[NetAdv][j].y=atoi(name);
     crd[NetAdv][j].y=atoi(name);
     crd[NetAdv][j].show=atoi(name);
     
     crd[NetDB][j].y=atoi(name);
     crd[NetDB][j].y=atoi(name);
     crd[NetDB][j].show=atoi(name);
   
     crd[Dialogs][j].x=atoi(name);
     crd[Dialogs][j].y=atoi(name);
     crd[Dialogs][j].show=atoi(name);
     
     crd[FreeRam][j].x=atoi(name);
     crd[FreeRam][j].y=atoi(name);
     crd[FreeRam][j].show=atoi(name);*/
}

