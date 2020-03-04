//========================================================================================


void dr_inf()
{
	char l=1, j=0, tmp[64];
	unsigned int len=0, k;
	RECT p;
	Dt.act->onInf();
	if(!Dt.act->inf) return;
	for(k=1; k<strlen(Dt.act->inf); k++)
	{
		j++;
		if(Dt.act->inf[k]=='\n')
		{
			l++;
			zeromem(tmp, 64);
			memcpy(tmp, Dt.act->inf+k-j, j);
			wsprintf(ews, perc_t, tmp);
			j=0;
			if(Get_WS_width(ews, FONT_SMALL)>len) len=Get_WS_width(ews, FONT_SMALL);
		}
	}
	zeromem(tmp, 64);
	if(l==1) strcpy(tmp, Dt.act->inf);
	else memcpy(tmp, Dt.act->inf+k-j, j);
	wsprintf(ews, perc_t, tmp);
	j=0;
	if(Get_WS_width(ews, FONT_SMALL)>len) len=Get_WS_width(ews, FONT_SMALL);
	wsprintf(ews, perc_t, Dt.act->inf );
	/* if(l==1) */ //len=Get_WS_width(ews, FONT_SMALL);
	if(cur.x+len+3>=scr_w)
	{
		p.x=scr_w-len-3;
		p.x2=scr_w;
	}
	else
	{
		p.x=cur.x;
		p.x2=cur.x+len+3;
	}
	if(cur.y+cursor->h+l*f_h+1>=scr_h)
	{
		p.y=cur.y-l*f_h-1;
		p.y2=cur.y-1;
	}
	else
	{
		p.y=cur.y+cursor->h+1;
		p.y2=cur.y+cursor->h+l*f_h+1;
	}
	DrawRectangle(p.x,p.y,p.x2,p.y2, 0,white,white0);
	DrawString(ews,p.x+2,p.y+1,p.x2,p.y2,FONT_SMALL,1,black,NULL);
}

	// __root const CFG_HDR cfghdr0_0={CFG_CBOX,"Selecting mode",0,2};
	// __root const int MN_SEL_MOD=0;
	// __root const CFG_CBOX_ITEM cfgcbox1[]={"Cursor","Selecting"};

extern const char MN_COL1[],MN_COL0[],S_MN_COL1[],S_MN_COL0[],MN_FONT_COL[];

void dr_m( char fnd, char inf ) //
{
//   if(Dt.pglist.fullscr) goto drawfullscrwindow;
  // DrwImg(mfn, 0,0, NULL,NULL); // fon
  DrawObject(&fonobj);
  //DrwImg(&Dt.i, 0,0, NULL,NULL); // icons
  TAI=Dt.first;
  if(fnd)
  {
	Dt.n=Dt.ni.bitmap[GetBitmapMapPos(&Dt.ni, cur.x, cur.y)];
	Dt.foc=0;
  }
  if(Dt.n<128)
  {
	do
	{
	  //if(Dt.n!=0xFE)
	  {
		if( fnd && Dt.n && (TAI->n==Dt.n)) // selected found
		{
		  Dt.act=TAI;
		  Dt.foc=1;
		  //break;
		}
	  }
	  if(TAI->dyn) TAI->onDraw();
	  if(Dt.foc /*&& Dt.n<128*/) DrawRectangle(Dt.act->p.x-1,Dt.act->p.y-1,Dt.act->p.x2,Dt.act->p.y2,1,white,NULL); // round selected
	}while(TAI=TAI->next);
  }
  
//   drawfullscrwindow:
  if(Dt.pf->PG_Draw) Dt.pf->PG_Draw(fnd, inf);
  
  if( inf && Dt.inf && Dt.foc )
  {
	if(Dt.n<128) dr_inf(); // draw info
  }
}

void dr_WAIT()
{
	DrawRectangle(0,0,scr_w,scr_h,0,red,black);
	wsprintf(ews, perc_t, "Подождите..." );
	DrawString(ews,10,scr_h/2-2*f_h,scr_w-10,scr_h,FONT_MEDIUM,TEXT_ALIGNMIDDLE,white,NULL);
}

void dr_MAIN()
{
	dr_m(1,1);
	DrwImg(Dt.cursor, cur.x,cur.y, white,NULL);
	if(Dt.nn<=Dt.cnt)
	{
		wsprintf(ews, perc_t, "Загрузка..." );
		DrawString(ews,0,YDISP,scr_w,YDISP+f_h,FONT_SMALL,TEXT_ALIGNMIDDLE,red,white0);
		DrawRectangle(0,scr_h-5,scr_w,scr_h,0,red,NULL);
		DrawRectangle(0,scr_h-5,scr_w*Dt.nn/Dt.cnt,scr_h,0,red,white0);
	}
}


void dr_MENU(  ) //
{
		dr_m( 0, 0 );
	DT_MN *m=&mm.m;
	DT_MN_POINT *tmnp;//=m->first;

	for(int i=0; i<=mm.lev; i++)
	{
		tmnp=m->first;
		DrawRectangle(m->p.x,m->p.y,m->p.x2,m->p.y2,0,MN_COL0,MN_COL1);
		if( m->sub || MN_SEL_MOD || mm.foc) DrawRectangle(m->act->p.x-1,m->act->p.y-1,m->act->p.x2+1,m->act->p.y2+1,0,S_MN_COL0,S_MN_COL1);
		mm.foc=0;
		do
		{
		 if(!MN_SEL_MOD&&!m->sub)
		  if(infoc(&tmnp->p, cur.x,cur.y))
		  {
		   mm.foc=1;
		   mm.act=m->act=tmnp;
		  }
		  wsprintf(ews, perc_t, tmnp->nm );
		  DrawString(ews,tmnp->p.x,tmnp->p.y,tmnp->p.x2,tmnp->p.y2,FONT_SMALL,1,MN_FONT_COL,NULL);
		}while(tmnp=(DT_MN_POINT*)tmnp->next);
	
		if(m->sub)
		{
			m=(DT_MN*)(m->sub);
		}
		else i=mm.lev+1;
	}
		if(!MN_SEL_MOD) DrwImg(Dt.cursor, cur.x,cur.y, white,NULL);
}

void dr_RESZ()
{
	Dt.act->p.x2=cur.x;
	Dt.act->p.y2=cur.y;
	dr_m( 0, 0 );
}

void dr_MOVE()
{
	Dt.act->p.x=mi.x+cur.x;
	Dt.act->p.y=mi.y+cur.y;
	Dt.act->p.x2=mi.x2+cur.x;
	Dt.act->p.y2=mi.y2+cur.y;
	dr_m( 0, 0 );
}

void dr_CHTR()
{
	dr_m( 0, 0 );
	DrawRectangle(mi.x, mi.y, mi.x2, mi.y2, 0, S_MN_COL0,MN_COL1);
	DrawRectangle(mi.x, mi.y, mi.x+Dt.act->alfa/2, mi.y2, 0, S_MN_COL0,S_MN_COL1);
}

void dr_LPLG()
{
// 	wsprintf(ews, perc_t, "Загрузка плагинов" );
// 	DrawString(ews,0,YDISP,scr_w,YDISP+f_h,FONT_SMALL,TEXT_ALIGNMIDDLE,red,white);
// 	loadplg();
	State=DT_MAIN;
	SMART_REDRAW();//REDRAW();
}

void dr_ERR()
{
	DrawRectangle(0,0,scr_w,scr_h,0,red,black);
	wsprintf(ews, perc_t, "ОШИБКА!!!" );
	DrawString(ews,10,scr_h/2-2*f_h,scr_w-10,scr_h,FONT_MEDIUM,TEXT_ALIGNMIDDLE,white,NULL);
}

void OnRedraw(GUI *data)
{
	State->D();
}



