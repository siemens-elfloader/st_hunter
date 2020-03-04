
/*============================================*/




SOFTKEY_DESC menu_sk[]=
{
	{0x0018,0x0000,(int)""},
	{0x0001,0x0000,(int)"выход"},
	{0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt=
{
	menu_sk,0
};

void ed1_locret(void){}

// =================================================;

// =================================================;





// =================================================;


int open_filer(void *ed_gui, int type)
{
	int len=strlen(Dt.rootadr);
	char *tx=malloc(256);
	zeromem(tx, 256);
	sprintf(tx,"%saddons\\filer.elf",Dt.rootadr);
	//memcpy(tx, Dt.rootadr, len);
	//char *s=tx+len;
	//while (*s!='\\') s--;
	//strcpy(tx+len,"filer.elf");
	//*(s+10)='\0';
	wsprintf(ews,perc_t,tx);
	mfree(tx);
	int t[2];
	t[0]=(int)ed_gui;
	t[1]=type;
	ExecuteFile(ews, NULL, &t);
	return 0;
}

void on_utf8ec(USR_MENU_ITEM *item)
{
  if (item->type==0)
  {
    switch(item->cur_item)
    {
    case 0:
      wsprintf(item->ws,perc_t,"SelectFolder");
      break;
    case 1:
      wsprintf(item->ws,perc_t,"SelectFile");
      break;
    }
  }
  if (item->type==1)
  {
    switch(item->cur_item)
    {
    case 0:
	  open_filer(item->user_pointer, 0);
      //open_select_file_gui(item->user_pointer, 0);
      break;
    case 1:
      //open_select_file_gui(item->user_pointer, 1);
	  open_filer(item->user_pointer, 1);
      break;
    }
  }   
}

// =================================================;


int ed_onkey(GUI *data, GUI_MSG *msg)
{
	EDITCONTROL ec;
	char txt[128];
	unsigned int ul;
	// char txtpos;
	// if(EDIT_GetUserPointer(data)) txtpos=6;
	// else txtpos=4;
	DT_ICON *TI,*TTI=EDIT_GetUserPointer(data);
	if (msg->keys==0xFFF)
	{
		if(TTI) TI=TTI;
		else TI=malloc(sizeof(DT_ICON));
		if(TTI)
		{
			zeromem(txt,128);
			char *s=translit(TI->nm);
			sprintf(txt,"%sicons\\%s.dti",Dt.rootadr,s);
			mfree(s); s=NULL;
			unlink(txt, &ul);
		}
		EDIT_SetFocus(data, 2);
		switch(EDIT_GetItemNumInFocusedComboBox(data))
		{
		 case 1:
			TI->type='F';
			if(!TTI) memcpy(TI, &_icon, sizeof(DT_ICON));
			else
			{
				TI->d=_icon.d;
				TI->dyn=_icon.dyn;
				TI->sel=_icon.sel;
				TI->onDraw=_icon.onDraw;
				TI->onEnter=_icon.onEnter;
				TI->onInf=_icon.onInf;
			}
			break;
		 case 2:
			TI->type='E';
			if(!TTI) memcpy(TI, &entry_icon, sizeof(DT_ICON));
			else
			{
				TI->d=entry_icon.d;
				TI->dyn=entry_icon.dyn;
				TI->sel=entry_icon.sel;
				TI->onDraw=entry_icon.onDraw;
				TI->onEnter=entry_icon.onEnter;
				TI->onInf=entry_icon.onInf;
			}
			break;
		 case 3:
			TI->type='S';
			if(!TTI) memcpy(TI, &shortcut_icon, sizeof(DT_ICON));
			else
			{
				TI->d=shortcut_icon.d;
				TI->dyn=shortcut_icon.dyn;
				TI->sel=shortcut_icon.sel;
				TI->onDraw=shortcut_icon.onDraw;
				TI->onEnter=shortcut_icon.onEnter;
				TI->onInf=shortcut_icon.onInf;
			}
			break;
		 case 4:
			TI->type='m';
			if(!TTI) memcpy(TI, &mc_icon, sizeof(DT_ICON));
			else
			{
				TI->d=mc_icon.d;
				TI->dyn=mc_icon.dyn;
				TI->sel=mc_icon.sel;
				TI->onDraw=mc_icon.onDraw;
				TI->onEnter=mc_icon.onEnter;
				TI->onInf=mc_icon.onInf;
			}
			break;
		 case 5:
			TI->type='C';
			if(!TTI) TI=&clock_icon;
			break;
		 case 6:
			TI->type='B';
			if(!TTI) TI=&akk_icon;
			break;
		 case 7:
			TI->type='N';
			if(!TTI) TI=&net_icon;
			break;
		 case 8:
			//TI->type='P';
			if(!TTI)
			{
			 memcpy(TI, &dt_null, sizeof(DT_ICON));
			 TI->d=0xFF;
			 TI->type='P';
			}
			else
			{
				//TI->d=dt_null.d;
				TI->dyn=dt_null.dyn;
				TI->sel=dt_null.sel;
				TI->onDraw=dt_null.onDraw;
				TI->onEnter=dt_null.onEnter;
				TI->onInf=dt_null.onInf;
				TI->d=0xFF;
				TI->type='P';
			}
			break;
		}
		
		ExtractEditControl(data,4,&ec);
		if(TTI) mfree(TI->nm);
		TI->nm=malloc(wstrlen(ec.pWS)+1);
		zeromem(TI->nm,wstrlen(ec.pWS)+1);
		ws2str(ec.pWS, TI->nm);
		
		ExtractEditControl(data,6,&ec);
		if(TTI) mfree(TI->fl);
		TI->fl=malloc(wstrlen(ec.pWS)+1);
		zeromem(TI->fl,wstrlen(ec.pWS)+1);
		ws2str(ec.pWS, TI->fl);
		
		ExtractEditControl(data,8,&ec);
		if(TTI) if(TI->img_fl) mfree(TI->img_fl);
		if(wstrlen(ec.pWS))
		{
			TI->img_fl=malloc(wstrlen(ec.pWS)+1);
			zeromem(TI->img_fl,wstrlen(ec.pWS)+1);
			ws2str(ec.pWS, TI->img_fl);
		}
		
		if(!TTI)
		{
			StoreXYWHtoRECT(&TI->p,cur.x,cur.y, 32,32);
		}
		
		if(TI->img)
		{
			if(TI->img->bitmap) mfree(TI->img->bitmap);
			mfree(TI->img);
		}
			TI->img=NULL;
		
		if(TI->img_fl)
		{
#if NEWSGOLD || X75
			TI->img=CreateIMGHDRFromPngFile(TI->img_fl, 3);
#else
			TI->img=Dt.ef->create_imghdr(TI->img_fl, 3);
#endif
			TI->p.x2=TI->p.x+TI->img->w;//GetImgWidth((int)TI->img_fl);
			TI->p.y2=TI->p.y+TI->img->h;//GetImgHeight((int)TI->img_fl);
			// TI->img=malloc(wstrlen(ec.pWS)+1);
			// zeromem(TI->img_fl,wstrlen(ec.pWS)+1);
			// ws2str(ec.pWS, TI->img_fl);
		}
		
		
		// saveicon(TI);
		if(!TTI)
		{
			TI->inf=malloc(256);
			zeromem(TI->inf, 256);
			add_icon(TI);
			TI->n=((DT_ICON*)(TI->prev))->n+1;
		}
		// {
			// mfree(TI->nm);
			// TI->nm=NULL;
			// mfree(TI->fl);
			// TI->fl=NULL;
			// mfree(TI->img_fl);
			// TI->img_fl=NULL;
			// if(TI->img)
			// {
				// if(TI->img->bitmap) mfree(TI->img->bitmap);
				// mfree(TI->img);
				// TI->img=NULL;
			// }
			// if((TI->type!='B')&&(TI->type!='N')&&(TI->type!='C')) mfree(TI);
		// }
		//refresh_dt();
			SUBPROC((void*)saveicon, TI);
			// ICON_REDRAW(&Dt, TI);
		fill_icons(3);
		
		return 1;
	}
	
	
	
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    int l=msg->gbsmsg->submess;
    int i=EDIT_GetFocus(data);
    if ((i==6)||(i==8))
    {
        if (l==ENTER_BUTTON)
        {
            EDIT_OpenOptionMenuWithUserItems(data,on_utf8ec,data,2);
            return (-1);
        }
	}
  }
	
	
	// if (msg->gbsmsg->msg==KEY_DOWN)
	// {
		// if (msg->keys==0xFFF)
		// {
			// if(!wstrlen(ec.pWS))
			// {
				// EDIT_SetFocus(data, 2);
				// MsgBoxError(1, (int)LG_ENT_NUM);
				// return -1;
			// }
			// OUT_SMS *tmp=malloc(sizeof(OUT_SMS));
			// zeromem(tmp,sizeof(OUT_SMS));
			// ws_2str(ec.pWS, tmp->num, wstrlen(ec.pWS));
			// ExtractEditControl(data,txtpos,&ec);
			// if(!wstrlen(ec.pWS))
			// {
				// EDIT_SetFocus(data, txtpos);
				// mfree(tmp);
				// MsgBoxError(1, (int)LG_ENT_TEXT);
				// return -1;
			// }
			// tmp->ws=AllocWS(ec.pWS->wsbody[0]);
			// wstrcpy(tmp->ws,ec.pWS);
			// tmp->up=EDIT_GetUserPointer(data);
			// outsms_ed(tmp);
			// return(1);
		// }
		// if (msg->gbsmsg->submess==ENTER_BUTTON)
		// {
			// if(EDIT_GetFocus(data)==2)
			// {
				// EDIT_OpenOptionMenuWithUserItems(data,nom_options,data,1);
				// return (-1);
			// }
			// if(EDIT_GetFocus(data)==txtpos)
			// {
				// EDIT_OpenOptionMenuWithUserItems(data,tr_options,data,1);
				// return (-1);
			// }
		// }
	// }
	
  //-1 - do redraw
	return(0); //Do standart keys
  //1: close
}


static const SOFTKEY_DESC sk_send0={0x0FFF,0x0000,(int)"Ok"};

void ed_ghook(GUI *data, int cmd)
{
	int i;
//  static SOFTKEY_DESC sk={0x0018,0x0000,(int)"Menu"};
// 	static const SOFTKEY_DESC sk_cancel={0x0FF0,0x0000,(int)LG_CLOSE};
	EDITCONTROL ec;
	//DT_ICON TI;
// 	EDITC_OPTIONS ec_options;
// 	int cnt, len;
	switch(cmd)
	{
		case TI_CMD_CREATE:
			break;
		case TI_CMD_FOCUS:
			DisableIDLETMR();
			break;
		case TI_CMD_REDRAW:
			SetSoftKey(data,&sk_send0,SET_SOFT_KEY_N);
			break;
			
		case TI_CMD_COMBOBOX_FOCUS://0x0D
			//onCombo
			if (i=EDIT_GetItemNumInFocusedComboBox(data))
			{
				switch(i)
				{
				 case 1: wsprintf(ews,"File"); break;
				 case 2: wsprintf(ews,"Entrypoint"); break;
				 case 3: wsprintf(ews,"Shortcut"); break;
				 case 4: wsprintf(ews,"Folder"); break;
				 case 5: wsprintf(ews,"Clock"); break;
				 case 6: wsprintf(ews,"Battery-indicator"); break;
				 case 7: wsprintf(ews,"Net-indicator"); break;
				 case 8: wsprintf(ews,"Plugin"); break;
				}
				// if((int)TI->onEnter==(int)&Enter_icon) s[3]='F'; else
				// if((int)TI->onEnter==(int)&Enter_entry) s[3]='E'; else
				// if((int)TI->onEnter==(int)&Enter_shortcut) s[3]='S'; else
				// if((int)TI->onEnter==(int)&Enter_mc) s[3]='m'; else
				
				// if((int)TI->onEnter==(int)&Enter_clock_icon) s[3]='C'; else
				// if((int)TI->onEnter==(int)&Enter_akk_icon) s[3]='B'; else
				// if((int)TI->onEnter==(int)&Enter_net_icon) s[3]='N';
				// else s[3]='P';
			  // wsprintf(ews,"%t%d","Пункт: ",i);
			}
			else
			{
			  ExtractEditControl(data,EDIT_GetFocus(data)-1,&ec);
			  wstrcpy(ews,ec.pWS);
			}
			EDIT_SetTextToFocused(data,ews);
			break;
	}
}

HEADER_DESC ed_hdr={0,0,0,0,NULL,NULL,LGP_NULL};

INPUTDIA_DESC ed_desc=
{
	1,
 ed_onkey,
 ed_ghook,
 (void *)ed1_locret,
  0,
  &menu_skt,
  {0,0,0,0},
  FONT_SMALL,
  100,
  101,
  0,

//  0x00000001 - Выровнять по правому краю
//  0x00000002 - Выровнять по центру
//  0x00000004 - Инверсия знакомест
//  0x00000008 - UnderLine
//  0x00000020 - Не переносить слова
//  0x00000200 - bold
  0,

//  0x00000002 - ReadOnly
//  0x00000004 - Не двигается курсор
  0x40000000 // Поменять местами софт-кнопки
};

int edit_icon(DT_ICON *TI)
{
	void *ma=malloc_adr();
	void *eq;
	EDITCONTROL ec;
	EDITC_OPTIONS ec_options;
	int i;

	PrepareEditControl(&ec);
	eq=AllocEQueue(ma,mfree_adr());
	
	if(!TI) ed_hdr.lgp_id=(int)"New icon";
	else ed_hdr.lgp_id=(int)TI->nm;
	
// 	Type
	wsprintf(ews,perc_t,"Type");
	ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
	PrepareEditCOptions(&ec_options);
	//SetPenColorToEditCOptions(&ec_options,2);
	SetFontToEditCOptions(&ec_options,2);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
	if(!TI) i=1;
	else
	switch(TI->type)
	{
		case 'F': i=1; break;
		case 'E': i=2; break;
		case 'S': i=3; break;
		case 'm': i=4; break;
		
		case 'C': i=5; break;
		case 'B': i=6; break;
		case 'N': i=7; break;
		case 'P': i=8; break;
	}
	
	ConstructComboBox(&ec,ECT_COMBO_BOX,ECF_APPEND_EOL,ews,64,0,8,i);
	//PrepareEditCOptions(&ec_options);
	// SetFontToEditCOptions(&ec_options,1);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
// 	Name
	wsprintf(ews,perc_t,"Name");
	ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
	PrepareEditCOptions(&ec_options);
	//SetPenColorToEditCOptions(&ec_options,2);
	SetFontToEditCOptions(&ec_options,2);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
	if(!TI) wsprintf(ews,perc_t,"");
	else wsprintf(ews,perc_t,TI->nm);
	ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ews,64);
	PrepareEditCOptions(&ec_options);
	SetFontToEditCOptions(&ec_options,1);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
	
//	File
	wsprintf(ews,perc_t,"File");
	ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
	PrepareEditCOptions(&ec_options);
	//SetPenColorToEditCOptions(&ec_options,2);
	SetFontToEditCOptions(&ec_options,2);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
	if((!TI)||(!TI->fl)) wsprintf(ews,perc_t,"");
	else wsprintf(ews,perc_t,TI->fl);
	ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ews,64);
	PrepareEditCOptions(&ec_options);
	SetFontToEditCOptions(&ec_options,1);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
	
//	Image
	wsprintf(ews,perc_t,"Image");
	ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
	PrepareEditCOptions(&ec_options);
	//SetPenColorToEditCOptions(&ec_options,2);
	SetFontToEditCOptions(&ec_options,2);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
	if((!TI)||(!TI->img_fl)) wsprintf(ews,perc_t,"");
	else wsprintf(ews,perc_t,TI->img_fl);
	ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ews,64);
	PrepareEditCOptions(&ec_options);
	SetFontToEditCOptions(&ec_options,1);
	CopyOptionsToEditControl(&ec,&ec_options);
	AddEditControlToEditQend(eq,&ec,ma);
	
	
	
	patch_header(&ed_hdr);
	patch_input(&ed_desc);

	return CreateInputTextDialog(&ed_desc,&ed_hdr,eq,1,TI);
}


