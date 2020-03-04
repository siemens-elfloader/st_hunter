

int open_icon_subm();

int menup_run()
{
	Dt.act->onEnter();
	State=DT_MAIN;
	return 0;
}

#define mi	Dt.mi
#define mic	Dt.mic

int moveicon()
{
	memcpy(&mic,&Dt.act->p,sizeof(RECT));
	State=DT_MOVE;
	mi.x=Dt.act->p.x-cur.x;
	mi.y=Dt.act->p.y-cur.y;
	mi.x2=Dt.act->p.x2-cur.x;
	mi.y2=Dt.act->p.y2-cur.y;
	if(!Dt.act->dyn)
	{
		Dt.act->dyn=0xff;
#ifndef NEWSGOLD
		if(Dt.act->img)
		{
			tIMG=Dt.act->img;
			Dt.act->img=Dt.ef->img_24_16(Dt.act->img, 0);
		}
#endif
	}
	fill_icons(1);
	REDRAW();
	return 0;
}

int resizeicon()
{
	memcpy(&mic,&Dt.act->p,sizeof(RECT));
	State=DT_RESZ;
	cur.x=Dt.act->p.x2;
	cur.y=Dt.act->p.y2;
	REDRAW();
	return 0;
}

int transp_changer()
{
  if(KEY_CPU_HI) SetCpuClockTempHi(2);
	memcpy(&mi,&Dt.act->p,sizeof(RECT));
	mic.x=Dt.act->alfa;
	mi.x=(mi.x2+mi.x)/2-25;
	if(mi.x<0) mi.x=2;
	if(mi.x+50>scr_w) mi.x=scr_w-52;
	mi.x2=mi.x+50;
	if(mi.y<10) mi.y=mi.y2+5;
	else mi.y=mi.y-10;
	mi.y2=mi.y+5;
	State=DT_CHTR;
	REDRAW();
	return 0;
}

int exit()
{
	exitt=1;
	return 1;
}

int openicondt()
{
	WSHDR *fws=AllocWS(256);
	char *s=translit(Dt.act->nm);
	wsprintf(fws,"%ticons\\%t.dti",Dt.rootadr,s);
	mfree(s); s=NULL;
	ExecuteFile(fws,NULL,NULL);
	FreeWS(fws);
	State=DT_MAIN;
	return 0;
}

int openconf()
{
	WSHDR *fws=AllocWS(256);
	extern const char *successed_config_filename;
	wsprintf(fws,perc_t,successed_config_filename);
	ExecuteFile(fws,NULL,NULL);
	FreeWS(fws);
	State=DT_MAIN;
	return 0;
}

extern void ClearIcons(),initic(),dofn();

int refresh_dt()
{
	ClearIcons();
	extern DT_ICON dt_first;
	dofn();
	State=DT_WAIT;
	Dt.inf=0;
	Dt.first=Dt.last=Dt.act=&dt_first;
	SUBPROC((void*)initic);
	return 0;
}

void doremoveicon(int i)
{
	char txt[64];
	unsigned int ul;
	zeromem(txt,64);
	State=DT_MAIN;
	if(!i) 
	{
		char *s=translit(Dt.act->nm);
		sprintf(txt,"%sicons\\%s.dti",Dt.rootadr,s);
		mfree(s); s=NULL;
		unlink(txt,&ul);
		//refresh_dt();
		del_icon(Dt.act);
// 		if(Dt.act->nm){ mfree(Dt.act->nm);Dt.act->nm=NULL;}
// 		if(Dt.act->fl){ mfree(Dt.act->fl);Dt.act->fl=NULL;}
// 		if(Dt.act->img_fl){ mfree(Dt.act->img_fl);Dt.act->img_fl=NULL;}
// 		if(Dt.act->inf){ mfree(Dt.act->inf);Dt.act->inf=NULL;}
// 		/* if(Dt.act->next)  */((DT_ICON*)Dt.act->prev)->next=Dt.act->next;
// 		/* if(Dt.act->prev)  */((DT_ICON*)Dt.act->next)->prev=Dt.act->prev;
// 		if(Dt.act->d) mfree(Dt.act);
// 		fill_icons(3);
	}
}

int removeicon()
{
	char txt[64];
	zeromem(txt,64);
	sprintf(txt,"%s \"%s\" ?", lg.remove,Dt.act->nm);
	MsgBoxYesNo(1, (int)txt, doremoveicon);
	return 0;
}

int edit_icon(DT_ICON *TI);

int m_new_icon()
{
	State=DT_MAIN;
	if(Dt.foc) edit_icon(Dt.act);
	else edit_icon(NULL);
	return 0;
}

/* typedef struct
{
	void *prev,*next;
	RECT p;
	char type, *nm, *inf;
	int (*onEnter)();
}DT_MENU_P; */
DT_MN_POINT mm_open={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_open,
		menup_run
		},
	mm_remove={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_remove,
		removeicon
		},
	mm_edit_icon={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_edit_icon,
		m_new_icon
		},
	mm_new_icon={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_new_icon,
		m_new_icon
		},
	mm_transp={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		"прозрачность",
		transp_changer
		},
	mm_move={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_move,
		moveicon
		},
	mm_resz={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_resize,
		resizeicon
		},
	mm_exit={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_exit,
		exit
		},
	mm_icon_dt={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_icon_config,
		openicondt,
		},
	mm_icon_subm={
		NULL,NULL,
		{0,0,0,0},
		MNP_SUBM,
		lg_icon_subm,
		open_icon_subm,
		},
	mm_bcfg={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_config,
		openconf,
		},
	mm_refresh_dt={
		NULL,NULL,
		{0,0,0,0},
		MNP_NORM,
		lg_refresh,
		refresh_dt,
		};

typedef struct
{
	DT_MN m;
	DT_MN_POINT *act;
	//void *sub;
	char foc, lev;
}R_MENU;
R_MENU mm;//={{0,0,0,0},NULL,NULL,NULL,NULL,0,0};



int open_icon_subm()
{
	char mcnt=0;
	unsigned short len=0;
	State=DT_MENU;
	
	DT_MN *m=malloc(sizeof(DT_MN));
	mm.m.sub=m;
	mm.lev=1;
	zeromem(m, sizeof(DT_MN));
	
	//zeromem(mm, sizeof(R_MENU));
	
	m->first=m->last=&mm_icon_dt;
	m->last->next=NULL;
	
	 // Ad_MN_Point(&mm.m, &mm_icon_dt);
	 Ad_MN_Point(m, &mm_remove);
	 Ad_MN_Point(m, &mm_edit_icon);
	 Ad_MN_Point(m, &mm_resz);
	 if(Dt.act->img) Ad_MN_Point(m, &mm_transp);
	 Ad_MN_Point(m, &mm_move);
	 //Ad_MN_Point(m, &mm_open);
	
	m->first->prev=NULL;
	if(MN_SEL_MOD) m->act=m->first;
	
	
	mm.foc=0;
	DT_MN_POINT *tmnp=m->first;
	do
	{
		//StoreXYWHtoRECT(&tmnp->p,cur.x+3,cur.y+(f_h+1)*mcnt+1,100,f_h+1);
		wsprintf(ews, perc_t, tmnp->nm);
		if(len<Get_WS_width(ews,FONT_SMALL)) len=Get_WS_width(ews,FONT_SMALL);
		mcnt++;
	}while(tmnp=tmnp->next);
	StoreXYWHtoRECT(&m->p,mm.m.p.x2-10,cur.y,len+4,(f_h+2)*mcnt+2);
	if(m->p.x2>scr_w) StoreXYXYtoRECT(&m->p,m->p.x-(m->p.x2-scr_w),m->p.y,scr_w,m->p.y2);
	if(m->p.y2>scr_h) StoreXYXYtoRECT(&m->p,m->p.x,m->p.y-(m->p.y2-scr_h),m->p.x2,scr_h);
	mcnt=0;
	tmnp=m->first;
	do
	{
		StoreXYWHtoRECT(&tmnp->p,m->p.x+2,m->p.y+(f_h+2)*mcnt+2,len,f_h);
		mcnt++;
	}while(tmnp=tmnp->next);
	
	return 0;
}


void onRight_menu()
{
	char mcnt=0;
	unsigned short len=0;
	State=DT_MENU;
	
	zeromem(&mm, sizeof(R_MENU));
	
	mm.m.first=mm.m.last=&mm_exit;
	mm.m.last->next=NULL;
	
	Ad_MN_Point(&mm.m, &mm_refresh_dt);
	
	
	Ad_MN_Point(&mm.m, &mm_bcfg);
	
	if(Dt.foc)
	{
	 Ad_MN_Point(&mm.m, &mm_icon_subm);
	 Ad_MN_Point(&mm.m, &mm_open);
	 // Ad_MN_Point(&mm.m, &mm_icon_dt);
	 // Ad_MN_Point(&mm.m, &mm_remove);
	 // Ad_MN_Point(&mm.m, &mm_edit_icon);
	 // Ad_MN_Point(&mm.m, &mm_resz);
	 // Ad_MN_Point(&mm.m, &mm_move);
	}
	else
	 Ad_MN_Point(&mm.m, &mm_new_icon);
	
	mm.m.first->prev=NULL;
	if(MN_SEL_MOD) mm.act=mm.m.act=mm.m.first;
	
	
	mm.foc=0;
	DT_MN_POINT *tmnp=mm.m.first;
	do
	{
		//StoreXYWHtoRECT(&tmnp->p,cur.x+3,cur.y+(f_h+1)*mcnt+1,100,f_h+1);
		wsprintf(ews, perc_t, tmnp->nm);
		if(len<Get_WS_width(ews,FONT_SMALL)) len=Get_WS_width(ews,FONT_SMALL);
		mcnt++;
	}while(tmnp=tmnp->next);
	StoreXYWHtoRECT(&mm.m.p,cur.x,cur.y,len+4,(f_h+2)*mcnt+2);
	if(mm.m.p.x2>scr_w) StoreXYXYtoRECT(&mm.m.p,mm.m.p.x-(mm.m.p.x2-scr_w),mm.m.p.y,scr_w,mm.m.p.y2);
	if(mm.m.p.y2>scr_h) StoreXYXYtoRECT(&mm.m.p,mm.m.p.x,mm.m.p.y-(mm.m.p.y2-scr_h),mm.m.p.x2,scr_h);
	mcnt=0;
	tmnp=mm.m.first;
	do
	{
		StoreXYWHtoRECT(&tmnp->p,mm.m.p.x+2,mm.m.p.y+(f_h+2)*mcnt+2,len,f_h);
		mcnt++;
	}while(tmnp=tmnp->next);
}
