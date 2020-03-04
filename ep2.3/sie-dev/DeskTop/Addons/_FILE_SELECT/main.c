#include "..\..\..\inc\swilib.h"
#include "..\..\..\inc\pnglist.h"


//===============================================================================================

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}
#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}
//===============================================================================================



#include "../../_Dt/files.c"





const int minus11=-11;
unsigned short maincsm_name_body[140];
extern void kill_data(void *p, void (*func_p)(void *));


void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


int scr_w, scr_h, sf_h;
WSHDR *ews;
const char perc_t[]="%t",
		perc_d[]="%d",
		empty_str[]="",
		perc_s[]="%s",
		black[]={0,0,0,100},
		trans_black[]={0,0,0,50},
		red[]={0xff,0,0,100},
		grey[]={0x7f,0x7f,0x7f,100},
		white[]={0xff,0xff,0xff,100},
		white0[]={0xff,0xff,0xff,50};

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  void *data;
  int type, cur, cnt, max, tmp;
  IMGHDR *view;
}MAIN_GUI;


void DrwImg(IMGHDR *img, int x, int y, char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}


IMGHDR* CreateImgHdrByAnyFile (WSHDR * path,short width, short height, int rot){
  unsigned int err=0;
  int msz,uid;
  IMGHDR *tmpimg,*myimg=NULL;  
  WSHDR *ext;
  HObj  mypicObj;
  short pos;
  int len;
  if (!path)goto exit0;
  len=wstrlen(path); 
  pos= wstrrchr(path,len,'.'); 
  if (!pos)goto exit0;

  ext=AllocWS(len-pos);
  wstrcpybypos(ext,path,pos+1,len-pos);
  uid=GetExtUid_ws(ext);      
  FreeWS(ext);
  mypicObj=Obs_CreateObject(uid,0x2d,0x02,0x80A8,1,1,&err);    
    if (err)  return 0;
  err=Obs_SetInput_File(mypicObj,0,path);
    if (err)  goto exit1;  
  if (width!=0){ 
    unsigned short w=width;  
    unsigned short h=height;      
    /*
    err=Obs_GetInfo(mypicObj,0);    
      if (err)  goto exit1;  
    err=Obs_GetInputImageSize(mypicObj,&w,&h);    
      if (err)  goto exit1;  
      */
    err=Obs_SetOutputImageSize(mypicObj,w,h);
      if (err)  goto exit1;      
    if (rot){  
      err= Obs_SetRotation(mypicObj,rot);
        if (err)  goto exit1;      
    }
    err=Obs_SetScaling(mypicObj,5);
      if (err)  goto exit1;          
  }
  err=Obs_Start(mypicObj);
    if (err)  goto exit1;            
  err=Obs_Output_GetPictstruct(mypicObj,&tmpimg);
    if (err)  goto exit1;             
  msz=CalcBitmapSize (tmpimg->w,tmpimg->h,tmpimg->bpnum);
  myimg=malloc(sizeof(IMGHDR));
  myimg->w=tmpimg->w;
  myimg->h=tmpimg->h;
  myimg->bpnum=tmpimg->bpnum;
  myimg->bitmap=malloc(msz);
  memcpy(myimg->bitmap,tmpimg->bitmap,msz);
exit1:
		// ShowMSG(1, (int)"Жопа!!!");
  Obs_DestroyObject(mypicObj);
exit0:  
  return myimg;
}





// MAIN_GUI *tempdata;

void create_preview(MAIN_GUI *data)
{
	MAIN_GUI *tempdata=malloc(sizeof(MAIN_GUI));
	memcpy(tempdata, data, sizeof(MAIN_GUI));
	FLIST *fl=FindFLISTtByN(tempdata->cur);
	IMGHDR *si;//, *tsi;
	
	// char *s=(char*)&fl->name[strlen(fl->name)-3];
	// if( (((s[0]=='P')||(s[0]=='p')) && ((s[1]=='N')||(s[1]=='n')) && ((s[2]=='G')||(s[2]=='g'))) ||
		// (((s[0]=='J')||(s[0]=='j')) && ((s[1]=='P')||(s[1]=='p')) && ((s[2]=='G')||(s[2]=='g'))) ||
		// (((s[0]=='G')||(s[0]=='g')) && ((s[1]=='I')||(s[1]=='i')) && ((s[2]=='F')||(s[2]=='f'))) )
	{
		//mfree(si->bitmap);
		int len=strlen(fl->fullname);
		char *tmp=malloc(len);
		//zeromem(tmp, len);
		memcpy(tmp, fl->fullname, len);
		for(int i=0; i<len; i++)
			if((tmp[i]>='A')&&(tmp[i]<='Z')) tmp[i]=tmp[i]-'A'+'a';
			else if((tmp[i]>='А')&&(tmp[i]<='Я')) tmp[i]=tmp[i]-'Я'+'я';
		//wsprintf(ws,perc_t,tmp);
		str_2ws(data->ws2,tmp,len);
		mfree(tmp);
		//str_2ws(data->ws2,fl->fullname,strlen(fl->fullname));
		si=CreateImgHdrByAnyFile(data->ws2, scr_w, scr_h/3, 0);
	}
	if(tempdata->tmp==data->cur)
	{
		data->view=si;
		REDRAW();
	}
	else
	if(si)
	{
		if(si->bitmap) mfree(si->bitmap);
		mfree(si);
	}
	mfree(tempdata);
}

void preview(MAIN_GUI *data)
{
 if(data->type)
 {
	FLIST *fl=FindFLISTtByN(data->cur);
	if(data->view)
	{
		if(data->view->bitmap) mfree(data->view->bitmap);
		mfree(data->view);
		data->view=NULL;
	}
	char *s=(char*)&fl->name[strlen(fl->name)-3];
	if(fl->is_folder==IS_FILE)
	if( (((s[0]=='P')||(s[0]=='p')) && ((s[1]=='N')||(s[1]=='n')) && ((s[2]=='G')||(s[2]=='g'))) ||
		(((s[0]=='J')||(s[0]=='j')) && ((s[1]=='P')||(s[1]=='p')) && ((s[2]=='G')||(s[2]=='g'))) ||
		(((s[0]=='G')||(s[0]=='g')) && ((s[1]=='I')||(s[1]=='i')) && ((s[2]=='F')||(s[2]=='f'))) )
	{
		data->tmp=data->cur;
		//create_preview(data);
		//tempdata=data;
		SUBPROC((void*)create_preview, data);
	}
 }
}

void OnRedraw(MAIN_GUI *data)
{
  DrawRectangle(0,YDISP,scr_w-1,scr_h-1,0, black, black);
	
	int len=strlen(header);
	str_2ws(data->ws1,header,len);
	DrawString(data->ws1,3,3+YDISP,scr_w-4,3+YDISP+sf_h,
	     FONT_SMALL,TEXT_ALIGNMIDDLE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
	
	FLIST *fl;//=FindFLISTtByN(data->cur);
	if(data->cur<data->max/2)
	{
		int i=data->cur;
		DrawRectangle(3,2*sf_h+YDISP+sf_h*i,scr_w-4,2*sf_h+YDISP+sf_h*(i+1),
				0,red,black);
		fl=FindFLISTtByN(0);
		i=0;
		while(fl&&(i<data->max))
		{
			fl=FindFLISTtByN(i);
			len=strlen(fl->name);
			str_2ws(data->ws1,fl->name,len);
			DrawString(data->ws1,3,2*sf_h+YDISP+sf_h*i,scr_w-4,2*sf_h+YDISP+sf_h*(i+1),
				FONT_SMALL,1,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
			i++;
			fl=FindFLISTtByN(i);
		}
	}
	else
	{
		int i=data->max/2;//data->cur;
		DrawRectangle(3,2*sf_h+YDISP+sf_h*i,scr_w-4,2*sf_h+YDISP+sf_h*(i+1),
				0,red,black);
		fl=FindFLISTtByN(data->cur-data->max/2);
		i=0;
		while(fl&&(i<data->max))
		{
			len=strlen(fl->name);
			str_2ws(data->ws1,fl->name,len);
			DrawString(data->ws1,3,2*sf_h+YDISP+sf_h*i,scr_w-4,2*sf_h+YDISP+sf_h*(i+1),
				FONT_SMALL,1,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
			i++;
			fl=FindFLISTtByN(i+data->cur-data->max/2);
		}
	}	

	if((data->type)&&(data->tmp==data->cur))
	  if(data->view)
	  {
		DrwImg(data->view, scr_w/2-data->view->w/2, scr_h*5/6-data->view->h/2,(char*)white,NULL);FILE_PROP wl;
		fl=FindFLISTtByN(data->cur);
		if(fl->is_folder==IS_FILE)
		{
			len=strlen(header);
			str_2ws(ews,header,len);
			len=strlen(fl->fullname);
			str_2ws(data->ws1,fl->fullname,len);
			zeromem(&wl, sizeof(wl));
			wl.type=0x4000;
			wl.filename=data->ws1;
			GetFileProp(&wl,data->ws1,ews);
		}
		wsprintf(data->ws1, "%d X %d", wl.width, wl.height);
		DrawString(data->ws1,0, scr_h*5/6-data->view->h/2-sf_h, scr_w-1, scr_h*5/6-data->view->h/2,
			FONT_SMALL,TEXT_ALIGNMIDDLE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
	  }
}


void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  scr_w=ScreenW();
  scr_h=ScreenH();
  sf_h=GetFontYSIZE(FONT_SMALL);
  if(data->type) data->max=(scr_h*2/3-YDISP-3*sf_h)/sf_h;
  else data->max=(scr_h-YDISP-2*sf_h)/sf_h;
  data->ws1=AllocWS(256);
  data->ws2=AllocWS(256);
  ews=AllocWS(256);
  data->gui.state=1;
}

void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  Free_FLIST();
  FreeWS(data->ws1);
  FreeWS(data->ws2);
  FreeWS(ews);
  data->gui.state=0;
  SUBPROC((void *)ElfKiller);
}

void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  DisableIDLETMR();
  data->gui.state=2;
}

void OnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  
  FLIST *fl;
  fl=FindFLISTtByN(data->cur);
  
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
    case RED_BUTTON:
      return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
	
    case RIGHT_SOFT:
		fl=FindFLISTtByN(0);
		if (fl) 
		{
		  if (fl->is_folder==IS_BACK)
		  {
			int len;
			if (strcmp(fl->fullname,"ROOT"))
			{
			  strncpy(header,fl->fullname,sizeof(header)-1);
			  len=strlen(fl->fullname);
			  header[len>sizeof(header)-1?sizeof(header)-1:len]=0;
			  data->cnt=FindFiles(fl->fullname,data->type);
			}
			else
			{
			  //int CreateRootMenu();
			  data->cnt=CreateRootMenu();
			}
			data->cur=0;
			//RefreshGUI();
			preview(data);
			return (-1);
		  }
		}
      return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
	  
    case LEFT_SOFT:
		if (fl)
		{
		  if (fl->is_folder!=IS_BACK)
		  {
			SavePath(data->data,fl);
			return (1);
		  }      
		}
	  return (-1);
	  
	  
    case ENTER_BUTTON:
		if (fl) 
		{
		  if (fl->is_folder==IS_FOLDER || fl->is_folder==IS_BACK)
		  {
			int len;
			if (strcmp(fl->fullname,"ROOT"))
			{
			  strncpy(header,fl->fullname,sizeof(header)-1);
			  len=strlen(fl->fullname);
			  header[len>sizeof(header)-1?sizeof(header)-1:len]=0;
			  data->cnt=FindFiles(fl->fullname,data->type);
			}
			else
			{
			  //int CreateRootMenu();
			  data->cnt=CreateRootMenu();
			}
			data->cur=0;
			//RefreshGUI();
		  }
		  else
		  {
			SavePath(data->data,fl);        
			return (1);
		  }
		}
			preview(data);
		return(-1);
      
    case LEFT_BUTTON:
    case '4':
      break;
    case RIGHT_BUTTON:
    case '6':
      break;
    case UP_BUTTON:
    case '2':
		data->cur--;
		if(data->cur<0) data->cur=data->cnt-1;
			preview(data);
      break;
    case DOWN_BUTTON:
    case '8':
		data->cur++;
		if(data->cur>=data->cnt) data->cur=0;
			preview(data);
      break;
      
    }
  }
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(msg->gbsmsg->submess)
    {
    case LEFT_BUTTON:
    case '4':
      break;
    case RIGHT_BUTTON:
    case '6':
      break;
    case UP_BUTTON:
    case '2':
		data->cur--;
		if(data->cur<0) data->cur=data->cnt-1;
			preview(data);
      break;
    case DOWN_BUTTON:
    case '8':
		data->cur++;
		if(data->cur>=data->cnt) data->cur=0;
			preview(data);
      break;
      
    }
  }
  DirectRedrawGUI();
  return (0);
  
}


int method8(void){return(0);}

int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)OnRedraw,	//Redraw
  (void *)OnCreate,	//Create
  (void *)OnClose,	//Close
  (void *)OnFocus,	//Focus
  (void *)OnUnfocus,	//Unfocus
  (void *)OnKey,	//OnKey
  0,
  (void *)kill_data, //method7,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};

RECT Canvas={0,0,0,0};


int main(char *exename, void *t)
{
  EDITCONTROL ec;
  char path[128];
  char *s, *rev=0;
  int c, len;
  int tt[2];
  memcpy(tt, t, 8);
  void *ed_gui=(void*)tt[0];
  int type=tt[1];
  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->data=ed_gui;
  main_gui->type=type;
  EDIT_ExtractFocusedControl(ed_gui,&ec);
  ws_2str(ec.pWS,path,127);
  s=path;
  while((c=*s++))
  {
    if (c=='\\' && *s!='\0') rev=s;  
  }
  if (!rev) main_gui->cnt=CreateRootMenu();
  else
  {
    *rev=0;
    strncpy(header,path,sizeof(header)-1);
    len=strlen(path);
    header[len>sizeof(header)-1?sizeof(header)-1:len]=0;
    main_gui->cnt=FindFiles(path,type);
  }
  
  
  
  
  LockSched();
  //MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  //zeromem(main_gui,sizeof(MAIN_GUI));
  Canvas.x2=ScreenW()-1;
  Canvas.y2=ScreenH()-1;
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
  UnlockSched();
}
