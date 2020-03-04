#include "..\inc\swilib.h"
#include "xtask_ipc.h"
#include "system.h"

int MHico;
int pos=4;
extern int const icon_big_y;
extern int const headline_y;
short hs[]={56,101,159,204};
void *menu_data;
char main_hdr_txt[]="";
const void * NGuiMeths[11];

unsigned int MAINGUI_ID=0, MAINCSM_ID=0;

SOFTKEY_DESC menu_sk[]= 
{
  {0x0018,0x0000,(int)""},
  {0x0001,0x0000,(int)""},  
  {0x003D,0x0000,(int)LGP_DOIT_PIC} 
};

char pass[9];
int pass_pos;
int window_menu=0;
int start;

SOFTKEYSTAB menu_skt ={menu_sk,0};

int forvard=0, pic_n;

GBSTMR tmr;

int animkey=1;

void Animations()
{
  //int maxpic;
if (window_menu==0)
{
  if ((anim_on) && animkey)
  {    
    if (anim_invert)
    {      
      if(forvard==0 && pic_n!=Items[pos].Anim) pic_n++;      
      if(forvard==1 && pic_n!=0) pic_n--;
      
      if(forvard==1 && pic_n==0) forvard=0;
      if(forvard==0 && pic_n==Items[pos].Anim) forvard=1;
    }else
    {
      //maxpic=Items[pos].Anim;
      pic_n++;
      if(pic_n >= Items[pos].Anim)
        pic_n = 0;
    }
  }else
  {
    pic_n=select_nom;
  }
}
}

void TableMenu()
{
  Animations();
  for (int i=0; i<12; i++){
    
   int x=3*m+2+(36*m+5+smesh_x)*(i%3);
   int y=31*m-2+(28*m+2+smesh_y)*(i/3)-podn;
   
   if (i!=pos) DrawImg(x, y, *Items[i].icon_inact);
   else{
     if (usePicFon){
       DrawImg (x-smesh_pic_x, y-smesh_pic_y, (int)PicFon);
       DrawImg (x, y, *Items[i].icon_inact);
     }
     else{
      if (m==1) DrawImg(x, y, Items[pos].icon_big[pic_n]);
      if (m==2) DrawImg(pos%3*(77)+1+smesh_x, hs[pos/3]-podn, Items[pos].icon_big[pic_n]);
   }
  }
  }
}

void BrewMenu(){
  int scr_w=ScreenW(),
      scr_h=ScreenH();
  
#define icon_w (GetImgWidth(*Items[0].icon_inact-11-m)+2)
#define icon_h (GetImgHeight(*Items[0].icon_inact-11-m)+2)  
  
  int ot=(scr_w-(6*icon_w+6))/2+1;
  int h = scr_h-2*icon_h-SoftkeyH();
  //const char color3[4]={100,100,100,100};
  int x,y;
  //int font;
  
 // if (m==1) font=12;
  //else      font=8;
  
  for(int i=0; i<12; i++){
 
    x=ot+1;
    y=h+1;
    
    if (i==pos) DrawRoundedFrame(ot,h,ot+icon_w,h+icon_h,3,3,0,color1,color2);    
    DrawImg(x, y, *Items[i].icon_inact-11-m);    
    if(i==5)
    {
      ot=(scr_w-(6*icon_w+6))/2+1;
      h+=icon_h+1;
    }else
      ot+=icon_w+1;
  }
  
  WSHDR *ws = AllocWS(15); 
  wsprintf(ws, per_t, Items[pos].lgp_id);
  //DrawString(ws,0, UNI_YDISP+(HeaderH()-GetFontYSIZE(FONT_MEDIUM))/2, ScreenW()-1, UNI_YDISP+(HeaderH()-GetFontYSIZE(FONT_MEDIUM))/2+GetFontYSIZE(FONT_MEDIUM), FONT_MEDIUM, 2, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
  DrawString(ws,0, headline_y, scr_w, headline_y+GetFontYSIZE(FONT_MEDIUM), FONT_MEDIUM, 2, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
  Animations();
  
  //DrawImg((ScreenW()-1-GetImgWidth(Items[pos].icon_big[pic_n]))/2,
          //UNI_YDISP+(HeaderH()/2)+(104-GetFontYSIZE(12)-HeaderH()-GetImgHeight(Items[pos].icon_big[pic_n]))/2+4, 
          
  //DrawImg((scr_w-GetImgWidth(Items[pos].icon_big[pic_n]))/2,
          //UNI_YDISP+HeaderH()+(y-UNI_YDISP-HeaderH()-GetImgHeight(Items[pos].icon_big[pic_n]))/2,
          //Items[pos].icon_big[pic_n]);
  DrawImg(//icon_big_x,
          (scr_w-GetImgWidth(Items[pos].icon_big[pic_n]))/2,
          icon_big_y, Items[pos].icon_big[pic_n]);
          
  
  FreeWS(ws);
}

void PrintPass()
{
  WSHDR *ws2 = AllocWS(256);
  int font;
  if (m==1) font=12;
  else      font=8;
  
  char const foncol[]={0x00,0x00,0x00,0x50};
  
  DrawRoundedFrame( 0, UNI_YDISP, ScreenW()-1, ScreenH()-1, 0, 0, 0,  NULL,foncol);

  DrawImg(ScreenW()/2 - GetImgWidth(179)/2, ypos, 179);
  wsprintf(ws2,per_t,"Enter password:");
  DrawString(ws2, pos1.x, pos1.y, pos1.x2, pos1.y2, font, 1, fontENcol, NULL);
  wsprintf(ws2,per_t,pass);
  DrawString(ws2, pos2.x, pos2.y, pos2.x2, pos2.y2, font, 2+64, fontPAcol, NULL);
  FreeWS(ws2);
}

void myRedrawGUI() {PendedRedrawGUI();}

void UpdateHeader(GUI* gui)
{
  if (Menu_style)
  {
    MHico=-1;
    ((int*)GetDataOfItemByID(menu_data, 2))[13]=(int)"";
  }
  else
  {
    MHico=*Items[pos].icon_inact-11-m;
    ((int*)GetDataOfItemByID(menu_data, 2))[13]=Items[pos].lgp_id;
  }

}

void (*OOnRedraw)(GUI *data);
void NOnRedraw(GUI *data)
{
  OOnRedraw(data);
  if (Menu_style)
    BrewMenu();
  else
    TableMenu(); 
  
  if (window_menu==1)
    PrintPass();
}

void MM_locret(void){}

int MM_onkey(GUI *data, GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  int mode=msg->gbsmsg->msg;

  if (mode==LONG_PRESS)
    animkey=0;
  else
    animkey=1;
  
 // §°§Ò§â§Ñ§Ò§à§ä§é§Ú§Ü §Ü§Ý§Ñ§Ó§Ú§ê§î §á§â§Ú §Ó§Ó§à§Õ§Ö §á§Ñ§ã§Ñ
 if (window_menu==1 && mode==KEY_DOWN)
 {
   switch(key)
   {
      case ENTER_BUTTON:
        if (*pass!='\0')
        {
          EnterAction();
          return(ret);
        } 
      case LEFT_SOFT:
        if (pass[0]=='\0')
        {
          if (prun)
          {
            //GBS_StopTimer(&tmr);
            return(1);
          }else
          {
            window_menu=0;
            //GBS_StopTimer(&tmr);
            pass_pos_cler();
            return(-1);
          }
        }
        if (pass_pos>0)
        {
          pass_pos--;
          pass[pass_pos]='\0';
          return(-1);
        } 
      case RIGHT_SOFT:
        if (prun)
        {
          //GBS_StopTimer(&tmr);
          return(1);
        }else
        {
          window_menu=0;
          pass_pos_cler();
          return(-1);
        }
   }
        if((key>='0') && (key<='9'))
        {
          if (pass_pos<=8)
          {
            pass[pass_pos]=key;
            pass_pos++;
            if (prov)
            {
              if (!strcmp(pass,passw) && *pass!='\0') //if the general password coincided
              {
                if (!prun)
                {
                  pass_pos_cler();
                  Actions();
                }else
                {
                  pass_pos_cler();
                  start=0;
                  ret=-1;
                }
                window_menu=0;
              }
              return(ret);
            }  
          }
        }
 }
  
  if (((mode==KEY_DOWN)||(mode==LONG_PRESS)) && window_menu==0)
  {   
    pic_n=0;

    if (!Menu_style)
    {
      if(pos==0  && (key==LEFT_BUTTON || key==UP_BUTTON))     {pos=11;goto end;}
      if(pos==11 && (key==RIGHT_BUTTON || key==DOWN_BUTTON))  {pos=0;goto end;}
      if(pos==1  &&  key==UP_BUTTON)                          {pos=9;goto end;}
      if(pos==2  &&  key==UP_BUTTON)                          {pos=10;goto end;}
      if(pos==9  &&  key==DOWN_BUTTON)                        {pos=1;goto end;}
      if(pos==10 &&  key==DOWN_BUTTON)                        {pos=2;goto end;}    
    }else
    {
      if(pos==0  && key==LEFT_BUTTON)     {pos=11; goto end;}
      if(pos==11 && key==RIGHT_BUTTON)    {pos=0;  goto end;}
      
      if(pos==0  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=6;goto end;}
      if(pos==1  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=7;goto end;}
      if(pos==2  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=8;goto end;}
      if(pos==3  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=9;goto end;}
      if(pos==4  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=10;goto end;}
      if(pos==5  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=11;goto end;}
      
      if(pos==6  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=0;goto end;}
      if(pos==7 &&  (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=1;goto end;}
      if(pos==8  && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=2;goto end;}
      if(pos==9 &&  (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=3;goto end;}   
      if(pos==10 && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=4;goto end;}
      if(pos==11 && (key==DOWN_BUTTON || key==UP_BUTTON))       {pos=5;goto end;}         
    }
    
    
    switch(key)
    {
      case RIGHT_SOFT: return(1);
      case LEFT_SOFT:  ShowOptMenu(); goto end;
      case LEFT_BUTTON:  pos--; goto end;
      case RIGHT_BUTTON: pos++; goto end;
      case UP_BUTTON:    pos-=3; goto end;
      case DOWN_BUTTON:  pos+=3; goto end;
      case ENTER_BUTTON: 
      {
        EnterAction(); 
        return(ret);
      }
      case GREEN_BUTTON:
      {
        RunAction((char *)successed_config_filename); 
        return(0);
      }
      
      default:
      {
        if (key>='1' && key<='9'){pos=key-1-'0'; EnterAction();}
        if (key=='*')            {pos=9;         EnterAction();}
        if (key=='0')            {pos=10;        EnterAction();}
        if (key=='#')            {pos=11;        EnterAction();}
      }
      return(ret);
    }
  }
  end:
  UpdateHeader(data);
  RefreshGUI();
  return(0);
}

HEADER_DESC MHeader={0, 0, 0, 0, &MHico,(int)main_hdr_txt , LGP_NULL};

void MM_ghook(GUI *data, int cmd)
{
 static SOFTKEY_DESC sk[]=
  {
    {0x0018,0x0000,(int)"Options"},
    {0x0018,0x0000,(int)"Back"},
    {0x003D,0x0000,(int)LGP_DOIT_PIC}
  };

  if (cmd==TI_CMD_CREATE)
  {
    menu_data=data;
    memcpy(NGuiMeths, data->methods, 11*sizeof(void*));
    data->methods=(void*)NGuiMeths;
    OOnRedraw=(void(*)(GUI*))NGuiMeths[0];
    NGuiMeths[0]=(void*)NOnRedraw;
  }
  if (cmd==TI_CMD_DESTROY)
  {
   menu_data=NULL;

  }
  if (cmd==TI_CMD_REDRAW)
  {
   SetSoftKey(data, &sk[0], 0);
   SetSoftKey(data, &sk[1], 1);
   SetSoftKey(data, &sk[2], 2);
   if (anim_on && window_menu==0)
    GBS_StartTimerProc(&tmr, 216/TIME, myRedrawGUI);
/*   
   if (!anim_on && window_menu==1)
    GBS_StopTimer(&tmr);
   */
  }
  if (cmd==TI_CMD_UNFOCUS)
  {
    if (anim_on)
      GBS_StopTimer(&tmr);
  }
  if (cmd==TI_CMD_FOCUS)
  {
    if (anim_on)
      GBS_StartTimerProc(&tmr, 216/TIME, myRedrawGUI);
    
    UpdateHeader(data);
  }
 /*****************************************************************************************/
}

INPUTDIA_DESC MMenu=
{1, MM_onkey, MM_ghook, (void *)MM_locret, 0, &menu_skt,
  {0, 0, 0, 0}, 4, 100, 101, 0, 0, 0x40000000 };

int create_ed(void)
{
  void *eq;
  EDITCONTROL ec;
  void *ma=malloc_adr();
  PrepareEditControl(&ec);
  eq=AllocEQueue(ma, mfree_adr());
  ConstructEditControl(&ec, 0, 8, '\0', 1);
  AddEditControlToEditQend(eq, &ec, ma);
  patch_header(&MHeader);
  return CreateInputTextDialog(&MMenu, &MHeader, eq, 1, 0);
}

static void maincsm_oncreate(CSM_RAM *data)
{
  MAINGUI_ID=create_ed();
 
  if (anim_on)
    GBS_StartTimerProc(&tmr, 216/TIME, myRedrawGUI);
 
  if (!anim_on)
    pic_n=select_nom;
  else
    pic_n=0;
 
  if(!possave)
  {
    if (Menu_style)
      pos=0;
    else
      pos=4;
  }
  
  if ((prun==1) && !passw<=0)
  {
    window_menu=1;
    start=1;
  }else
    window_menu=0;
  
}

static void maincsm_onclose(CSM_RAM *csm)
{
 MAINCSM_ID=0;
 GBS_DelTimer(&tmr);
 pass_pos_cler();
}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
 {
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_GUI_DESTROYED)
  {
   if ((int)msg->data0==MAINGUI_ID)csm->csm.state=-3;
  }
 }return(1);
}

static unsigned short maincsm_name_body[140];
static const struct
{
 CSM_DESC maincsm;
 WSHDR maincsm_name;
}MAINCSM =
{{maincsm_onmessage,
maincsm_oncreate,
0, 0, 0, 0,
maincsm_onclose,
sizeof(MAIN_CSM), 1, &minus11 },
 {maincsm_name_body, NAMECSM_MAGIC1, NAMECSM_MAGIC2, 0x0, 139 }
};

static void UpdateCSMname() {wsprintf((WSHDR *)&MAINCSM.maincsm_name, per_t,"Main Menu");}

void CreateMMenu()
{
  if(MAINCSM_ID)
  {
    gipc.name_to=IPC_XTASK_NAME;
    gipc.name_from="EMenuD 2";
    gipc.data=(void *)MAINCSM_ID;
    GBS_SendMessage(MMI_CEPID, MSG_IPC, IPC_XTASK_SHOW_CSM, &gipc);
  }
  else
  {
    char dummy[sizeof(MAIN_CSM)];
    UpdateCSMname();
    CSM_RAM *save_cmpc;
    LockSched();
    save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
    CSM_root()->csm_q->current_msg_processing_csm=NULL;
    MAINCSM_ID = CreateCSM(&MAINCSM.maincsm, dummy, 0);
    CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
    UnlockSched();
  }
}


