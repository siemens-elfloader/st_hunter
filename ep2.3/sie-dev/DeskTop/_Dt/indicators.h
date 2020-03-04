
void dr_null(){}

static DT_ICON dt_first={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{241,321,240,320},
	{0,0,0,0},
	NULL,
	100,
	0,
	'F',
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	0,0,1,
	dr_null,
	Enter_inf_dt,
	inf_inf_dt,
	NULL,
	NULL,
	NULL,
};

static DT_ICON dt_null={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{0,0,0,0},
	{0,0,0,0},
	NULL,
	100,
	0,
	'F',
	1,
	NULL,
	NULL,
	NULL,
	NULL,
	1,0,1,
	dr_icon,
	int_0,
	void_0,
	NULL,
	NULL,
	NULL,
};


static DT_ICON _icon={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{0,0,0,0},
	{0,0,0,0},
	NULL,
	100,
	0,
	'F',
	1,
	NULL,
	NULL,
	NULL,
	NULL,
	0,0,1,
	dr_icon,
	Enter_icon,
	inf_icon,
	NULL,
	NULL,
	NULL,
};


static DT_ICON entry_icon={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{0,0,0,0},
	{0,0,0,0},
	NULL,
	100,
	0,
	'E',
	1,
	NULL,
	NULL,
	NULL,
	NULL,
	0,0,1,
	dr_icon,
	Enter_entry,
	inf_entry,
	NULL,
	NULL,
	NULL,
};


static DT_ICON shortcut_icon={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{0,0,0,0},
	{0,0,0,0},
	NULL,
	100,
	0,
	'S',
	1,
	NULL,
	NULL,
	NULL,
	NULL,
	0,0,1,
	dr_icon,
	Enter_shortcut,
	inf_entry,
	NULL,
	NULL,
	NULL,
};


static DT_ICON mc_icon={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{0,0,0,0},
	{0,0,0,0},
	NULL,
	100,
	0,
	'm',
	1,
	NULL,
	NULL,
	NULL,
	NULL,
	0,0,1,
	dr_icon,
	Enter_mc,
	inf_mc,
	NULL,
	NULL,
	NULL,
};




static DT_ICON clock_icon={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{10,20,100,40},
	{0,0,0,0},
	NULL,
	100,
	0,
	'C',
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	1,0,1,
	dr_clock_icon,
	Enter_clock_icon,
	inf_clock_icon,
	NULL,
	NULL,
	NULL//clock_pgui
};



static DT_ICON akk_icon={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{10,20,100,40},
	{0,0,0,0},
	NULL,
	100,
	0,
	'B',
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	1,0,1,
	dr_akk_icon,
	Enter_akk_icon,
	inf_akk_icon,
	NULL,
	NULL,
	NULL,
};





static DT_ICON net_icon={
	NULL,NULL, NULL,NULL,NULL,NULL,
	{10,20,100,40},
	{0,0,0,0},
	NULL,
	100,
	0,
	'N',
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	1,0,1,
	dr_net_icon,
	Enter_net_icon,
	inf_net_icon,
	NULL,
	NULL,
	NULL,
};

// static DT_ICON dt_window={
// 	NULL,NULL, NULL,NULL,NULL,NULL, // dont touch
//  {0,0,0,0}, // position
//  {0,0,0,0},
//  NULL, // image
//  100, // transparency
//  0xFE, // number
//  0, //type
//  1, // 1-mfree when clear
//  NULL, // name
//  NULL, // popup info text
//  NULL, // image file
//  NULL, // file, shortcut or entrypoint
//  1, // 0-static icon, 1-dinamic icon
//  1, // nothing
//  dr_null, // draw function
//  int_0, // action on enter button
//  void_0, // create info text
//  NULL, // action on close (for plagins)
//  NULL, // refresh (for plagins)
//  //NULL, // methods for subgui
// };


