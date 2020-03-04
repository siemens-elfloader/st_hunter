#include "io.h"
#include "ui.h"

int hex2int (char ch){
	if (ch>='A' && ch<='F') return ch-'A'+10;
	else return ch-'0';
}


int hex2int2 (char *str){
	return hex2int(str[0])*16+hex2int(str[1]);
}

color str2col (char *str){
	color clr;

	char R[2]; R[0]=str[0]; R[1]=str[1];
	clr.R=hex2int2 (R);
	char G[2]; G[0]=str[0+2]; G[1]=str[1+2];
	clr.G=hex2int2 (G);
	char B[2]; B[0]=str[0+2+2]; B[1]=str[1+2+2];
	clr.B=hex2int2 (B);
	char A[2]; A[0]=str[0+2+2+2]; A[1]=str[1+2+2+2];
	clr.A=hex2int2 (A);

	return clr;

}

struct ThemeList{
	char name[64];
	char value[64];
};

ThemeList tlist[64];
int ThemeItems=0;
int ParseTheme (unsigned char *fname){
  //unsigned char tmp[64];
  AFILE file;
  file.Open ((char*)fname);

  if (file.GetCount ()>0){
  while (file.GetCur ()<file.GetCount ()){
    if (file.GoToSymb ('.')==-1) return 1;
	if (file.CopyToSymb ((unsigned char*)tlist[ThemeItems].name, ':')==-1) return -1;;
    //RList[RIndex].freq=str2int (tmp);

	//cout<<tmp;
	  
	if (file.GoToSymb ('#')==-1) return -1;
    if (file.CopyToSymb ((unsigned char*)tlist[ThemeItems].value, ';')==-1) return -1;;
	
	//tlist[ThemeItems].clr=str2col ((char*)tmp);

	ThemeItems++;
    
  }

  file.Close ();
  }
  
  return 0;
}

char *theme[]={
  "background",
  "panel",
  "panel2",

  "header",
  "header2",

  "scrollbg",
  "scrollbg2",

  "scroll",
  "scroll2",

  "cursor",
  "cursor2",

  "font-header",
  "font-menu",
  "font-soft",
    
  "size_panel_up",
  "size_panel_down",
  "size_header"
};

char *GetValueByName (char *name){
  for (int i=0; i<ThemeItems; i++){
    if (!strcmp (name, tlist[i].name)){
      //ShowMSG (1, (int)name);
      return tlist[i].value;
    }
  }
  
  return NULL;
}


void LoadTheme (ConfigInfo *cfg){
  char buf[256];
  
  sprintf (buf, "%s\\%s", GetCurDir(), "theme.acs");
  if (GetFileSize (buf)){
    ParseTheme ((unsigned char*)buf);
  
  cfg->background=str2col (GetValueByName ("background"));
  
  cfg->panel=str2col (GetValueByName ("panel"));
  //iLog (cfg.panel.R); iLog (cfg.panel.G); iLog (cfg.panel.B);
  cfg->panel2=str2col (GetValueByName ("panel2"));
  //iLog (cfg.panel2.R); iLog (cfg.panel2.G); iLog (cfg.panel2.B);
  
  cfg->header=str2col (GetValueByName ("header"));
  cfg->header2=str2col (GetValueByName ("header2"));
  
  cfg->scrollbg=str2col (GetValueByName ("scrollbg"));
  cfg->scrollbg2=str2col (GetValueByName ("scrollbg2"));
  
  cfg->scroll=str2col (GetValueByName ("scroll"));
  cfg->scroll2=str2col (GetValueByName ("scroll2"));
  
  cfg->cursor=str2col (GetValueByName ("cursor"));
  cfg->cursor2=str2col (GetValueByName ("cursor2"));
  
  cfg->size_panel_up=str2int (GetValueByName ("size_panel_up"));
  cfg->size_panel_down=str2int (GetValueByName ("size_panel_down"));
  cfg->size_header=str2int (GetValueByName ("size_header"));
  }
  
}
