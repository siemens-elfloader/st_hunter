
#define APG dt->pglist.act
// #define dr_MOVE dt->states->st_MOVE->D
// #define dr_RESZ dt->states->st_RESZ->D
#define cur dt->cur

void reset_WIND(PGUI_methods *p, char t);
void Add_PGUI(PGUI_methods *p);
void Del_PGUI(PGUI_methods *p);

void PG_Draw( char fnd, char inf )
{
  if(dt->pglist.show)
  {
	PGUI_methods *TPG=dt->pglist.first;
	if(TPG)
	  do
	  {
		TPG->OnRedraw();
		if(TPG->type&2)
		{
		  DrwImg(TPG->w, TPG->wp.x,  TPG->wp.y, white,NULL);
		  
		  wsprintf(ews, perc_t, TPG->name );
		  DrawString(ews, TPG->wp.x,  TPG->wp.y+1, TPG->wp.x2-2*f_h-1,  TPG->wp.y+1+f_h,  FONT_SMALL,TEXT_ALIGNMIDDLE,white,NULL);
		}
	  }while(TPG=TPG->next);
	  
	  if(fnd)
	  {
		APG->bn=0;
		if(dt->n==APG->n) APG->bn=APG->bi->bitmap[dt->ef->GetBitmapMapPos(APG->bi, cur.x-APG->wp.x, cur.y-APG->wp.y)];
	  }
  }
}

void dr_PGM()
{
  APG->wp.x=dt->mi.x+cur.x;
  APG->wp.y=dt->mi.y+cur.y;
  APG->wp.x2=dt->mi.x2+cur.x;
  APG->wp.y2=dt->mi.y2+cur.y;
  //   dr_MOVE();
  StoreXYXYtoRECT(&APG->p, APG->wp.x+1, APG->wp.y+2+f_h, APG->wp.x2-1, APG->wp.y2-1);
  dt->mf->dr_m(0,0);
//   mdr_PGUI();
}

void dr_PGR()
{
  APG->wp.x2=cur.x;
  APG->wp.y2=cur.y;
//   dr_RESZ();
  reset_WIND(APG, 2);
  dt->mf->dr_m(0,0);
  // 	dt->pguif->crpgwind(APG);
//   StoreXYXYtoRECT(&APG->p, dt->act->p.x+1, dt->act->p.y+2+f_h, dt->act->p.x2-1, dt->act->p.y2-1);
//   mdr_PGUI();
}



int m_key_PGM(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  if(m==KEY_DOWN)
	switch(msg->gbsmsg->submess)
	{
	  case RIGHT_SOFT:
		memcpy(&APG->wp,&dt->mic,sizeof(RECT));
		reset_WIND(APG, 0);
		dt->st=dt->states->st_MAIN;
		SMART_REDRAW();//REDRAW();
		return 0;
	  case ENTER_BUTTON:
	  case '5':
		StoreXYXYtoRECT(&APG->wp, dt->mi.x+cur.x,dt->mi.y+cur.y, dt->mi.x2+cur.x,dt->mi.y2+cur.y);
		reset_WIND(APG, 0);
		dt->st=dt->states->st_MAIN;
		SMART_REDRAW();//REDRAW();
		return 0;
	}
	// 	dt->mf->stdmove(msg);
	dt->mf->curkey(msg);
	return 0;
}



// extern void crpgwind(PGUI_methods *p);


int m_key_PGR(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  if(m==KEY_DOWN)
	switch(msg->gbsmsg->submess)
	{
	  case RIGHT_SOFT:
		memcpy(&APG->wp,&dt->mic,sizeof(RECT));
		reset_WIND(APG, 1);
		dt->st=dt->states->st_MAIN;
		SMART_REDRAW();//REDRAW();
		return 0;
	  case ENTER_BUTTON:
	  case '5':
		APG->wp.x2=cur.x;
		APG->wp.y2=cur.y;
		reset_WIND(APG, 1);
		dt->st=dt->states->st_MAIN;
		SMART_REDRAW();//REDRAW();
		return 0;
	}
	// 	dt->mf->stdmove(msg);
	dt->mf->curkey(msg);
	return 0;
}



void Close_PGUI(PGUI_methods *p);

int key_PG_exit(GUI_MSG *msg)
{
  return 1;
}

int key_PG_move(GUI_MSG *msg)
{
  if(APG->st!=PG_st_max)
  {
	memcpy(&dt->mic,&APG->wp,sizeof(RECT));
	dt->st=&st_PGM;
	dt->mi.x=APG->wp.x-cur.x;
	dt->mi.y=APG->wp.y-cur.y;
	dt->mi.x2=APG->wp.x2-cur.x;
	dt->mi.y2=APG->wp.y2-cur.y;
	SMART_REDRAW();
  }
  
  
//   ShowMSG(1,(int)"Двигаемся :)");
//   if(APG->st!=PG_st_max)
//   {
// 	memcpy(&dt->mic,&dt->act->p,sizeof(RECT));
// // 	dt->st=&st_PGM;
// 	dt->mi.x=dt->act->p.x-cur.x;
// 	dt->mi.y=dt->act->p.y-cur.y;
// 	dt->mi.x2=dt->act->p.x2-cur.x;
// 	dt->mi.y2=dt->act->p.y2-cur.y;
// 	SMART_REDRAW();
//   }
  return 0;
}

int key_PG_resz(GUI_MSG *msg)
{
//   ShowMSG(1,(int)"Меняем размер :)");
  if(APG->st!=PG_st_max)
  {
	memcpy(&dt->mic,&APG->wp,sizeof(RECT));
	dt->st=&st_PGR;
	cur.x=APG->wp.x2;
	cur.y=APG->wp.y2;
	//REDRAW();
	SMART_REDRAW();
  }
  return 0;
}

int is_fullscreen_PGUI()
{
  PGUI_methods *TPG=dt->pglist.first;
  if(TPG)
  {
	do
	{
	  if(TPG->st==PG_st_max) return 1;
	}while(TPG=TPG->next);
  }
  return 0;
}

int key_PG_razv(GUI_MSG *msg)
{
  if(APG->st!=PG_st_max)
  {
	APG->st=PG_st_max;
	memcpy(&temp_wim_p, &APG->wp, sizeof(RECT));
	StoreXYXYtoRECT(&APG->wp, 0, 0+YDISP, dt->w, dt->h);
	reset_WIND(APG, 1);
	dt->pglist.fullscr=1;
  }
  else
  {
	APG->st=PG_st_norm;
	memcpy(&APG->wp, &temp_wim_p, sizeof(RECT));
	reset_WIND(APG, 1);
	dt->pglist.fullscr=is_fullscreen_PGUI();
  }
  SMART_REDRAW();
  return 0;
}





PGUI_methods *find_PGUI_by_n(int n)
{
  PGUI_methods *TPG=dt->pglist.first;
  if(TPG)
  {
	do
	{
	  if(TPG->n==n) return TPG;
	}while(TPG=TPG->next);
  }
  return 0;
}

void remun_PGUI_list()
{
  PGUI_methods *TPG=dt->pglist.first;
  int n=0;
  if(TPG)
  {
	do
	{
	  n++;
	  TPG->n=n+128;
	}while(TPG=TPG->next);
  }
  dt->pglist.n=n;
}


int PG_Keys(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  PG_BUTTON *TB;
  if(m==KEY_DOWN)
  {
	switch(msg->gbsmsg->submess)
	{
	  case ENTER_BUTTON:
		if(dt->n==APG->n)
		{
		  TB=APG->btn.first;
		  do
		  {
			if(TB->n==APG->bn)
			{
			  if(TB->Enter()==1)
				Close_PGUI(APG);
			  SMART_REDRAW();
			  return 0;
			}
		  }while(TB=TB->next);
		}
		else if(dt->n>128)
		{
		  PGUI_methods *tpg=find_PGUI_by_n(dt->n);
		  if(tpg)
		  {
			Del_PGUI(tpg);
// 			remun_PGUI_list();
			Add_PGUI(tpg);
			dt->ef->fill_icons(2);
			return 0;
		  }
		}
		break;
	}
  }
  
  int rtn=0;
  if(APG) rtn=APG->OnKey(msg);
  switch(rtn)
  {
	case 1:
	  Close_PGUI(APG);
	  SMART_REDRAW();
	  return 0;
	case -1:
	  SMART_REDRAW();
	  return 0;
	case 0:
	  break;
	default:
	  return 2;
  }
  
  // 	if(APG->type&4)
  // 	{
	// 	  if(APG->type&1) dt->mf->curkey(msg);//dr_m( 0, 0 );
	// 	}
	// 	else
	// 	{
	  // 	  if(APG->type&1) dt->states->st_MAIN->K(msg);//m_key_MAIN(msg);
	  // 	}
	  
	  //if(Dt.pglist.act->type&1) curkey(msg);
	  return 2;
}



DtSt  st_PGM={dr_PGM, m_key_PGM},
  st_PGR={dr_PGR, m_key_PGR};
//   st_PGUI={dr_PGUI, m_key_PGUI};

