







typedef struct
{
  int type;
  char name[32];
  unsigned int min;
  unsigned int max;
}CFG_HDR;

typedef struct
{
  char cbox_text[32];
}CFG_CBOX_ITEM;

















































 




                                                
















__root const CFG_HDR cfghdr0={1,"UIN",0,0xFFFFFFFF};
__root const unsigned int UIN=0;

__root const CFG_HDR cfghdr1={6,"Password",0,8};
__root const char PASS[9]="";


__root const CFG_HDR cfghdr2={9,"Hosts and paths",1,0};
  __root const CFG_HDR cfghdr2_1={4,"Host",0,127};
  __root const char NATICQ_HOST[128]="cbsie.dyndns.info; freegw.alltime.ru; himoza.zyns.com; ig.boba.su";

  __root const CFG_HDR cfghdr2_2={1,"Default port",0,65535};
  __root const unsigned int NATICQ_PORT=5050;

  __root const CFG_HDR cfghdr2_3={1,"Reconnect timeout",0,65535};
  __root const unsigned int RECONNECT_TIME=10;
  
  __root const CFG_HDR cfghdr2_4={2,"History path",0,63};
  __root const char HIST_PATH[64]= "0" ":\\ZBin\\NatICQ\\history";

  __root const CFG_HDR cfghdr2_5={2,"Smiles File",0,63};
  __root const char SMILE_FILE[64]= "0" ":\\ZBin\\NatICQ\\smiles.cfg";

  __root const CFG_HDR cfghdr2_6={2,"Smiles .png path",0,63};
  __root const char SMILE_PATH[64]= "0" ":\\ZBin\\NatICQ\\smiles";

  __root const CFG_HDR cfghdr2_7={2,"Images .png path",0,63};
  __root const char ICON_PATH[64]= "0" ":\\ZBin\\NatICQ\\img";

  __root const CFG_HDR cfghdr2_8={2,"XStatus .png path",0,63};
  __root const char XSTATUSES_PATH[64]= "0" ":\\ZBin\\NatICQ\\ximg";

  __root const CFG_HDR cfghdr2_9={2,"Templates path",0,63};
  __root const char TEMPLATES_PATH[64]= "0" ":\\ZBin\\NatICQ\\templates";
__root const CFG_HDR cfghdr3={9,"",0,0};


__root const CFG_HDR cfghdr4={9,"Interface",1,0};

  __root const CFG_HDR cfghdr4_1={7,"Idle icon position",0,0};
  __root const unsigned int IDLEICON_X=0;
  __root const unsigned int IDLEICON_Y=50;


  __root const CFG_HDR cfghdr4_2={5,"Popup",0,3};
  __root const int DEVELOP_IF = 2;
  __root const CFG_CBOX_ITEM cfgcbox15_1[3]={"Always", "If Kbd Unlock","Never"};

  
  __root const CFG_HDR cfghdr4_3={5,"Sort CList",0,2};
  __root const int SORT_CLIST = 0;
  __root const CFG_CBOX_ITEM cfgcbox4_3[2]={"By Name","By Status"};

  __root const CFG_HDR cfghdr4_4={5,"First letter",0,2};
  __root const int FIRST_LETTER = 0;
  __root const CFG_CBOX_ITEM cfgcbox4_4[2]={"Small","Big"};

  
  __root const CFG_HDR cfghdr4_5={9,"Font and colors",1,0};
  
    __root const CFG_HDR cfghdr4_5_1={1,"My string color",0,1000};
    __root const unsigned int I_COLOR=3;
  
    __root const CFG_HDR cfghdr4_5_2={1,"Your string color",0,1000};
    __root const unsigned int TO_COLOR=2;
  
    __root const CFG_HDR cfghdr4_5_3={1,"XStatus color",0,1000};
    __root const unsigned int X_COLOR=2;
  
    __root const CFG_HDR cfghdr4_5_4={1,"Main font size",0,6};
    __root const unsigned int ED_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_5={1,"Header font size",0,6};
    __root const unsigned int ED_H_FONT_SIZE=2;
  
    __root const CFG_HDR cfghdr4_5_6={1,"XStatus font size",0,6};
    __root const unsigned int ED_X_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_7={1,"My old string color",0,1000};
    __root const unsigned int O_I_COLOR=15;
  
    __root const CFG_HDR cfghdr4_5_8={1,"Your old string color",0,1000};
    __root const unsigned int O_TO_COLOR=14;
    
    __root const CFG_HDR cfghdr4_5_9={1,"XStatus old color",0,1000};
    __root const unsigned int O_X_COLOR=14;
  
    __root const CFG_HDR cfghdr4_5_10={1,"Main old font size",0,6};
    __root const unsigned int O_ED_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_11={1,"Header old font size",0,6};
    __root const unsigned int O_ED_H_FONT_SIZE=2;
  
    __root const CFG_HDR cfghdr4_5_12={1,"XStatus old font size",0,6};
    __root const unsigned int O_ED_X_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_13={1,"Acked color",0,1000};
    __root const unsigned int ACK_COLOR=4;
  
    __root const CFG_HDR cfghdr4_5_14={1,"Unacked color",0,1000};
    __root const unsigned int UNACK_COLOR=20;
  

  __root const CFG_HDR cfghdr4_6={9,"",0,0};  

  
  __root const CFG_HDR cfghdr4_7={9,"Illumination setup",1,0};
    __root const CFG_HDR cfghdr4_7_1={3,"Display on Recv",0,100};
    __root const unsigned int ILL_DISP_RECV=10;
  
    __root const CFG_HDR cfghdr4_7_2={3,"Keys on Recv",0,100};
    __root const unsigned int ILL_KEYS_RECV=10;
  
    __root const CFG_HDR cfghdr4_7_3={3,"Timeout on Recv",0,60};
    __root const unsigned int ILL_RECV_TMR=10;
  
    __root const CFG_HDR cfghdr4_7_4={3,"Fade on Recv",0,1000};
    __root const unsigned int ILL_RECV_FADE=256;
  
    __root const CFG_HDR cfghdr4_7_5={3,"Display on Send",0,100};
    __root const unsigned int ILL_DISP_SEND=10;
  
    __root const CFG_HDR cfghdr4_7_6={3,"Keys on Send",0,100};
    __root const unsigned int ILL_KEYS_SEND=0;
  
    __root const CFG_HDR cfghdr4_7_7={3,"Timeout on Send",0,60};
    __root const unsigned int ILL_SEND_TMR=5;
  
    __root const CFG_HDR cfghdr4_7_8={3,"Fade on Send",0,1000};
    __root const unsigned int ILL_SEND_FADE=256;
  
    __root const CFG_HDR cfghdr4_7_9={3,"Fade on Off",0,1000};
    __root const unsigned int ILL_OFF_FADE=256;
  __root const CFG_HDR cfghdr4_8={9,"",0,0};  
 
__root const CFG_HDR cfghdr5={9,"",0,0};







__root const CFG_HDR cfghdr6={9,"History",1,0};


    __root const CFG_HDR cfghdr6_1={5,"Enable logs",0,2};
    __root const int LOG_ALL = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_1[2]={"No","Yes"};


    __root const CFG_HDR cfghdr6_2={5,"History for",0,2};
    __root const int HISTORY_TYPE = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_2[2]={"All","Everyone"};
  

    __root const CFG_HDR cfghdr6_3={5,"Log status changes",0,2};
    __root const int LOG_STATCH = 0;
    __root const CFG_CBOX_ITEM cfgcbox6_3[2]={"No","Yes"};


    __root const CFG_HDR cfghdr6_4={5,"Log X-Text",0,2};
    __root const int LOG_XTXT = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_4[2]={"No","Yes"};
  

    __root const CFG_HDR cfghdr6_5={5,"Auto Request XText",0,2};
    __root const int ENA_AUTO_XTXT = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_5[2]={"No","Yes"};


    __root const CFG_HDR cfghdr6_6={5,"Not log same XText",0,2};
    __root const int NOT_LOG_SAME_XTXT = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_6[2]={"No","Yes"};


    __root const CFG_HDR cfghdr6_7={5,"History read buffer",0,6};
    __root const int HISTORY_BUFFER = 4;
    __root const CFG_CBOX_ITEM cfgcbox6_7[6]={"Disabled","128 bytes","256 bytes","512 bytes","1 kbyte","2 kbyte"};
  
__root const CFG_HDR cfghdr7={9,"",0,0};



__root const CFG_HDR cfghdr8={9,"Notify power",1,0};

  __root const CFG_HDR cfghdr8_1={1,"Sound Volume",0,6};
  __root const unsigned int sndVolume=3;

  __root const CFG_HDR cfghdr8_2={1,"Vibra power",0,100};
  __root const unsigned int vibraPower=30;

  __root const CFG_HDR cfghdr8_3={5,"Vibration type",0,2};
  __root const int VIBR_TYPE = 1;
  __root const CFG_CBOX_ITEM cfgcbox8_3[2]={"Single","Double"};

  __root const CFG_HDR cfghdr8_4={5,"Vibra on connect",0,2};
  __root const int VIBR_ON_CONNECT = 1;
  __root const CFG_CBOX_ITEM cfgcbox8_4[2]={"No","Yes"};

  __root const CFG_HDR cfghdr8_5={9,"Sounds setup",1,0};
  

    __root const CFG_HDR cfghdr8_5_1={2,"snd Startup",0,63};
    __root const char sndStartup[64]= "0" ":\\ZBin\\NatICQ\\Sounds\\sndStartup.wav";
  
    __root const CFG_HDR cfghdr8_5_2={2,"snd SrvMsg",0,63};
    __root const char sndSrvMsg[64]= "0" ":\\ZBin\\NatICQ\\Sounds\\sndSrvMsg.wav";
  
    __root const CFG_HDR cfghdr8_5_3={2,"snd Global",0,63};
    __root const char sndGlobal[64]= "0" ":\\ZBin\\NatICQ\\Sounds\\sndGlobal.wav";
  
    __root const CFG_HDR cfghdr8_5_4={2,"snd Msg",0,63};
    __root const char sndMsg[64]= "0" ":\\ZBin\\NatICQ\\Sounds\\sndMsg.wav";
  
    __root const CFG_HDR cfghdr8_5_5={2,"snd MsgSent",0,63};
    __root const char sndMsgSent[64]= "0" ":\\ZBin\\NatICQ\\Sounds\\sndMsgSent.wav";
    
  __root const CFG_HDR cfghdr8_6={9,"",0,0};
  
__root const CFG_HDR cfghdr9={9,"",0,0};

