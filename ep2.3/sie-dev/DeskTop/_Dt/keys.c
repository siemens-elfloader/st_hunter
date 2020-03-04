




void cursor_screenend_jump()
{
  if(!CUR_JUMP)
  {
	if(cur.x>scr_w) cur.x=scr_w;
	if(cur.x<0) cur.x=0;
	if(cur.y>scr_h) cur.y=scr_h;
	if(cur.y<0+YDISP) cur.y=0+YDISP;
  }
  else
  {
	if(cur.x>scr_w) cur.x=0;
	if(cur.x<0) cur.x=scr_w;
	if(cur.y>scr_h) cur.y=0+YDISP;
	if(cur.y<0+YDISP) cur.y=scr_h;
  }
}

void showinf()
{
	Dt.inf=1;
	if(Dt.foc) SMART_REDRAW();//DirectRedrawGUI();
}

void movecur()
{
	Dt.inf=0;
	cur.x+=movcr.st*movcr.x;
	cur.y+=movcr.st*movcr.y;
	
	cursor_screenend_jump();

// 	if(!CUR_JUMP)
// 	{
// 		if(cur.x>scr_w) cur.x=scr_w;
// 		if(cur.x<0) cur.x=0;
// 		if(cur.y>scr_h) cur.y=scr_h;
// 		if(cur.y<0+YDISP) cur.y=0+YDISP;
// 	}
// 	else
// 	{
// 		if(cur.x>scr_w) cur.x=0;
// 		if(cur.x<0) cur.x=scr_w;
// 		if(cur.y>scr_h) cur.y=0+YDISP;
// 		if(cur.y<0+YDISP) cur.y=scr_h;
// 	}

	DirectRedrawGUI();
	if( movcr.x || movcr.y )
	{
		if(IsTimerProc(&inftmr)) GBS_DelTimer(&inftmr);
		if(movcr.t<L_STEP_COUNT)
		{
		 movcr.t++;
		 GBS_StartTimerProc(&curtmr, CUR_TIMER, movecur);
		}
// 		else if(movcr.t<15)
// 		{
// 		 movcr.st=5;
// 		 movcr.t++;
// 		 GBS_StartTimerProc(&curtmr, 10, movecur);
// 		}
		else
		{
		 movcr.st=CUR_BIG_STEP;
		 GBS_StartTimerProc(&curtmr, CUR_TIMER, movecur);
		}
	}
	else
	{
		movcr.t=0;
		if(KEY_CPU_HI) SetCpuClockLow(2);
		if(IsTimerProc(&inftmr)) GBS_DelTimer(&inftmr);
		GBS_StartTimerProc(&inftmr, 108, showinf);
	}
	//movcr.st=10;
}

void srart_movecur()
{
	movcr.st=2;
	if(IsTimerProc(&curtmr)) GBS_DelTimer(&curtmr);
	if(KEY_CPU_HI) SetCpuClockTempHi(2);
	movecur();
}

/* void stop_movecur()
{
	Dt.inf=0;
	if((movcr.x==0)&&(movcr.y==0))
} */

void curkey(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  switch(m)
  {
   case KEY_DOWN:
	switch(msg->gbsmsg->submess)
	{
	 case UP_BUTTON:
	 case '2':
		movcr.y=-1;
		srart_movecur();
	   break;
	 case DOWN_BUTTON:
	 case '8':
		movcr.y=1;
		srart_movecur();
	   break;
	 case RIGHT_BUTTON:
	 case '6':
		movcr.x=1;
		srart_movecur();
	   break;
	 case LEFT_BUTTON:
	 case '4':
		movcr.x=-1;
		srart_movecur();
	   break;
	 case '1':
		movcr.x=-1;
		movcr.y=-1;
		srart_movecur();
	   break;
	 case '3':
		movcr.x=1;
		movcr.y=-1;
		srart_movecur();
	   break;
	 case '7':
		movcr.x=-1;
		movcr.y=1;
		srart_movecur();
	   break;
	 case '9':
		movcr.x=1;
		movcr.y=1;
		srart_movecur();
	   break;
	}
	//DirectRedrawGUI();
	break;
   case KEY_UP:
	switch(msg->gbsmsg->submess)
	{
	 case UP_BUTTON:
	 case DOWN_BUTTON:
	 case '2':
	 case '8':
		movcr.y=0;
	   break;
	 case RIGHT_BUTTON:
	 case LEFT_BUTTON:
	 case '4':
	 case '6':
		movcr.x=0;
	   break;
	 case '1':
	 case '3':
	 case '7':
	 case '9':
		movcr.x=0;
		movcr.y=0;
	   break;
	 case '#':SetCpuClockLow(2);
	   break;
	}
	//DirectRedrawGUI();
	break;
  }
}


void stdmove(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  switch(m)
  {
   case KEY_DOWN:
	switch(msg->gbsmsg->submess)
	{
	 case UP_BUTTON:
	 case '2':
		cur.y--;
	   break;
	 case DOWN_BUTTON:
	 case '8':
		cur.y++;
	   break;
	 case RIGHT_BUTTON:
	 case '6':
		cur.x++;
	   break;
	 case LEFT_BUTTON:
	 case '4':
		cur.x--;
	   break;
	 case '1':
		cur.x--;
		cur.y--;
	   break;
	 case '3':
		cur.x++;
		cur.y--;
	   break;
	 case '7':
		cur.x--;
		cur.y++;
	   break;
	 case '9':
		cur.x++;
		cur.y++;
	   break;
	}
	cursor_screenend_jump();
// 	if(!CUR_JUMP)
// 	{
// 		if(cur.x>scr_w) cur.x=scr_w;
// 		if(cur.x<0) cur.x=0;
// 		if(cur.y>scr_h) cur.y=scr_h;
// 		if(cur.y<0+YDISP) cur.y=0+YDISP;
// 	}
// 	else
// 	{
// 		if(cur.x>scr_w) cur.x=0;
// 		if(cur.x<0) cur.x=scr_w;
// 		if(cur.y>scr_h) cur.y=0+YDISP;
// 		if(cur.y<0+YDISP) cur.y=scr_h;
// 	}
	DirectRedrawGUI();
	break;
   case LONG_PRESS:
	switch(msg->gbsmsg->submess)
	{
	 case UP_BUTTON:
	 case '2':
		cur.y-=CUR_BIG_STEP;
	   break;
	 case DOWN_BUTTON:
	 case '8':
		cur.y+=CUR_BIG_STEP;
	   break;
	 case RIGHT_BUTTON:
	 case '6':
		cur.x+=CUR_BIG_STEP;
	   break;
	 case LEFT_BUTTON:
	 case '4':
		cur.x-=CUR_BIG_STEP;
	   break;
	 case '1':
		cur.x-=CUR_BIG_STEP;
		cur.y-=CUR_BIG_STEP;
	   break;
	 case '3':
		cur.x+=CUR_BIG_STEP;
		cur.y-=CUR_BIG_STEP;
	   break;
	 case '7':
		cur.x-=CUR_BIG_STEP;
		cur.y+=CUR_BIG_STEP;
	   break;
	 case '9':
		cur.x+=CUR_BIG_STEP;
		cur.y+=CUR_BIG_STEP;
	   break;
	}
	cursor_screenend_jump();
// 	if(!CUR_JUMP)
// 	{
// 		if(cur.x>scr_w) cur.x=scr_w;
// 		if(cur.x<0) cur.x=0;
// 		if(cur.y>scr_h) cur.y=scr_h;
// 		if(cur.y<0+YDISP) cur.y=0+YDISP;
// 	}
// 	else
// 	{
// 		if(cur.x>scr_w) cur.x=0;
// 		if(cur.x<0) cur.x=scr_w;
// 		if(cur.y>scr_h) cur.y=0+YDISP;
// 		if(cur.y<0+YDISP) cur.y=scr_h;
// 	}
	DirectRedrawGUI();
	break;
  }
}



//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////








//void onEnter()
//{
  // DT_ICON *TI=Dt.icon;
  // for(int i=0; i<Dt.icon_cnt; i++)
	// if((cur.x>=TI->p.x)&&(cur.x<=TI->p.x2)&&(cur.y>=TI->p.y)&&(cur.y<=TI->p.y2))
	// {
		// TI->onEnter();
		// i=Dt.icon_cnt;
		// if(i<Dt.icon_cnt-1) TI=TI->next;
	// }
//}

void MN_OnKey(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  
  
	DT_MN *mn=&mm.m;

	for(int i=0; i<=mm.lev; i++)
	{
		if(mn->sub) mn=(DT_MN*)(mn->sub);
		else i=mm.lev+1;
	}
   if((m==KEY_DOWN)||(m==LONG_PRESS))
	switch(msg->gbsmsg->submess)
	{
	 case RIGHT_BUTTON:
		mn->act=mn->last;
	  break;
	 case LEFT_BUTTON:
		mn->act=mn->first;
	  break;
	 case UP_BUTTON:
		if(!(mn->act=mn->act->prev)) mn->act=mn->last;
	  break;
	 case DOWN_BUTTON:
		if(!(mn->act=mn->act->next)) mn->act=mn->first;
	  break;
	 default: return;
	}
	mm.act=mn->act;
	DirectRedrawGUI();
}



#include "gui.c"

int do_cur_dis()
{
	if(IsTimerProc(&inftmr)) GBS_DelTimer(&inftmr);
	GBS_StartTimerProc(&inftmr, 108, showinf);
	//DirectRedrawGUI();
	return 0;
}

void Stop_SSV_Timer()
{
  if(IsTimerProc(&ssvtmr))
  {
	GBS_StopTimer(&ssvtmr);
	GBS_DelTimer(&ssvtmr);
  }
}

void Start_SSV_Timer()
{
  Stop_SSV_Timer();
  GBS_StartTimerProc(&ssvtmr, TMR_SEC*60, Start_Screen_Saver);
}

int m_key_MAIN(GUI_MSG *msg)
{
//   if(Dt.n>128)
  int rtn=Dt.pf->PG_Keys(msg);
  switch(rtn)
  {
	case 0: return 0;
	case 1: return 1;
	default: break;
  }
  
  int m=msg->gbsmsg->msg;
  if(m==KEY_DOWN)
  {
	Stop_SSV_Timer();
	switch(msg->gbsmsg->submess)
	{
	  case RIGHT_SOFT:
		if(KEY_CPU_HI) SetCpuClockTempHi(2);
		onRight_menu();
		return 0;
	  case ENTER_BUTTON:
	  case '5':
		if(Dt.foc) return(Dt.act->onEnter());
		else return 0;
	  case RED_BUTTON:
		Start_Screen_Saver();
		// 			ShowMSG(1,(int)Dt.rootadr);
		return 0;
	}
  }
  if(m==KEY_UP || msg->gbsmsg->submess==24 || msg->gbsmsg->submess==22)
	Start_SSV_Timer();
  if(m==LONG_PRESS && msg->gbsmsg->submess=='#')
	Start_Screen_Saver();
	   /* if(CUR_ENABLE)  */curkey(msg);
	   // else
	   // if((m==KEY_DOWN)||(m==LONG_PRESS))
		// switch(msg->gbsmsg->submess)
		// {
		 // case UP_BUTTON:
			// Dt.act=Dt.act->up;
			// return do_cur_dis();
		 // case DOWN_BUTTON:
			// Dt.act=Dt.act->down;
			// return do_cur_dis();
		 // case LEFT_BUTTON:
			// Dt.act=Dt.act->left;
			// return do_cur_dis();
		 // case RIGHT_BUTTON:
			// Dt.act=Dt.act->right;
			// return do_cur_dis();
		// }
		//DirectRedrawGUI();
	return 0;//-1;
}




int m_key_MENU(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
	if(m==KEY_DOWN)
		switch(msg->gbsmsg->submess)
		{
		 case RIGHT_SOFT:
			// if(!MN_SEL_MOD) onRight_menu(); // cursor
			// else
			{
			 if(KEY_CPU_HI) SetCpuClockLow(2);
			 State=DT_MAIN;
			}
		  break;
		 case RED_BUTTON:
			if(KEY_CPU_HI)
			{
				SetCpuClockLow(2);
			}
			State=DT_MAIN;
		  break;
		 case ENTER_BUTTON:
		 case '5':
			if((mm.act->type!=MNP_SUBM)&&KEY_CPU_HI) SetCpuClockLow(2);
			if(!MN_SEL_MOD) // cursor
			{
			 /* if(!infoc(&mm.p, cur.x,cur.y)) State=DT_MAIN;
			 else  */if(mm.foc) return(mm.act->onEnter());
			 else State=DT_MAIN;
			}
			else return(mm.act->onEnter());
		  break;
		}
	if(!MN_SEL_MOD) curkey(msg); // cursor
	else MN_OnKey(msg);
	DirectRedrawGUI();
	return 0;//-1;
}


int m_key_ERR(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
	if(m==KEY_DOWN)
	switch(msg->gbsmsg->submess)
	{
		case RIGHT_SOFT:
			return 1;
	}
	return 0;
}


int m_key_MOVE(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
	if(m==KEY_DOWN)
	switch(msg->gbsmsg->submess)
	{
		case RIGHT_SOFT:
#ifndef NEWSGOLD
			if(Dt.act->img)
			{
				mfree(Dt.act->img->bitmap);
				mfree(Dt.act->img);
				Dt.act->img=tIMG;
			}
#endif
			
			memcpy(&Dt.act->p,&mic,sizeof(RECT));
			if(Dt.act->dyn==0xff) Dt.act->dyn=0;
			ICON_REDRAW(&Dt, Dt.act);
			//fill_icons(1);
			State=DT_MAIN;
			SMART_REDRAW();//REDRAW();
		return 0;
		case ENTER_BUTTON:
		case '5':
#ifndef NEWSGOLD
			if(Dt.act->img)
			{
				mfree(Dt.act->img->bitmap);
				mfree(Dt.act->img);
				Dt.act->img=tIMG;
			}
#endif
			
			Dt.act->p.x=mi.x+cur.x;
			Dt.act->p.y=mi.y+cur.y;
			Dt.act->p.x2=mi.x2+cur.x;
			Dt.act->p.y2=mi.y2+cur.y;
			if(Dt.act->dyn==0xff) Dt.act->dyn=0;
			SUBPROC((void*)saveicon, Dt.act);
			ICON_REDRAW(&Dt, Dt.act);
			//fill_icons(2);
			//fill_icons(3);
			State=DT_MAIN;
			SMART_REDRAW();//REDRAW();
		return 0;
	}
	stdmove(msg);
	//curkey(msg);
	return 0;
}


int m_key_CHTR(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
	if(m==KEY_DOWN || m==LONG_PRESS)
	switch(msg->gbsmsg->submess)
	{
		case RIGHT_SOFT:
			Dt.act->alfa=(char)mic.x;
			ICON_REDRAW(&Dt, Dt.act);
			//fill_icons(1);
			if(KEY_CPU_HI) SetCpuClockLow(2);
			State=DT_MAIN;
			SMART_REDRAW();//REDRAW();
		return 0;
		case ENTER_BUTTON:
		case '5':
			// Dt.act->p.x=mi.x+cur.x;
			// Dt.act->p.y=mi.y+cur.y;
			// Dt.act->p.x2=mi.x2+cur.x;
			// Dt.act->p.y2=mi.y2+cur.y;
			// if(Dt.act->dyn==0xff) Dt.act->dyn=0;
			SUBPROC((void*)saveicon, Dt.act);
			ICON_REDRAW(&Dt, Dt.act);
			//fill_icons(2);
			//fill_icons(3);
			if(KEY_CPU_HI) SetCpuClockLow(2);
			State=DT_MAIN;
			SMART_REDRAW();//REDRAW();
		return 0;
		case RIGHT_BUTTON:
		case '6':
			Dt.act->alfa+=2;
			if(Dt.act->alfa>100) Dt.act->alfa=0;
			ICON_REDRAW(&Dt, Dt.act);
			DirectRedrawGUI();
		return 0;
		case LEFT_BUTTON:
		case '4':
			Dt.act->alfa-=2;
			if(Dt.act->alfa>100) Dt.act->alfa=100;
			ICON_REDRAW(&Dt, Dt.act);
			DirectRedrawGUI();
		return 0;
	}
	//stdmove(msg);
	//curkey(msg);
	return 0;
}


int m_key_RESZ(GUI_MSG *msg)
{
  int t=0;
  int m=msg->gbsmsg->msg;
	if(m==KEY_DOWN)
	switch(msg->gbsmsg->submess)
	{
		case RIGHT_SOFT:
		memcpy(&Dt.act->p,&mic,sizeof(RECT));
		State=DT_MAIN;
		SMART_REDRAW();//REDRAW();
		return 0;
		case ENTER_BUTTON:
		case '5':
		Dt.act->p.x2=cur.x;
		Dt.act->p.y2=cur.y;
		if(Dt.act->img_fl)
		{
			mfree(Dt.act->img->bitmap);
			mfree(Dt.act->img);
			Dt.act->img=0;
			// Dt.act->img=CreateIMGHDRFromPngFile(Dt.act->img_fl, 3);
			// Dt.act->img=resample(Dt.act->img, Dt.act->p.x2-Dt.act->p.x, Dt.act->p.y2-Dt.act->p.y, 0, 1);
		}
		if(Dt.act->Refresh) Dt.act->Refresh(0);
		SUBPROC((void*)saveicon, Dt.act);
		// ICON_REDRAW(&Dt, Dt.act);
		//fill_icons(2);
		// saveicon(Dt.act);
		fill_icons(3);
		State=DT_MAIN;
		SMART_REDRAW();//REDRAW();
		return 0;
		case '0':
		//if(Dt.act->img_fl)
		{
			cur.x=Dt.act->p.x+Dt.act->pp.x;//GetImgWidth((int)Dt.act->img_fl);
			cur.y=Dt.act->p.y+Dt.act->pp.y;//GetImgHeight((int)Dt.act->img_fl);
		}
		break;
		case '*':
		//if(Dt.act->img_fl)
		{
			t=Dt.act->p.y+((cur.x-Dt.act->p.x)*Dt.act->pp.y)/Dt.act->pp.x;//GetImgHeight((int)Dt.act->img_fl)/GetImgWidth((int)Dt.act->img_fl));
			if(t<=cur.y) cur.y=t;
			else cur.x=Dt.act->p.x+((cur.y-Dt.act->p.y)*Dt.act->pp.x)/Dt.act->pp.y;//GetImgWidth((int)Dt.act->img_fl)/GetImgHeight((int)Dt.act->img_fl));
			//cur.y=Dt.act->p.y+GetImgHeight((int)Dt.act->img_fl);
		}
		break;
	}
	stdmove(msg);
	//curkey(msg);
	return 0;
}


int m_key_WAIT(GUI_MSG *msg)
{return 0;}




int m_key_LPLG(GUI_MSG *msg)
{return 0;}


 //OnKey
// int m_key(GUI_MSG *msg)
// {
//   int m=msg->gbsmsg->msg;
//   //unsigned int ul;
//   int t=0;
// 	switch(State)
// 	{
// 	 //case DT_WAIT: /* curkey(msg); */ return 0;
// 	 case DT_MAIN:
// 		return m_key_MAIN(msg);
// 	 
// 	 case DT_MENU:
// 		return m_key_MENU(msg);
// 	  
// 	  
// 	 case DT_MOVE:
// 	  break;
// 	  
// 	  
// 	 case DT_CHTR:
// 	  break;
// 	  
// 	  
// 	  
// 	 case DT_RESZ:
// 	  break;
// 	  
// 	}
// 	//DirectRedrawGUI();
//   return 0;
// }

