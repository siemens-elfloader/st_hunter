#include <swilib.h>
#include <cfg_items.h>
#include <sieapi.h>
#include "scrtool.h"

//中文配置为 config_DATE_c.c

__root const CFG_HDR cfghdr0={CFG_CHECKBOX, "Menu Show", 0,2}; 
__root const int MENU_ENA= 1;
__root const CFG_HDR cfghdrTSI0={CFG_CHECKBOX, "Text Show", 0,2}; 
__root const int INFO_ENA= 1;
//菜单配置信息
//开始按键配置
__root const CFG_HDR cfghdr_mBAR1={CFG_LEVEL,"Main Setup",1,0};
__root const CFG_HDR cfghdrBAR_1={CFG_UINT,"Auto exit(sec)",0,20};
__root const uint AUTO_CLOSE=3;
__root const CFG_HDR cfghdrBAR_3={CFG_UINT,"Exit key",0,80};
__root const uint EXIT_BTN=RIGHT_SOFT;
__root const CFG_HDR cfghdrBAR_4={CFG_UINT,"Show key",0,80};
__root const uint CALL_BTN=RED_BUTTON;
__root const CFG_HDR cfghdrBAR_5={CFG_CBOX,"Key mode",0,3};
__root const int MODE_KBD=0;
__root const CFG_CBOX_ITEM cfgcbox03[3]={"Down","Up","LongPress"};
__root const CFG_HDR cfghdrBAR_7={CFG_UINT,"BAR Offset",0,400};
__root const uint OFFSET=0;
__root const CFG_HDR cfghdrBAR_8={CFG_COLOR,"BAR Frame",0,0};
__root const char cfgPanBorderCol[4]={255,255,255,100};
__root const CFG_HDR cfghdrBAR_9={CFG_COLOR,"BAR BKG Color",0,0};
__root const char cfgPanBGCol[4]={0,0,255,75};
__root const CFG_HDR cfghdrBAR_A={CFG_COLOR,"ICON Frame",0,0};
__root const char cfgBookBorderCol[4]={255,255,255,100};
__root const CFG_HDR cfghdrBAR_B={CFG_COLOR,"ICON BKG Color",0,0};
__root const char cfgBookBGCol[4]={255,255,255,20};
__root const CFG_HDR cfghdrBAR_C={CFG_COLOR,"APPTEXT Color",0,0};
__root const char APPTEXT_COLORS[4]={255,255,255,100};
__root const CFG_HDR cfghdrBAR_D={CFG_CBOX,"APPTEXT Font",0,17};
__root const uint APPTEXT_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxBAR[17]={FontStyle};
__root const CFG_HDR cfghdrBAR_J={CFG_CHECKBOX, "Extname show", 0,2}; 
__root const int SHOW_EXT= 1;
__root const CFG_HDR cfghdrBAR_E={CFG_STR_UTF8,"SET ICON",0,63};
__root const char AIST[64]=DEFAULT_DISK ":\\ZBin\\ScrTool\\st.png";
__root const CFG_HDR cfghdrBAR_F={CFG_STR_UTF8,"NO ICON",3,63};
__root const char AINO[64]=DEFAULT_DISK ":\\ZBin\\ScrTool\\no.png";
__root const CFG_HDR cfghdrBAR_G={CFG_STR_UTF8,"LOCK ICON",0,63};
__root const char AILK[64]=DEFAULT_DISK ":\\ZBin\\ScrTool\\lk.png";
__root const CFG_HDR cfghdrBAR_H={CFG_STR_UTF8,"REBOOT ICON",0,63};
__root const char AIRT[64]=DEFAULT_DISK ":\\ZBin\\ScrTool\\rt.png";
__root const CFG_HDR cfghdrBAR_I={CFG_STR_UTF8,"TURNOFF ICON",0,63};
__root const char AITO[64]=DEFAULT_DISK ":\\ZBin\\ScrTool\\to.png";
__root const CFG_HDR cfghdr_mBAR0={CFG_LEVEL,"",0,0};
//程序设置
__root const CFG_HDR cfghdr_mAPP1={CFG_LEVEL,"Apps Setup",1,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA041={CFG_LEVEL,"APP 01 Setup",1,0};
__root const CFG_HDR cfghdrA04_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN04[LEN]="\xE5\x85\xB6\xE5\xAE\x83\xE9\x85\x8D\xE7\xBD\xAE";
__root const CFG_HDR cfghdrA04_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI04[64]=DEFAULT_DISK ":\\ZBin\\ScrTool\\bc.png";
__root const CFG_HDR cfghdrA04_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF04[64]=DEFAULT_DISK ":\\ZBin\\ETC\\*.bcfg";
__root const CFG_HDR cfghdrA04_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT04=3; 
__root const CFG_CBOX_ITEM cfgcboxA04[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA040={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA051={CFG_LEVEL,"APP 02 Setup",1,0};
__root const CFG_HDR cfghdrA05_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN05[LEN]=NoName;
__root const CFG_HDR cfghdrA05_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI05[64]="";
__root const CFG_HDR cfghdrA05_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF05[64]="";
__root const CFG_HDR cfghdrA05_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT05=0; 
__root const CFG_CBOX_ITEM cfgcboxA05[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA050={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA061={CFG_LEVEL,"APP 03 Setup",1,0};
__root const CFG_HDR cfghdrA06_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN06[LEN]=NoName;
__root const CFG_HDR cfghdrA06_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI06[64]="";
__root const CFG_HDR cfghdrA06_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF06[64]="";
__root const CFG_HDR cfghdrA06_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT06=0; 
__root const CFG_CBOX_ITEM cfgcboxA06[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA060={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA071={CFG_LEVEL,"APP 04 Setup",1,0};
__root const CFG_HDR cfghdrA07_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN07[LEN]=NoName;
__root const CFG_HDR cfghdrA07_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI07[64]="";
__root const CFG_HDR cfghdrA07_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF07[64]="";
__root const CFG_HDR cfghdrA07_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT07=0; 
__root const CFG_CBOX_ITEM cfgcboxA07[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA070={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA081={CFG_LEVEL,"APP 05 Setup",1,0};
__root const CFG_HDR cfghdrA08_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN08[LEN]=NoName;
__root const CFG_HDR cfghdrA08_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI08[64]="";
__root const CFG_HDR cfghdrA08_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF08[64]="";
__root const CFG_HDR cfghdrA08_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT08=0; 
__root const CFG_CBOX_ITEM cfgcboxA08[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA080={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA091={CFG_LEVEL,"APP 06 Setup",1,0};
__root const CFG_HDR cfghdrA09_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN09[LEN]=NoName;
__root const CFG_HDR cfghdrA09_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI09[64]="";
__root const CFG_HDR cfghdrA09_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF09[64]="";
__root const CFG_HDR cfghdrA09_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT09=0; 
__root const CFG_CBOX_ITEM cfgcboxA09[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA090={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA101={CFG_LEVEL,"APP 07 Setup",1,0};
__root const CFG_HDR cfghdrA10_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN10[LEN]=NoName;
__root const CFG_HDR cfghdrA10_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI10[64]="";
__root const CFG_HDR cfghdrA10_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF10[64]="";
__root const CFG_HDR cfghdrA10_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT10=0; 
__root const CFG_CBOX_ITEM cfgcboxA10[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA100={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA111={CFG_LEVEL,"APP 08 Setup",1,0};
__root const CFG_HDR cfghdrA11_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN11[LEN]=NoName;
__root const CFG_HDR cfghdrA11_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI11[64]="";
__root const CFG_HDR cfghdrA11_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF11[64]="";
__root const CFG_HDR cfghdrA11_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT11=0; 
__root const CFG_CBOX_ITEM cfgcboxA11[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA110={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA121={CFG_LEVEL,"APP 09 Setup",1,0};
__root const CFG_HDR cfghdrA12_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN12[LEN]=NoName;
__root const CFG_HDR cfghdrA12_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI12[64]="";
__root const CFG_HDR cfghdrA12_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF12[64]="";
__root const CFG_HDR cfghdrA12_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT12=0; 
__root const CFG_CBOX_ITEM cfgcboxA12[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA120={CFG_LEVEL,"",0,0};
//-------------------------------------------------
__root const CFG_HDR cfghdr_mA131={CFG_LEVEL,"APP 10 Setup",1,0};
__root const CFG_HDR cfghdrA13_1={CFG_UTF8_STRING,"Label",0,LEN-1};
__root const char AN13[LEN]=NoName;
__root const CFG_HDR cfghdrA13_2={CFG_STR_UTF8,"Icon",0,63};
__root const char AI13[64]="";
__root const CFG_HDR cfghdrA13_3={CFG_STR_UTF8,"Call",0,63};
__root const char AF13[64]="";
__root const CFG_HDR cfghdrA13_4 ={CFG_CBOX,"Type", 0,ExcuteLen}; 
__root const int  AT13=0; 
__root const CFG_CBOX_ITEM cfgcboxA13[ExcuteLen]={ExcuteType};
__root const CFG_HDR cfghdr_mA130={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_mAPP0={CFG_LEVEL,"",0,0};
//TextInfo begin
__root const CFG_HDR cfghdr_mTSI1={CFG_LEVEL,"Text Setup",1,0};
//显示自定义文本
__root const CFG_HDR cfghdrTSI={CFG_UINT,"Refresh speed(sec)",0,600};
__root const uint REFRESH=10;
__root const CFG_HDR cfghdr_mTSI11={CFG_LEVEL,"Title Setup",1,0};
__root const CFG_HDR cfghdrTSI1_1={CFG_CHECKBOX, "Title Show", 0, 2};
__root const int TEXT_ENA=1;
__root const CFG_HDR cfghdrTSI1_2={CFG_COORDINATES,"Title Position",0,0};
__root const uint TEXT_X=20;
__root const uint TEXT_Y=70;
__root const CFG_HDR cfghdrTSI1_3={CFG_UTF8_STRING,"Title Value",0,LEN-1};
__root const char TEXT_FMT[LEN]="\xE7\x8E\xAB\xE7\x91\xB0\xE5\xB7\xA5\xE4\xBD\x9C\xE5\xAE\xA4";
__root const CFG_HDR cfghdrTSI1_4={CFG_COLOR,"Title Color",0,0};
__root const char TEXT_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI1_5={CFG_COLOR,"Title Frame",0,0};
__root const char TEXT_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI1_6={CFG_CBOX,"Title Font",0,17};
__root const uint TEXT_FONT=0;
__root const CFG_CBOX_ITEM cfgcboxTSI11[17]={FontStyle};
__root const CFG_HDR cfghdr_mTSI10={CFG_LEVEL,"",0,0};
//显示温度
__root const CFG_HDR cfghdr_mTSI21={CFG_LEVEL,"Temp Setup",1,0};
__root const CFG_HDR cfghdrTSI2_1={CFG_CHECKBOX, "Temp Show", 0, 2};
__root const int TEMP_ENA=1;
__root const CFG_HDR cfghdrTSI2_2={CFG_COORDINATES,"Temp Position",0,0};
__root const uint TEMP_X=3;
__root const uint TEMP_Y=25;
__root const CFG_HDR cfghdrTSI2_3={CFG_CBOX,"Temp Font",0,17};
__root const uint TEMP_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSI21[17]={FontStyle};
__root const CFG_HDR cfghdrTSI2_4={CFG_STR_WIN1251,"Temp Format",0,15};
__root const char TEMP_FMT[16]="%d.%d癈";
__root const CFG_HDR cfghdrTSI2_5={CFG_COLOR,"Temp Color",0,0};
__root const char TEMP_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI2_6={CFG_COLOR,"Temp Frame",0,0};
__root const char TEMP_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdr_mTSI20={CFG_LEVEL,"",0,0};
//显示电压
__root const CFG_HDR cfghdr_mTSI31={CFG_LEVEL,"Voltage Setup",1,0};
__root const CFG_HDR cfghdrTSI3_1={CFG_CHECKBOX, "Voltage Show", 0, 2};
__root const int VOLT_ENA=1;
__root const CFG_HDR cfghdrTSI3_2={CFG_COORDINATES,"Voltage Position",0,0};
__root const uint VOLT_X=175;
__root const uint VOLT_Y=25;
__root const CFG_HDR cfghdrTSI3_3={CFG_COLOR,"Voltage Color",0,0};
__root const char VOLT_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI3_6={CFG_COLOR,"Voltage Frame",0,0};
__root const char VOLT_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI3_4={CFG_CBOX,"Voltage Font",0,17};
__root const uint VOLT_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSI31[17]={FontStyle};
__root const CFG_HDR cfghdrTSI3_5={CFG_STR_WIN1251,"Voltage Format",0,15};
__root const char VOLT_FMT[16]="%d.%02dV";
__root const CFG_HDR cfghdr_mTSI30={CFG_LEVEL,"",0,0};
//显示内存
__root const CFG_HDR cfghdr_mTSI41={CFG_LEVEL,"AvailRAM Setup",1,0};
__root const CFG_HDR cfghdrTSI4_1={CFG_CHECKBOX, "AvailRAM Show", 0, 2};
__root const int RAM_ENA=1;
__root const CFG_HDR cfghdrTSI4_2={CFG_COORDINATES,"AvailRAM Position",0,0};
__root const uint RAM_X=0;
__root const uint RAM_Y=293;
__root const CFG_HDR cfghdrTSI4_3={CFG_COLOR,"AvailRAM Color",0,0};
__root const char RAM_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI4_5={CFG_COLOR,"AvailRAM Frame",0,0};
__root const char RAM_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI4_6={CFG_CBOX,"AvailRAM Font",0,17};
__root const uint RAM_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSI41[17]={FontStyle};
__root const CFG_HDR cfghdr_mTSI40={CFG_LEVEL,"",0,0};
//显示星期
__root const CFG_HDR cfghdr_mTSI51={CFG_LEVEL,"WEEK Setup",1,0};
__root const CFG_HDR cfghdrTSI5_1={CFG_CHECKBOX, "WEEK Show", 0, 2};
__root const int WEEK_ENA=1;
__root const CFG_HDR cfghdrTSI5_2={CFG_COORDINATES,"WEEK Position",0,0};
__root const uint WEEK_X=90;
__root const uint WEEK_Y=25;
__root const CFG_HDR cfghdrTSI5_3={CFG_COLOR,"WEEK Color",0,0};
__root const char WEEK_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI5_6={CFG_COLOR,"WEEK Frame",0,0};
__root const char WEEK_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI5_4={CFG_CBOX,"WEEK Font",0,17};
__root const uint WEEK_FONT=10;
__root const CFG_CBOX_ITEM cfgcboxTSI51[17]={FontStyle};
__root const CFG_HDR cfghdrTSI5_5={CFG_CBOX,"Format",0,6};
__root const int WEEK_FMT=4;
__root const CFG_CBOX_ITEM cfgcboxTSI52[6]={"SUNDAY", "SUN","Sunday","Sun","星期日","日"};
__root const CFG_HDR cfghdr_mTSI50={CFG_LEVEL,"",0,0};
//显示日期
__root const CFG_HDR cfghdr_mTSI61={CFG_LEVEL,"DATE Setup",1,0};
__root const CFG_HDR cfghdrTSI6_1={CFG_CHECKBOX, "DATE Show", 0, 2};
__root const int DATE_ENA=1;
__root const CFG_HDR cfghdrTSI6_2={CFG_COORDINATES,"DATE Position",0,0};
__root const uint DATE_X=85;
__root const uint DATE_Y=293;
__root const CFG_HDR cfghdrTSI6_3={CFG_COLOR,"DATE Color",0,0};
__root const char DATE_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI6_6={CFG_COLOR,"DATE Frame",0,0};
__root const char DATE_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI6_4={CFG_CBOX,"DATE Font",0,17};
__root const uint DATE_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSI61[17]={FontStyle};
__root const CFG_HDR cfghdrTSI6_5={CFG_CBOX,"DATE Format",0,10};
__root const int DATE_FMT=5;
__root const CFG_CBOX_ITEM cfgcboxTSI62[10]={DateFmt};
__root const CFG_HDR cfghdr_mTSI60={CFG_LEVEL,"",0,0};
//显示时间
__root const CFG_HDR cfghdr_mTSI71={CFG_LEVEL,"TIME Setup",1,0};
__root const CFG_HDR cfghdrTSI7_1={CFG_CHECKBOX, "TIME Show", 0, 2};
__root const int TIME_ENA=1;
__root const CFG_HDR cfghdrTSI7_2={CFG_COORDINATES,"TIME Position",0,0};
__root const uint TIME_X=30;
__root const uint TIME_Y=135;
__root const CFG_HDR cfghdrTSI7_3={CFG_COLOR,"TIME Color",0,0};
__root const char TIME_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI7_6={CFG_COLOR,"TIME Frame",0,0};
__root const char TIME_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI7_4={CFG_CBOX,"TIME Font",0,17};
__root const uint TIME_FONT=0;
__root const CFG_CBOX_ITEM cfgcboxTSI71[17]={FontStyle};
__root const CFG_HDR cfghdrTSI7_5={CFG_CBOX,"TIME Format",0,7};
__root const int TIME_FMT=3;
__root const CFG_CBOX_ITEM cfgcboxTSI72[7]={TimeFmt};
__root const CFG_HDR cfghdr_mTSI70={CFG_LEVEL,"",0,0};
//显示农历年份
__root const CFG_HDR cfghdr_mTSI81={CFG_LEVEL,"CHSYear Setup",1,0};
__root const CFG_HDR cfghdrTSI8_1={CFG_CHECKBOX, "CHSYear Show", 0, 2};
__root const int CHSYear_ENA=0;
__root const CFG_HDR cfghdrTSI8_2={CFG_COORDINATES,"CHSYear Position",0,0};
__root const uint CHSYear_X=80;
__root const uint CHSYear_Y=260;
__root const CFG_HDR cfghdrTSI8_3={CFG_COLOR,"CHSYear Color",0,0};
__root const char CHSYear_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI8_4={CFG_COLOR,"CHSYear Frame",0,0};
__root const char CHSYear_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI8_5={CFG_CBOX,"CHSYear Font",0,17};
__root const uint CHSYear_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSI81[17]={FontStyle};
__root const CFG_HDR cfghdr_mTSI80={CFG_LEVEL,"",0,0};
//显示农历日期
__root const CFG_HDR cfghdr_mTSI91={CFG_LEVEL,"CHSDay Setup",1,0};
__root const CFG_HDR cfghdrTSI9_1={CFG_CHECKBOX, "CHSDay Show", 0, 2};
__root const int CHSDay_ENA=1;
__root const CFG_HDR cfghdrTSI9_2={CFG_COORDINATES,"CHSDay Position",0,0};
__root const uint CHSDay_X=150;
__root const uint CHSDay_Y=293;
__root const CFG_HDR cfghdrTSI9_3={CFG_COLOR,"CHSDay Color",0,0};
__root const char CHSDay_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSI9_4={CFG_COLOR,"CHSDay Frame",0,0};
__root const char CHSDay_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSI9_5={CFG_CBOX,"CHSDay Font",0,17};
__root const uint CHSDay_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSI91[17]={FontStyle};
__root const CFG_HDR cfghdr_mTSI90={CFG_LEVEL,"",0,0};
//NET begin
__root const CFG_HDR cfghdr_mTSIA1={CFG_LEVEL,"NET Setup",1,0};
__root const CFG_HDR cfghdrTSIA_1={CFG_CHECKBOX, "NET Show", 0, 2};
__root const int NET_ENA=0;
__root const CFG_HDR cfghdrTSIA_2={CFG_COORDINATES,"NET Position",0,0};
__root const uint NET_X=0;
__root const uint NET_Y=50;
__root const CFG_HDR cfghdrTSIA_3={CFG_COLOR,"NET Color",0,0};
__root const char NET_COLORS[4]={0,0,0,100};
__root const CFG_HDR cfghdrTSIA_4={CFG_COLOR,"NET Frame",0,0};
__root const char NET_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSIA_5={CFG_CBOX,"NET Font",0,17};
__root const uint NET_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSIA1[17]={FontStyle};
__root const CFG_HDR cfghdrTSIA_6={CFG_STR_WIN1251,"NET Format",0,15};
__root const char NET_FMT[16]="%c%ddB";
__root const CFG_HDR cfghdr_mTSIA0={CFG_LEVEL,"",0,0};
//NET end
//AccuCAP begin
__root const CFG_HDR cfghdr_mTSIB1={CFG_LEVEL,"AccuCap Setup",1,0};
__root const CFG_HDR cfghdrTSIB_1={CFG_CHECKBOX, "AccuCap Show", 0, 2};
__root const int CAP_ENA=0;
__root const CFG_HDR cfghdrTSIB_2={CFG_COORDINATES,"AccuCap Position",0,0};
__root const uint ACCU_X=0;
__root const uint ACCU_Y=170;
__root const CFG_HDR cfghdrTSIB_3={CFG_COLOR,"AccuCap Color",0,0};
__root const char ACCU_COLORS[4]={0,0,0,80};
__root const CFG_HDR cfghdrTSIB_4={CFG_COLOR,"AccuCap Frame",0,0};
__root const char ACCU_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSIB_5={CFG_CBOX,"AccuCap Font",0,17};
__root const uint ACCU_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSIB1[17]={FontStyle};
__root const CFG_HDR cfghdrTSIB_6={CFG_STR_WIN1251,"AccuCap Format",0,15};
__root const char CAP_FMT[16]="%02d%%";
__root const CFG_HDR cfghdr_mTSIB0={CFG_LEVEL,"",0,0};
//AccuCAP end
//GPRS begin
__root const CFG_HDR cfghdr_mTSIC1={CFG_LEVEL,"GPRS Setup",1,0};
__root const CFG_HDR cfghdrTSIC_1={CFG_CHECKBOX, "GPRS Show", 0, 2};
__root const int GPRS_ENA=0;
__root const CFG_HDR cfghdrTSIC_2={CFG_COORDINATES,"GPRS Position",0,0};
__root const uint GPRS_X=0;
__root const uint GPRS_Y=210;
__root const CFG_HDR cfghdrTSIC_3={CFG_COLOR,"GPRS color",0,0};
__root const char GPRS_COLORS[4]={0,0,0,100};
__root const CFG_HDR cfghdrTSIC_6={CFG_COLOR,"GPRS Frame",0,0};
__root const char GPRS_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSIC_4={CFG_CBOX,"GPRS Font",0,17};
__root const uint GPRS_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSIC1[17]={FontStyle};
__root const CFG_HDR cfghdr_mTSIC0={CFG_LEVEL,"",0,0};
//GPRS end
//CPULoad begin
__root const CFG_HDR cfghdr_mTSID1={CFG_LEVEL,"CPULoad Setup",1,0};
__root const CFG_HDR cfghdrTSID_1={CFG_CHECKBOX, "CPULoad Show", 0, 2};
__root const int CPU_ENA=0;
__root const CFG_HDR cfghdrTSID_2={CFG_COORDINATES,"CPULoad Position",0,0};
__root const uint CPU_X=90;
__root const uint CPU_Y=293;
__root const CFG_HDR cfghdrTSID_3={CFG_COLOR,"CPULoad Color",0,0};
__root const char CPU_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrTSID_6={CFG_COLOR,"CPULoad Frame",0,0};
__root const char CPU_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrTSID_4={CFG_CBOX,"CPULoad Font",0,17};
__root const uint CPU_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxTSID1[17]={FontStyle};
__root const CFG_HDR cfghdrTSID_5={CFG_STR_WIN1251,"CPULoad Format",0,15};
__root const char CPU_FMT[16]="%02d%%";
__root const CFG_HDR cfghdr_mTSID0={CFG_LEVEL,"",0,0};
//CPULoad end
//Format begin
__root const CFG_HDR cfghdr_mTSIE1={CFG_LEVEL,"Format Setup",1,0};
__root const CFG_HDR cfghdrTSIE_1={CFG_STR_WIN1251,"Bytes Format",0,15};
__root const char BYTES_FMT[16]="%u%s";
__root const CFG_HDR cfghdrTSIE_2={CFG_STR_WIN1251,"Bytes Sign",0,7};
__root const char BYTES_SG[8]="B";
__root const CFG_HDR cfghdrTSIE_3={CFG_STR_WIN1251,"KBytes Sign",0,7};
__root const char KBYTES_SG[8]="KB";
__root const CFG_HDR cfghdr_mTSIE0={CFG_LEVEL,"",0,0};
//Format end
__root const CFG_HDR cfghdr_mTSI0={CFG_LEVEL,"",0,0};
//TextInfo end
__root const CFG_HDR cfghdr_mBIR1={CFG_LEVEL,"Holiday Setup",1,0};
__root const CFG_HDR cfghdrOBIR_1={CFG_CHECKBOX, "OldDay Show", 0, 2};
__root const int OBIR_ENA=1;
__root const CFG_HDR cfghdr_mOBIR1={CFG_LEVEL,"OldDay Setup",1,0};
__root const CFG_HDR cfghdrOBIR_2={CFG_COORDINATES,"OldDay Position",0,0};
__root const uint OBIR_X=30;
__root const uint OBIR_Y=220;
__root const CFG_HDR cfghdrOBIR_3={CFG_COLOR,"OldDay Color",0,0};
__root const char OBIR_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrOBIR_4={CFG_COLOR,"OldDay Frame",0,0};
__root const char OBIR_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrOBIR_5={CFG_CBOX,"OldDay Font",0,17};
__root const uint OBIR_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxOBIR1[17]={FontStyle};
__root const CFG_HDR cfghdr_mOBIR0={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdrNBIR_1={CFG_CHECKBOX, "NewDay Show", 0, 2};
__root const int NBIR_ENA=1;
__root const CFG_HDR cfghdr_mNBIR1={CFG_LEVEL,"NewDay Setup",1,0};
__root const CFG_HDR cfghdrNBIR_2={CFG_COORDINATES,"NewDay Position",0,0};
__root const uint NBIR_X=30;
__root const uint NBIR_Y=190;
__root const CFG_HDR cfghdrNBIR_3={CFG_COLOR,"NewDay Color",0,0};
__root const char NBIR_COLORS[4]={0,0,255,75};
__root const CFG_HDR cfghdrNBIR_4={CFG_COLOR,"NewDay Frame",0,0};
__root const char NBIR_FCOLOR[4]={255,255,255,100};
__root const CFG_HDR cfghdrNBIR_5={CFG_CBOX,"NewDay Font",0,17};
__root const uint NBIR_FONT=8;
__root const CFG_CBOX_ITEM cfgcboxNBIR1[17]={FontStyle};
__root const CFG_HDR cfghdr_mNBIR0={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdrBIRFILE={CFG_STR_UTF8,"Data FileName",0,63};
__root const char BIR_FILE[64]=DEFAULT_DISK ":\\ZBin\\ScrTool\\day.txt";
__root const CFG_HDR cfghdr_mBIR0={CFG_LEVEL,"",0,0};



