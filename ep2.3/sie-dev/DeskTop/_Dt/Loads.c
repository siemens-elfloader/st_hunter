

// void logtext(char *text)
// {
// int f;
// unsigned int err;
// char *mes=malloc(256);
// f = fopen("0:\\log.txt",A_ReadWrite+A_Create+A_Append,P_READ+P_WRITE, & err ); 
//  if ( f==-1 ){ShowMSG(1,(int)"Can't open file!");return; }else{
//     sprintf(mes,"%s\n",text);
//     fwrite(f,mes,strlen(mes),&err); 
//  fclose(f,&err);
//  }
//  mfree(mes);
// }


char defcurmap[]={
	0x80,0xC0,0xA0,0x90,0x88,0xFC,0x8,0x4,
	0xFF,0,0,0 ,0,0,0,0
};

IMGHDR defcur={
	8,8,1,
	defcurmap
};

// typedef struct
// {
  // void *next;
  // int is_folder;
  // char *fullname;
  // char *name;
// }FLIST;

// volatile FLIST *fltop;
int FindDti(char *str, int type);

// void loadplg()
// {
//   DT_ICON *TI=Dt.first;
//   //char tmp[4]={'P','L','U','G'};
// //   for(int i=0; i<Dt.icon_cnt; i++)
// //   {
// // 	PLG_P *TP=malloc(sizeof(PLG_P));
// // 	if(((DT_ICON*)TI->next)->d==0xFF)
// // 	{
// // 	  //TP->sig=tmp;
// // 	  //tmp[3]=i;
// // 	  //TP->ic=TI;
// // 	  wsprintf(ews,perc_t,((DT_ICON*)TI->next)->fl);
// // 	  ExecuteFile(ews,NULL,TI);
// // 	}
// // 	if(i<Dt.icon_cnt-1) TI=TI->next;
// //   }
// 	do
// 	{
// 		if(((DT_ICON*)TI)->d==0xFF)
// 		{
// 		  PLG_P *TP=malloc(sizeof(PLG_P));
// 		  TP->dt=&Dt;//&PLG_funcs;
// 		  TP->ic=TI;
// 		  wsprintf(ews,perc_t,((DT_ICON*)TI)->fl);
// 		  ExecuteFile(ews,NULL,TP);
// 		}
// 	}while(TI=TI->next);
// }

/*
char* mystrstr(char *s1, char *s2)
{
	if(!s1 || !s2) return 0;
	int l1=strlen(s1);
	int l2=strlen(s2);
	if(!l1 || !l2 || (l2>l1)) return 0;
	for(int i= 0; i<l1-l2; i++ )
	{
		if(!strncmp(s1+i, s2, l2)) return s1+i;
	}
	return 0;
}
*/

void saveicon(DT_ICON *TI)
{
  char txt[128],*s,*ts;
  int f;
  unsigned int ul, sz;
  zeromem(txt,128);
  s=translit(TI->nm);
  sprintf(txt,"%sicons\\%s.dti",Dt.rootadr,s);
  mfree(s); s=NULL;
  if ((f=fopen(txt,A_WriteOnly+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
  {
	s=malloc(512);
	zeromem(s,512);
	ts=s;
	memcpy(s,"DTI",3);
	// if((int)TI->onEnter==(int)&Enter_icon) s[3]='F'; else
	// if((int)TI->onEnter==(int)&Enter_entry) s[3]='E'; else
	// if((int)TI->onEnter==(int)&Enter_shortcut) s[3]='S'; else
	// if((int)TI->onEnter==(int)&Enter_mc) s[3]='m'; else
	
	// if((int)TI->onEnter==(int)&Enter_clock_icon) s[3]='C'; else
	// if((int)TI->onEnter==(int)&Enter_akk_icon) s[3]='B'; else
	// if((int)TI->onEnter==(int)&Enter_net_icon) s[3]='N';
	// else s[3]='P';
	s+=3;
	*(s++)=TI->type;
	*(s++)='|';
	
	*(s++)='p';
	*(s++)='=';
	sz=TI->p.x;
	*(s++)=sz/100+'0';
	sz-=((*(s-1)-'0')*100);
	*(s++)=sz/10+'0';
	sz-=((*(s-1)-'0')*10);
	*(s++)=sz+'0';
	*(s++)=',';
	sz=TI->p.y;
	*(s++)=sz/100+'0';
	sz-=((*(s-1)-'0')*100);
	*(s++)=sz/10+'0';
	sz-=((*(s-1)-'0')*10);
	*(s++)=sz+'0';
	*(s++)='|';
	
	*(s++)='s';
	*(s++)='=';
	sz=TI->p.x2-TI->p.x;
	*(s++)=sz/100+'0';
	sz-=((*(s-1)-'0')*100);
	*(s++)=sz/10+'0';
	sz-=((*(s-1)-'0')*10);
	*(s++)=sz+'0';
	*(s++)=',';
	sz=TI->p.y2-TI->p.y;
	*(s++)=sz/100+'0';
	sz-=((*(s-1)-'0')*100);
	*(s++)=sz/10+'0';
	sz-=((*(s-1)-'0')*10);
	*(s++)=sz+'0';
	*(s++)='|';
	
	if(TI->nm)
	{
	  sz=strlen(TI->nm);
	  if(sz)
	  {
		*(s++)='n';
		*(s++)='=';
		memcpy(s,TI->nm,sz);
		s+=sz;
		*(s++)='|';
	  }
	}
	
	if(TI->fl)
	{
	  sz=strlen(TI->fl);
	  if(sz)
	  {
		*(s++)='f';
		*(s++)='=';
		//=============Ояебу========
		if(strstr(TI->fl,Dt.rootadr)!=NULL)
		{
		  sz=strlen(TI->fl+strlen(Dt.rootadr)-1);
		  memcpy(s,TI->fl+strlen(Dt.rootadr)-1,sz);
		}else  //================и вот до сюда :)
		memcpy(s,TI->fl,sz);
		s+=sz;
		*(s++)='|';
	  }
	}
	
	if(TI->img_fl)
	{
	  sz=strlen(TI->img_fl);
	  if(sz)
	  {
		*(s++)='i';
		*(s++)='=';
		//===========Ояебу=======
		if(strstr(TI->img_fl,Dt.rootadr)!=NULL)
		{
		  sz=strlen(TI->img_fl+strlen(Dt.rootadr)-1);
		  memcpy(s,TI->img_fl+strlen(Dt.rootadr)-1,sz);
		}else //================и вот до сюда :)
		memcpy(s,TI->img_fl,sz);
		s+=sz;
		*(s++)='|';
	  }
	}
	if(TI->alfa!=100)
	{
	  *(s++)='a';
	  *(s++)='=';
	  sz=TI->alfa;
	  *(s++)=sz/100+'0';
	  sz-=((*(s-1)-'0')*100);
	  *(s++)=sz/10+'0';
	  sz-=((*(s-1)-'0')*10);
	  *(s++)=sz+'0';
	  *(s++)='|';
	}
	*s='\0';
	
	sz=strlen(ts);
	
	fwrite(f,ts,sz,&ul);
	mfree(ts); ts=NULL;
	fclose(f,&ul);
  }
  else ShowMSG(1,(int)lg.not_saved);
}


int abs0(int i)
{
	if(i<0) return -i;
	else return i;
}


int d_x(DT_ICON *TI, DT_ICON *TTI)
{return (TI->p.x-TTI->p.x);}

int d_y(DT_ICON *TI, DT_ICON *TTI)
{return (TI->p.y-TTI->p.y);}

void init_navi_key()
{
	DT_ICON *TI=Dt.first;
	// DT_ICON *TTI;
	// DT_ICON *TX[2],*TY[2];
	// int min_x[2], min_y[2], t;
	do
	{
		// RIGHT
		/* TI->right=(void*)TI;
		int t=scr_w-TI->p.x;
		for(int i=0; i<t; i++)
		{
			
		   DT_ICON *TTI=Dt.first;
		   do
		   {
		    if(TI!=TTI)
			 if(TTI->p.x==TI->p.x+i)
			 {
				TI->right=(void*)TTI;
				i=t;
				break;
			 }
		   }while(TTI=TTI->next);
		}
		// DOWN
		TI->down=(void*)TI;
		t=scr_h-TI->p.y;
		for(int i=0; i<t; i++)
		{
			
		   DT_ICON *TTI=Dt.first;
		   do
		   {
		    if(TI!=TTI)
			 if(TTI->p.y==TI->p.y+i)
			 {
				TI->down=(void*)TTI;
				i=t;
				break;
			 }
		   }while(TTI=TTI->next);
		}
		// LEFT
		TI->left=(void*)TI;
		t=TI->p.x;
		for(int i=0; i<t; i++)
		{
			
		   DT_ICON *TTI=Dt.first;
		   do
		   {
		    if(TI!=TTI)
			 if(TTI->p.x==TI->p.x-i)
			 {
				TI->left=(void*)TTI;
				i=t;
				break;
			 }
		   }while(TTI=TTI->next);
		}
		// UP
		TI->up=(void*)TI;
		t=TI->p.y;
		for(int i=0; i<t; i++)
		for(int j=0; j<t; j++)
		{
			
		   DT_ICON *TTI=Dt.first;
		   do
		   {
		    if(TI!=TTI)
			 if(TTI->p.y==TI->p.y-i)
			 {
				TI->up=(void*)TTI;
				i=t;
				break;
			 }
		   }while(TTI=TTI->next);
		} */
		
		
		
		
		
		
		
		if(TI->next) TI->right=(void*)TI->next;//TX[0];
		else TI->right=(void*)TI;
		if(TI->prev) TI->left=(void*)TI->prev;//TX[1];
		else TI->left=(void*)TI;
		TI->up=(void*)Dt.first;//TY[1];
		TI->down=(void*)Dt.last;//TY[0];
	}while(TI=TI->next);
}




void Refresh_Timer()
{
  DT_ICON *TI=Dt.first;
  do
  {
	if(TI->bytmr)
	{
	  TI->Refresh(0);
	}
  }while(TI=TI->next);
  SMART_REDRAW();
  GBS_StartTimerProc(&Dt.tmr, TMR_SEC*5, Refresh_Timer);
}

void Stop_Refresh_Timer()
{
  if(IsTimerProc(&Dt.tmr))
  {
	GBS_StopTimer(&Dt.tmr);
	GBS_DelTimer(&Dt.tmr);
  }
}

void Start_Refresh_Timer()
{
  Stop_Refresh_Timer();
  Refresh_Timer();
//   GBS_StartTimerProc(&Dt.tmr, TMR_SEC*5, Refresh_Timer);
}

// void refill_nmap()
// {
   // DT_ICON *TI=Dt.first;
   // zeromem(Dt.ni.bitmap, Dt.ni.w*Dt.ni.h);
	// do
	// {
		// FillRectByNum(&Dt.ni, TI->p, TI->n);
	// }while(TI=TI->next);
// }


void fill_windows(int t) // t=1 only windows, t=2 only nmap, t=3 both
{
  if(!Dt.pglist.show || !Dt.pglist.first) return;
  PGUI_methods *TPG=Dt.pglist.first;
  do
  {
// 	if(t&1)
// 	{
// 	  //TPG->img=resample(TI->img, TI->p.x2-TI->p.x, TI->p.y2-TI->p.y, 0, 1);
// 	  DrwImg2Img(TPG->w, TPG->wp, &Dt.i, 100);
// 	}
	if(t&2) FillRectByNum(&Dt.ni, TPG->wp, TPG->n);
  }while(TPG=TPG->next);
}

void fill_icons(int t) // t=1 only icons, t=2 only nmap, t=3 both (+4 REDRAW)
{
  // IMGHDR *t_img;
  
  if(t&1) memcpy(Dt.i.bitmap, mfn->bitmap, Dt.i.w*Dt.i.h*2);// zeromem(Dt.i.bitmap, Dt.i.w*Dt.i.h*4);
  if(t&2) zeromem(Dt.ni.bitmap, Dt.ni.w*Dt.ni.h);
  DT_ICON *TI=Dt.first;
  Dt.nn=0;
  do
  {
	if(t&1)
	{
	  if(!TI->dyn)
		if(TI->img_fl||TI->img)
		  if((TI->img_fl[1]==':')||TI->img)
		  {
			if(!TI->img)
			{
			  #if NEWSGOLD
			  TI->img=CreateIMGHDRFromPngFile(TI->img_fl, 3);
			  #else
			  TI->img=Dt.ef->create_imghdr(TI->img_fl, 3);
			  #endif
			  TI->pp.x=TI->img->w;
			  TI->pp.y=TI->img->h;
			}
			TI->img=resample(TI->img, TI->p.x2-TI->p.x, TI->p.y2-TI->p.y, 0, 1);
			DrwImg2Img(TI->img, TI->p, &Dt.i, TI->alfa/* , mfn */);
		  }
	}
	if(t&4)
	{
	  Dt.nn++;
	  SMART_REDRAW();//REDRAW();
	}
	if(t&2) FillRectByNum(&Dt.ni, TI->p, TI->n);
  }while(TI=TI->next);
  fill_windows(t);
  Dt.nn=Dt.cnt+1;
}


void del_icon(DT_ICON *TI)
{
	if(TI->nm){ mfree(TI->nm);TI->nm=NULL;}
	if(TI->fl){ mfree(TI->fl);TI->fl=NULL;}
	if(TI->img_fl){ mfree(TI->img_fl);TI->img_fl=NULL;}
	if(TI->inf){ mfree(TI->inf);TI->inf=NULL;}
	if(TI->prev) ((DT_ICON*)TI->prev)->next=TI->next;
	else Dt.first=TI->next;
	if(TI->next) ((DT_ICON*)TI->next)->prev=TI->prev;
	else Dt.last=TI->prev;
	if(TI->d)
	{
		mfree(TI);
		TI=NULL;
	}
	Dt.first->prev=NULL;
	Dt.last->next=NULL;
	Dt.cnt--;
	fill_icons(3);
}

void add_icon(DT_ICON *TI)
{
		DT_ICON *TTI=Dt.last;
		Dt.last=TI;
		Dt.last->prev=TTI;
		TTI->next=Dt.last;
	Dt.first->prev=NULL;
	Dt.last->next=NULL;
	Dt.cnt++;
}

// char *stt;

void initic()
{
  State=DT_WAIT;
  DT_ICON *TI;
  char *s, *stt, nsz;
  int f, t, m, i=0, n=0;
  unsigned int ul, cfg_sz;
  //int len=strlen(Dt.rootadr);
  char *tx=malloc(256);
  zeromem(tx, 256);
  sprintf(tx,"%sicons\\",Dt.rootadr);
  // 	memcpy(tx, Dt.rootadr, len);
  // 	strcpy(tx+len,"icons\\");
  Dt.cnt=FindDti(tx, 1);//(char*)ICON_DT, 1);
  mfree(tx);
  Dt.n=0;
  State=DT_MAIN;
  LockSched();
  FLIST *fl=(FLIST *)fltop;
  UnlockSched();
  while(fl)
  {
	if ((f=fopen(fl->fullname,A_ReadOnly,P_READ,&ul))!=-1)
	{
	  cfg_sz=lseek(f,0,S_END,&ul,&ul);
	  lseek(f,0,S_SET,&ul,&ul);
	  s=malloc(cfg_sz+1);
	  fread(f,s,cfg_sz,&ul);
	  fclose(f,&ul);
	  stt=s;
	  s[cfg_sz]='\0';
	  if((s[0]=='D')&&(s[1]=='T')&&(s[2]=='I'))
	  {
		switch(s[3])
		{
		  case DTI_CLOCK:
			TI=&clock_icon; //часы
			break;
		  case DTI_BAT:
			TI=&akk_icon; //аккум
			break;
		  case DTI_NET:
			TI=&net_icon; //сеть
			break;
			// case DTI_MENU:
			// break;
		  case DTI_SHORT:
			TI=malloc(sizeof(DT_ICON));
			memcpy(TI, &shortcut_icon, sizeof(DT_ICON));
			break;
		  case DTI_ENTRY:
			TI=malloc(sizeof(DT_ICON));
			memcpy(TI, &entry_icon, sizeof(DT_ICON));
			break;
		  case DTI_MC:
			TI=malloc(sizeof(DT_ICON));
			memcpy(TI, &mc_icon, sizeof(DT_ICON));
			break;
		  case DTI_FILE:
			TI=malloc(sizeof(DT_ICON));
			memcpy(TI, &_icon, sizeof(DT_ICON));
			break;
		  case DTI_PLAGIN:
			TI=malloc(sizeof(DT_ICON));
			memcpy(TI, &dt_null, sizeof(DT_ICON));
			TI->d=0xFF;
			TI->type='P';
			break;
		  default: goto noth;
		}
		TI->type=s[3];
		while(*s!='\0')
		{
		  if(*s=='|')
			switch(*(++s))
			{
			  case 'p': // координаты
				if(*(++s)!='=') break;
				s++;
				i=0;
				m=0;
				if(s[i]=='-'){m=1;i++;}
				t=0;
				while((s[i]<='9')&&(s[i]>='0')){t=t*10+s[i]-'0'; i++;}
				if(m) TI->p.x=scr_w-t;
				else TI->p.x=t;
				s++;
				
				m=0;
				if(s[i]=='-'){m=1;i++;}
				t=0;
				while((s[i]<='9')&&(s[i]>='0')){t=t*10+s[i]-'0'; i++;}
				if(m) TI->p.y=scr_h-t;
				else TI->p.y=t;
				break;
			  case 's': // размер
				if(*(++s)!='=') break;
				s++;
				i=0;
				t=0;
				while((s[i]<='9')&&(s[i]>='0')){t=t*10+s[i]-'0'; i++;}
				TI->p.x2=TI->p.x+t;
				s++;
				
				t=0;
				while((s[i]<='9')&&(s[i]>='0')){t=t*10+s[i]-'0'; i++;}
				TI->p.y2=TI->p.y+t;
				break;
			  case 'a': // прозрачность
				if(*(++s)!='=') break;
				s++;
				i=0;
				t=0;
				while((s[i]<='9')&&(s[i]>='0')){t=t*10+s[i]-'0'; i++;}
				TI->alfa=t;
				break;
			  case 'f': // файл
				if(*(++s)!='=') break;
				s++;
				nsz=0;
				while((s[nsz]!='|')&&(s[nsz]!='\0')){nsz++;}
				if(nsz==0) break;
				if(s[0]=='\\')			//Ояебу
				{
				  int i_ne_ebet=0;
				  i_ne_ebet=strlen(Dt.rootadr)+nsz+1;
				  TI->fl=malloc(i_ne_ebet);//Вот это я хуйню загнул....
				  zeromem(TI->fl, i_ne_ebet);//Хз
				  memcpy(TI->fl, Dt.rootadr,strlen(Dt.rootadr));//Вначале втыкаем рутдир
				  memcpy(TI->fl+strlen(Dt.rootadr)-1,s,nsz);//а теперь дописываем то че осталось со слешером.
				  s+=nsz-1;//а это тебе виднее че за нах.... :)
				}else{
				  TI->fl=malloc(nsz+1);
				  zeromem(TI->fl, nsz+1);
				  memcpy(TI->fl, s, nsz);
				  s+=nsz-1;
				}
				break;
			  case 'n': // имя
				if(*(++s)!='=') break;
				s++;
				nsz=0;
				while((s[nsz]!='|')&&(s[nsz]!='\0')){nsz++;}
				if(nsz==0) break;
				TI->nm=malloc(nsz+1);
				zeromem(TI->nm, nsz+1);
				memcpy(TI->nm, s, nsz);
				s+=nsz-1;
				break;
			  case 'i': // картинка
				if(*(++s)!='=') break;
				s++;
				nsz=0;
				while((s[nsz]!='|')&&(s[nsz]!='\0')){nsz++;}
				if(s[0]=='\\')			//Ояебу
				{
				  int i_ne_ebet=0;
				  i_ne_ebet=strlen(Dt.rootadr)+nsz+1;
				  TI->img_fl=malloc(i_ne_ebet);//Вот это я хуйню загнул....
				  zeromem(TI->img_fl, i_ne_ebet);//Хз
				  memcpy(TI->img_fl, Dt.rootadr,strlen(Dt.rootadr));//Вначале втыкаем рутдир
				  memcpy(TI->img_fl+strlen(Dt.rootadr)-1,s,nsz);//а теперь дописываем то че осталось со слешером.
				  s+=nsz-1;//а это тебе виднее че за нах.... :)
				}else{
				  TI->img_fl=malloc(nsz+1);
				  zeromem(TI->img_fl, nsz+1);
				  memcpy(TI->img_fl, s, nsz);
				  s+=nsz-1;
				}
				break;
			}
			if(*s=='\0') break;
			s++;
		}
		
		if(TI->img_fl)
		  if(TI->img_fl[1]==':')
		  {
			// zeromem(t_img, sizeof(IMGHDR));
			// t_img=CreateIMGHDRFromPngFile(TI->img_fl, 3);
			// TI->p.x2=TI->p.x+GetImgWidth((int)TI->img_fl);//t_img->w;
			// TI->p.y2=TI->p.y+GetImgHeight((int)TI->img_fl);//t_img->h;
			// DrwImg2Img(t_img, TI->p, &Dt.i, TI->alfa);
			// if(t_img->bitmap) mfree(t_img->bitmap);
			// if(t_img) mfree(t_img);
		  }
		  n++;
		  TI->n=n;
		  TI->inf=malloc(256);
		  zeromem(TI->inf, 256);
		  if(!TI->nm)
		  {
			TI->nm=malloc(strlen(fl->name)-3);
			zeromem(TI->nm, strlen(fl->name)-3);
			memcpy(TI->nm, fl->name, strlen(fl->name)-4);
		  }
		  TI->pp.x=TI->p.x2-TI->p.x;
		  TI->pp.y=TI->p.y2-TI->p.y;
	  }
	  add_icon(TI);
	  noth:
	  mfree(stt);
	  //REDRAW();
	}
	fl=fl->next;
  }
  fill_icons(3+4);
  gipc.name_to=IPC_DT_NAME;
  gipc.name_from=IPC_DT_NAME;
  gipc.data=NULL;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_LOAD_PLG,&gipc);
  Free_FLIST();
  //   if(!CUR_ENABLE) init_navi_key();
  State=DT_LPLG;
  SMART_REDRAW();//REDRAW();
  // loadplg();
}


/* 
void initic0()
{
  DT_ICON *TI;
  char *s, nm[32], nsz;
  int f, t, m, i=0;
  unsigned int ul, cfg_sz;
  if ((f=fopen(ICON_DT,A_ReadOnly,P_READ,&ul))!=-1)
  {
	cfg_sz=lseek(f,0,S_END,&ul,&ul);
	lseek(f,0,S_SET,&ul,&ul);
	s=malloc(cfg_sz+1);
	fread(f,s,cfg_sz,&ul);
	while(i<cfg_sz)
	{
		zeromem(nm, 32);
		while((s[i]!='[')&&(i<cfg_sz)){i++;}
		if(i>=cfg_sz) break;
		i++;
		nsz=0;
		while(s[i+nsz]!=']'){nsz++;}
		memcpy(nm, s+i, nsz);
		i+=nsz;
		while((s[i]!='+')&&(i<cfg_sz)){i++;}
		if(i>=cfg_sz) break;
		if(!strncmp(s+i,"+ACT",4))
		{
			i+=4;
			if(!strncmp(s+i,"(std)=",6))
			{
				i+=6;
				if(!strncmp(s+i,"net",3)) TI=&net_icon; //сеть
				else if(!strncmp(s+i,"bat",3)) TI=&akk_icon; //аккум
				else if(!strncmp(s+i,"clock",5)) TI=&clock_icon; //часы
				else continue; //ничего, продолжение
				TI->inf=malloc(256);
				zeromem(TI->inf, 256);
				TI->nm=malloc(32);
				memcpy(TI->nm, nm, 32);
			}
			else if(!strncmp(s+i,"(entry)=",8))
			{
				i+=8;
				TI=malloc(sizeof(DT_ICON));
				memcpy(TI, &entry_icon, sizeof(DT_ICON));
				goto dofl;
			}
			else if(!strncmp(s+i,"(shortcut)=",11))
			{
				i+=11;
				TI=malloc(sizeof(DT_ICON));
				memcpy(TI, &shortcut_icon, sizeof(DT_ICON));
				goto dofl;
			}
			else if(!strncmp(s+i,"(mc)=",5))
			{
				i+=5;
				TI=malloc(sizeof(DT_ICON));
				memcpy(TI, &mc_icon, sizeof(DT_ICON));
				goto dofl;
			}
			else if(!strncmp(s+i,"(file)=",7))
			{
				i+=7;
				TI=malloc(sizeof(DT_ICON));
				memcpy(TI, &_icon, sizeof(DT_ICON));
			 dofl:
				TI->inf=malloc(256);
				zeromem(TI->inf, 256);
				goto dodt;
				// nsz=0;
				// while(s[i+nsz]!=';'){nsz++;}
				// TI->fl=malloc(nsz+1);
				// zeromem(TI->fl, nsz+1);
				// memcpy(TI->fl, s+i, nsz);
				// i+=nsz;
				// TI->nm=malloc(32);
				// memcpy(TI->nm, nm, 32);
			}
			else if(!strncmp(s+i,"(plugin)=",9))
			{
				i+=9;
				TI=malloc(sizeof(DT_ICON));
				memcpy(TI, &dt_null, sizeof(DT_ICON));
				TI->d=0xFF;
			 dodt:
				nsz=0;
				while(s[i+nsz]!=';'){nsz++;}
				TI->fl=malloc(nsz+1);
				zeromem(TI->fl, nsz+1);
				memcpy(TI->fl, s+i, nsz);
				i+=nsz;
				TI->nm=malloc(32);
				memcpy(TI->nm, nm, 32);
			}
			else continue;
		}
		else continue;
		
		while((s[i]!='+')&&(i<cfg_sz)){i++;}
		if(i>=cfg_sz) break;
		if(!strncmp(s+i,"+ICON=",6))
		{
			i+=6;
			nsz=0;
			while(s[i+nsz]!=';'){nsz++;}
			TI->img_fl=malloc(nsz+1);
			zeromem(TI->img_fl, nsz+1);
			memcpy(TI->img_fl, s+i, nsz);
			i+=nsz;
		}
		else goto dopos;
		
		while((s[i]!='+')&&(i<cfg_sz)){i++;}
		if(i>=cfg_sz) break;
		
	  dopos:
		if(!strncmp(s+i,"+POS=",5))
		{
			i+=5;
			//nsz=0;
			m=0;
			if(s[i]=='-'){m=1;i++;}
			t=0;
			while(s[i]!=','){t=t*10+s[i]-'0'; i++;}
			 if(m) TI->p.x=scr_w-t;
			 else TI->p.x=t;
			i++;
			
			m=0;
			if(s[i]=='-'){m=1;i++;}
			t=0;
			while(s[i]!=';'){t=t*10+s[i]-'0'; i++;}
			 if(m) TI->p.y=scr_h-t;
			 else TI->p.y=t;
			TI->p.x2=TI->p.x+GetImgWidth((int)TI->img_fl);
			TI->p.y2=TI->p.y+GetImgHeight((int)TI->img_fl);
		}
		
		while((s[i]!='+')&&(i<cfg_sz)){i++;}
		if(i>=cfg_sz) break;
		if(!strncmp(s+i,"+SIZE=",6))
		{
			i+=6;
			//nsz=0;
			t=0;
			while(s[i]!=','){t=t*10+s[i]-'0'; i++;}
			 TI->p.x2=TI->p.x+t;
			i++;
			
			t=0;
			while(s[i]!=';'){t=t*10+s[i]-'0'; i++;}
			 TI->p.y2=TI->p.y+t;
		}
		
		DT_ICON *TTI=Dt.first;
		Dt.first=TI;
		Dt.first->next=TTI;
		TTI->prev=Dt.first;
		
		// TI->next=Dt.icon;
		//Dt.icon_cnt++;
		// Dt.icon=TI;
		i++;
		REDRAW();
		
	}
	fclose(f,&ul);
	Dt.first->prev=NULL;
	Dt.last->next=NULL;
	SUBPROC((void*)loadplg);
  }
  mfree(s);
}
 */


////////////////////
////////////////////


// void Free_FLIST(void)
// {
  // LockSched();
  // FLIST *fl=(FLIST *)fltop;
  // fltop=0;
  // UnlockSched();
  // while(fl)
  // {
  // FLIST *fl_prev=fl;
  // fl=fl->next;
  // mfree(fl_prev);
  // }
// }

// enum TYPES {IS_BACK, IS_FOLDER, IS_FILE};

// FLIST *AddToFList(const char* full_name, const char *name, int is_folder)
// {
  // int l_fname;
  // FLIST *fl;
  // FLIST *fn=malloc(sizeof(FLIST)+(l_fname=strlen(full_name))+strlen(name)+2);
  // fn->fullname=(char *)fn+sizeof(FLIST);
  // fn->name=(char *)fn+sizeof(FLIST)+l_fname+1;
  // strcpy(fn->fullname,full_name);
  // strcpy(fn->name,name);
  // fn->is_folder=is_folder;
  // fn->next=0;
  // fl=(FLIST *)fltop;
  // if (fl)
  // {
  // FLIST *pr;
  // pr=(FLIST *)&fltop;
  // while(strcmp_nocase(fl->name,fn->name)<0)
  // {
  // pr=fl;
  // fl=fl->next;
  // if (!fl) break;
  // }
  // fn->next=fl;
  // pr->next=fn;
  // }
  // else
  // {
  // fltop=fn;
  // }
  // return (fn);
// }

int FindDti(char *str, int type)  // type == 0 SelectFolder, type == 1 SelectFile
{
  DIR_ENTRY de;
  unsigned int err;
  char *rev,*s,*d;
  int i, c, n=0;
  char path[256];
  char name[128];
  strcpy(path,str);
  Free_FLIST();
  s=path;
  d=name;
  rev=0;
  while((c=*s++))
  {
  *d++=c;
  if (c=='\\' && *s!='\0') rev=d;
  }
  if(rev==0) return 0;//AddToFList("ROOT",back,IS_BACK);
  else
  {
  *rev=0;
  //AddToFList(name,back,IS_BACK);
  }
  n++;
  i=strlen(path);
  path[i++]='*';
  path[i++]='.';
  path[i++]='d';
  path[i++]='t';
  path[i++]='i';
  path[i]='\0';
  if (FindFirstFile(&de,path,&err))
  {
  do
  {
  i=strlen(de.folder_name);
  strcpy(path,de.folder_name);
  path[i++]='\\';
  strcpy(path+i,de.file_name);
  if (de.file_attr&FA_DIRECTORY)
  {
  // i=strlen(path);
  // path[i++]='\\';
  // path[i]=0;
  // name[0]='\\';
  // strcpy(name+1,de.file_name);
  // AddToFList(path,name,IS_FOLDER);
  // n++;
  }
  else
  {
  i=strlen(path);
  if((path[i-1]!='i')||(path[i-2]!='t')||(path[i-3]!='d')||(path[i-4]!='.')){}
  else
  {
  //if (type!=0)
  //{
  AddToFList(path,de.file_name,IS_FILE);
  n++;
  //}
  }
  }
  }
  while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
  return n;
}




void dofn()
{
	// Load background:
	int len=strlen(BG_IMG);
	char *tmp=malloc(len);
	zeromem(tmp, len);
	strcpy(tmp, BG_IMG);
	for(int i=0; i<len; i++)
		if((tmp[i]>='A')&&(tmp[i]<='Z')) tmp[i]=tmp[i]-'A'+'a';
		else if((tmp[i]>='А')&&(tmp[i]<='Я')) tmp[i]=tmp[i]-'Я'+'я';
	char *s=(char*)&BG_IMG[len-3];
	if( ((s[0]=='P')||(s[0]=='p')) && ((s[1]=='N')||(s[1]=='n')) && ((s[2]=='G')||(s[2]=='g')) )
		mfn=CreateIMGHDRFromPngFile(tmp, 2);
	else
	{
		str_2ws(ws,tmp,len);//wsprintf(ws,perc_t,tmp);
		mfn=CreateImgHdrByAnyFile(ws, scr_w+1, scr_h+1, 0);
	}
	mfree(tmp);
// 	mfn=CreateImgHdrByAnyFile(ws, scr_w+1, scr_h+1, 0);
	if(!mfn)
	{
		mfn=malloc(sizeof(IMGHDR));
		mfn->w=scr_w+1;
		mfn->h=scr_h+1;
		mfn->bpnum=8;
		
		mfn->bitmap=malloc((scr_w+1)*(scr_h+1)*2);
		zeromem(mfn->bitmap, (scr_w+1)*(scr_h+1)*2);
		//for(int q=3; q<(scr_w+1)*(scr_h+1)*4; q+=4) mfn->bitmap[q]=0xFF;
	}
	if(!mfn->bitmap)
	{
		//mfn=malloc(sizeof(IMGHDR));
		mfn->w=scr_w+1;
		mfn->h=scr_h+1;
		mfn->bpnum=8;
		
		mfn->bitmap=malloc((scr_w+1)*(scr_h+1)*2);
		zeromem(mfn->bitmap, (scr_w+1)*(scr_h+1)*2);
		//for(int q=3; q<(scr_w+1)*(scr_h+1)*4; q+=4) mfn->bitmap[q]=0xFF;
	}
	for(int i=0; i<YDISP; i++)
	  for(int j=0; j<Dt.w; j++)
		((unsigned short*)mfn->bitmap)[i*Dt.w+j]=0xE000;
	Dt.fon=mfn;
	
// 	StoreXYWHtoRECT(&bgrc,x,y,mfn->w,mfn->h);
// 	SetPropTo_Obj5(&fonobj,&bgrc,0,mfn);
// 	SetColor(&fonobj,NULL,NULL);
	//DrawObject(&fonobj);
   ///////////////////////////

	// Load cursor:
	len=strlen(CUR_IMG);
	tmp=malloc(len);
	zeromem(tmp, len);
	strcpy(tmp, CUR_IMG);
	for(int i=0; i<len; i++)
		if((tmp[i]>='A')&&(tmp[i]<='Z')) tmp[i]=tmp[i]-'A'+'a';
		else if((tmp[i]>='А')&&(tmp[i]<='Я')) tmp[i]=tmp[i]-'Я'+'я';
	s=(char*)&CUR_IMG[len-3];
	if( ((s[0]=='P')||(s[0]=='p')) && ((s[1]=='N')||(s[1]=='n')) && ((s[2]=='G')||(s[2]=='g')) )
		cursor=CreateIMGHDRFromPngFile(tmp, 0);
	else
	{
		str_2ws(ws,tmp,len);//wsprintf(ws,perc_t,tmp);
		cursor=CreateImgHdrByAnyFile(ws, 0, 0, 0);
	}
		mfree(tmp);
	if(!cursor) cursor=&defcur;// State=DT_ERR;
	if(!cursor->bitmap) cursor=&defcur;
	Dt.cursor=cursor;
// 	StoreXYWHtoRECT(&currc,x,y,cursor->w,cursor->h);
// 	SetPropTo_Obj5(&curobj,&currc,0,cursor);
// 	SetColor(&curobj,NULL,NULL);
	////////////////////////////

	// Create draw-buffer IMGHDR
	Dt.i.w=scr_w+1;
	Dt.i.h=scr_h+1;
	Dt.i.bpnum=0x8;
	Dt.i.bitmap=malloc((scr_w+1)*(scr_h+1)*2);
	memcpy(Dt.i.bitmap, mfn->bitmap, (scr_w+1)*(scr_h+1)*2);

	StoreXYWHtoRECT(&bgrc,0,0,Dt.i.w,Dt.i.h);
	SetPropTo_Obj5(&fonobj,&bgrc,0,&Dt.i);
	SetColor(&fonobj,NULL,NULL);

	// Create icon-numbers map IMGHDR
	Dt.ni.w=scr_w+1;
	Dt.ni.h=scr_h+1;
	Dt.ni.bpnum=5;
	Dt.ni.bitmap=malloc((scr_w+1)*(scr_h+1));
	zeromem(Dt.ni.bitmap, (scr_w+1)*(scr_h+1));
	
	// If errors:
}


