#include "..\inc\cfg_items.h"
#include "language.h"
#include "swilib.h"
//������������

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

//#ifdef BCFG
__root const CFG_HDR cfghdr0={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr31={CFG_LEVEL,"������������ ���������",1,0};

//Popup chat
  __root const CFG_HDR cfghdr4_2={CFG_CBOX,"Popup",0,2};
  __root const int DEVELOP_IF_MOD = 1;
  __root const CFG_CBOX_ITEM cfgcbox15_1[2]={"Use set orig.config", "After unlock"};
    
  __root const CFG_HDR cfghdr6_10={CFG_CBOX,"Save history with name",0,2};
  __root const int HISTORY_SAVE_TYPE = 0;
  __root const CFG_CBOX_ITEM cfgcbox6_9[2]={"No","Yes"};

//Buffer for fill from history
    __root const CFG_HDR cfghdr6_7={CFG_CBOX,"History read buffer",0,3};
    __root const int HISTORY_BUFFER_MOD = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_7[3]={"Use set orig.conf.","4 kbyte","8 kbyte"};    
    
__root const CFG_HDR cfghdr30={CFG_LEVEL,"",0,0};


//-----------------------------------------------------------------------------//
//                                   ���������                                 //
//-----------------------------------------------------------------------------//

__root const CFG_HDR cfghdrI_1={CFG_LEVEL,"���������",1,0};

  __root const CFG_HDR cfghdrI_4 = {CFG_UINT, "���-�� ��������� � ����", 0, 75};  
  __root const unsigned int MAXLOGMSG = 20;    
  
  
   __root const CFG_HDR cfghdrI_2={CFG_CHECKBOX,"��� �������",0,2};
#ifdef ELKA
   __root const int VIEW_MODE = 1;
#else
   __root const int VIEW_MODE = 0;
#endif
          
  __root const CFG_HDR cfghdrI_3={CFG_CHECKBOX,"��� �����",0,2};
  __root const int LINE_MODE = 0;
  
  __root const CFG_HDR cfghdrI_7={CFG_CBOX,"����������",0,3};
  __root const unsigned int SOFT_MODE = 1;
  __root const CFG_CBOX_ITEM cfgcboxI_8[3]={"������","�����","����"};
  
  __root const CFG_HDR cfghdrI_5={CFG_CHECKBOX,"������� � ��������",0,2};
  __root const unsigned int ShowStatusToIconBar = 1;
  
  __root const CFG_HDR cfghdrI_6={CFG_CHECKBOX,"���� � ��������",0,2};
  __root const unsigned int PingToIconBar = 0;
  
//  __root const CFG_HDR cfghdrI_8={CFG_CHECKBOX,"���������� ��������",0,2};
  //__root const unsigned int ShowProcessSend = 0;    

  __root const CFG_HDR cfghdrI_8={CFG_CBOX,"���������� ��������",0,4};
  __root const unsigned int ShowProcessSend = 1;
  __root const CFG_CBOX_ITEM cfgcboxI_9[4]={"������","�������","������","�����+����"};  
  
  //history
  __root const CFG_HDR cfghdr6_8={CFG_CBOX,"���������� �������� ����",0,2};
  __root const int SHOW_ACTIVE = 1;
  __root const CFG_CBOX_ITEM cfgcbox6_8[2]={"No","Yes"};  
  
  __root const CFG_HDR cfghdrI_10={CFG_CBOX,"�������� �������� ����",0,2};
  __root const int SelBoldActiveChat = 1;
  __root const CFG_CBOX_ITEM cfgcboxI_11[2]={"�������","������"};  
    
  __root const CFG_HDR cfghdrI_9={CFG_CHECKBOX,"��������� ��� ����������� ����.",0,2};
  __root const unsigned int IllByONCantact = 1;
          
#ifdef ELKA  
  __root const CFG_HDR cfghdr2_7={CFG_STR_UTF8,"Small images .png path",0,63};
  __root const char ICON_PATH2[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\img2";
#endif

//------------------------------------------------------------------------------//
//                         SETTING CONTACTLIST HEADER                           //
//------------------------------------------------------------------------------//

__root const CFG_HDR cfghdr_s1={CFG_LEVEL,"��������� ��",1,0};

 
    __root const CFG_HDR cfghdrs1_s1={CFG_UINT,"����� �����./������.(���)",0,1000};
    __root const unsigned int ON_OFF_TIME=10;  
  
  
    __root const CFG_HDR cfghdrHead_1={CFG_CHECKBOX,"������",0,2};
    __root const int HEAD_STATUS = 1;
  
    __root const CFG_HDR cfghdrHead_2={CFG_CHECKBOX,"�-������",0,2};
    __root const int HEAD_XSTATUS = 1;
  
    __root const CFG_HDR cfghdrHead_3={CFG_CHECKBOX,"������.������",0,2};
    __root const int HEAD_PRIVATE = 1;
  
    __root const CFG_HDR cfghdrHead_4={CFG_CBOX,"����",0,3};
    __root const int HEAD_PING = 1;
    __root const CFG_CBOX_ITEM cfgcboxs15[3]={"��������","�����","������"};
    
    __root const CFG_HDR cfghdrs1_15={CFG_CHECKBOX,"���������� �������",0,2};
    __root const int ShowTraff = 1;
  //    __root const CFG_CBOX_ITEM cfgcboxs15[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_11={CFG_CHECKBOX,"���������� ���-�� ���������",0,2};
    __root const int isShowCont = 1;
  //  __root const CFG_CBOX_ITEM cfgcboxs11[2]={"No","Yes"};
  
    __root const CFG_HDR cfghdrs1_12={CFG_CHECKBOX,"���������� ���-�� ��������.",0,2};
    __root const int isShowUnread = 1;
    //__root const CFG_CBOX_ITEM cfgcboxs12[2]={"No","Yes"};
  
    __root const CFG_HDR cfghdrs1_4={CFG_CHECKBOX,"���������� ����� [V] ",0,2};
    __root const int isShowV = 1;
    //__root const CFG_CBOX_ITEM cfgcboxs3[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_5={CFG_CHECKBOX,"���������� ���� [S]",0,2};
    __root const int isShowS = 1;
    //_root const CFG_CBOX_ITEM cfgcboxs5[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_6={CFG_CHECKBOX,"���������� ���������� [A]",0,2};
    __root const int isShowA = 1;
  //  __root const CFG_CBOX_ITEM cfgcboxs6[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_7={CFG_CHECKBOX,"���������� ����������� [Vr]",0,2};
    __root const int isShowR = 1;
    //__root const CFG_CBOX_ITEM cfgcboxs7[2]={"No","Yes"};
    
    
    __root const CFG_HDR cfghdrs1_9={CFG_CHECKBOX,"���������� ����� �������",0,2};
    __root const int isShowBat = 0;
   // __root const CFG_CBOX_ITEM cfgcboxs9[2]={"No","Yes"};    
  
__root const CFG_HDR cfghdr_s0={CFG_LEVEL,"",0,0};

    
    
  __root const CFG_HDR cfghdrCl_1={CFG_LEVEL,"������� ����",1,0};   

     __root const CFG_HDR cfghdrrCl_2={CFG_COLOR,"���� ���� ��",0,0};
     __root const char CLBackgrClr[4]={0x00,0x00,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_3={CFG_COLOR,"���� ������",0,0};
     __root const char SoftBackgrClr[4]={0xFF,0x48,0x00,0x1B};   
     
     __root const CFG_HDR cfghdrrCl_4={CFG_COLOR,"���� �������� ������",0,0};
     __root const char SoftFontClr[4]={0xFF,0xFF,0xFF,0x64};
     
     __root const CFG_HDR cfghdrrCl_5={CFG_COLOR,"���� ���������",0,0};
     __root const char HeadBackgrClr[4]={0xFF,0x48,0x00,0x1B};   
     
     __root const CFG_HDR cfghdrrCl_6={CFG_COLOR,"���� �������� ���������",0,0};
     __root const char HeadFontClr[4]={0xFF,0xFF,0xFF,0x64};
     
     __root const CFG_HDR cfghdrrCl_7={CFG_COLOR,"���� ����������",0,0};
     __root const char ScrollBarClr[4]={0xFF,0x00,0x00,0x15};
     
     __root const CFG_HDR cfghdrrCl_8={CFG_COLOR,"���� �������",0,0};
     __root const char ClCursorClr[4]={0xFF,0x00,0x00,0x32};
     
     __root const CFG_HDR cfghdrrCl_9={CFG_COLOR,"���� ����� �������",0,0};
     __root const char ClCursorBordClr[4]={0x00,0x00,0x00,0x60};
     
     __root const CFG_HDR cfghdrrCl_10={CFG_COLOR,"���� ��������",0,0};
     __root const char ActiveClr[4]={0xFF,0xFF,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_11={CFG_COLOR,"���� ����������",0,0};
     __root const char NotActiveClr[4]={0x00,0xFF,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_16={CFG_COLOR,"���� �����",0,0};
     __root const char GroupClr[4]={0xFF,0x00,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_17={CFG_COLOR,"���� ��������",0,0};
     __root const char CLDescricpClr[4]={0xFF,0x00,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_12={CFG_COLOR,"��� ���.����",0,0};
     __root const char AddInfoClr[4]={0xFF,0x00,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_13={CFG_COLOR,"����� ���.����",0,0};
     __root const char FontInfoClr[4]={0xFF,0xFF,0xFF,0x64};
     
     __root const CFG_HDR cfghdrrCl_14={CFG_COLOR,"���� �� ������������",0,0};
     __root const char UnAckedClr[4]={0xFF,0xFF,0x0,0x15};
     
     __root const CFG_HDR cfghdrrCl_15={CFG_COLOR,"���� ������������",0,0};
     __root const char SendClr[4]={0x00,0xFF,0x00,0x15};
  /*      
     __root const CFG_HDR cfghdrrCl_2={CFG_COLOR,"���� ���� ��",0,0};
     __root const char CLBackgrClr[4]={0xFF,0xFF,0xFF,0x64};
     
     __root const CFG_HDR cfghdrrCl_3={CFG_COLOR,"���� ������",0,0};
     __root const char SoftBackgrClr[4]={0xFF,0x00,0x00,0x15};   
     
     __root const CFG_HDR cfghdrrCl_4={CFG_COLOR,"���� �������� ������",0,0};
     __root const char SoftFontClr[4]={0xFF,0xFF,0xFF,0x64};
     
     __root const CFG_HDR cfghdrrCl_5={CFG_COLOR,"���� ���������",0,0};
     __root const char HeadBackgrClr[4]={0xFF,0x00,0x00,0x15};   
     
     __root const CFG_HDR cfghdrrCl_6={CFG_COLOR,"���� �������� ���������",0,0};
     __root const char HeadFontClr[4]={0xFF,0xFF,0xFF,0x64};
     
     __root const CFG_HDR cfghdrrCl_7={CFG_COLOR,"���� ����������",0,0};
     __root const char ScrollBarClr[4]={0xFF,0x00,0x00,0x15};
     
     __root const CFG_HDR cfghdrrCl_8={CFG_COLOR,"���� �������",0,0};
     __root const char ClCursorClr[4]={0xFF,0x00,0x00,0x20};
     
     __root const CFG_HDR cfghdrrCl_9={CFG_COLOR,"���� ����� �������",0,0};
     __root const char ClCursorBordClr[4]={0x00,0x00,0x00,0x60};
     
     __root const CFG_HDR cfghdrrCl_10={CFG_COLOR,"���� ��������",0,0};
     __root const char ActiveClr[4]={0xFF,0x00,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_11={CFG_COLOR,"���� ����������",0,0};
     __root const char NotActiveClr[4]={0x00,0x00,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_12={CFG_COLOR,"��� ���.����",0,0};
     __root const char AddInfoClr[4]={0xFF,0x00,0x00,0x64};
     
     __root const CFG_HDR cfghdrrCl_13={CFG_COLOR,"����� ���.����",0,0};
     __root const char FontInfoClr[4]={0xFF,0xFF,0xFF,0x64};
     
     __root const CFG_HDR cfghdrrCl_14={CFG_COLOR,"���� �� ������������",0,0};
     __root const char UnAckedClr[4]={0x00,0x00,0x0,0x15};
     
     __root const CFG_HDR cfghdrrCl_15={CFG_COLOR,"���� ������������",0,0};
     __root const char SendClr[4]={0x00,0xFF,0x00,0x15};
*/
     
     
  __root const CFG_HDR cfghdrCl_0={CFG_LEVEL,"",0,0};
  
  __root const CFG_HDR cfghdrM_1={CFG_LEVEL,"������� ����",1,0};
  
      __root const CFG_HDR cfghdrrM_4={CFG_COLOR,"���� ����",0,0};
      __root const char MMBackgrClr[4]={0x00,0x00,0x00,0x45};//"\x00\x00\x00\x45"
      
      __root const CFG_HDR cfghdrrM_5={CFG_COLOR,"���� �������",0,0};
      __root const char MMBackgrBordClr[4]={0xFF,0xFF,0xFF,0x64};
           
      __root const CFG_HDR cfghdrrM_2={CFG_COLOR,"���� �������",0,0};
      __root const char MMCursorBGClr[4]={0x00,0x00,0x00,0x00};
      
      __root const CFG_HDR cfghdrrM_3={CFG_COLOR,"���� ������� �������",0,0};
      __root const char MMCursorClr[4]={0xFF,0xFF,0xFF,0x64}; 
      

      __root const CFG_HDR cfghdrM_6 = {CFG_RECT, "����", 0, 0};
#ifdef ELKA
      __root const RECT MMRect = {0,60,179,303};
#else
      __root const RECT MMRect = {0,26,125,164};
#endif
      
      __root const CFG_HDR cfghdrM_7 = {CFG_RECT, "�������", 0, 0};
      __root const RECT SRect = {30,YDISP+2,190,260};
      
      __root const CFG_HDR cfghdrM_8 = {CFG_RECT, "�-�������", 0, 0};
      __root const RECT XSRect = {30,YDISP+2,231,300};   
      
       __root const CFG_HDR cfghdrM_9={CFG_CHECKBOX,"���������� �����",0,2};
       __root const unsigned int ShowNumbMenu = 1;    
   
  __root const CFG_HDR cfghdrM_0={CFG_LEVEL,"",0,0};//
          
  //--------------------
  
  
    __root const CFG_HDR cfghdr_e1={CFG_LEVEL,"��� �����",1,0};//level       
          
          __root const CFG_HDR cfghdr23 = {CFG_STR_UTF8, "���� � 1 png ������", 0, 127};
          __root const char fontpath2[128] = "";
          
          __root const CFG_HDR cfghdr24 = {CFG_STR_UTF8, "���� � 2 png ������", 0, 127};
          __root const char fontpath3[128] = "";        
          
          __root const CFG_HDR cfghdre1_2 = {CFG_STR_UTF8, "png ����� ���������", 0, 127};
          __root const char fontpath4[128] = "";
          
          __root const CFG_HDR cfghdr_e1_3={CFG_CHECKBOX,"��� ����� ���������",0,2};
          __root const int PNG_HEADER = 1;
        
          __root const CFG_HDR cfghdr_e1_4={CFG_CHECKBOX,"��� ����� ������",0,2};
          __root const int PNG_SOFTKEY = 1;
    
    __root const CFG_HDR cfghdr_e0={CFG_LEVEL,"",0,0};// 
     
     
    __root const CFG_HDR cfghdr_y1={CFG_LEVEL,"��������� �����",1,0};//level     
     
          __root const CFG_HDR cfghdr_2_2={CFG_CHECKBOX,"������������ ����. �����",0,2};
          __root const int isSystemFont = 1;
          
          __root const CFG_HDR cfghdr2_8 = {CFG_UINT, "�����", 0, 15};  
          __root const unsigned int pngFont = FONT_SMALL;
          
          __root const CFG_HDR cfghdr2_9 = {CFG_UINT, "����� ���������", 0, 15};  
          __root const unsigned int HeadFont = FONT_SMALL;
    
    __root const CFG_HDR cfghdr_y0={CFG_LEVEL,"",0,0};//      


    //********* ��������� ������ ************//

    //View settings
    __root const CFG_HDR cfghdr23_4={CFG_LEVEL,"�������",1,0};    
    
      //Action on # button in contact list
        __root const CFG_HDR cfghdr78_4520={CFG_CBOX,"�������� �� # � ��",0,2};
        __root const int ActionResh = 0;
        __root const CFG_CBOX_ITEM cfgcbox_668_z[2]={"�������� ���������� ���","����.���.���"};
        
    
    //Press vol up or vol down button in CL
      __root const CFG_HDR cfghdr34_642={CFG_CBOX,"���. �������� �� �����.����",0,2};
      __root const int isVolDownButton = 1;
      __root const CFG_CBOX_ITEM cfgcbox434_56[2]={"���","��"};
      
      __root const CFG_HDR cfghdr34_6342={CFG_CBOX,"���.�������� �� �����.�����",0,2};
      __root const int isVolUpButton = 1;
      __root const CFG_CBOX_ITEM cfgcbox4134_56[2]={"���","��"};
      
    
    //{"��� ��������","����","�����","�����","���.�����������","�������� ����","������������","�����.�����/�����","������","������ ����","����","�������������","�� ��","���� �����"}
      __root const CFG_HDR cfghdr99_1={CFG_LEVEL,"������� �������",1,0};
        //Long press right in chat
          __root const CFG_HDR cfghdr4_67={CFG_CBOX,"������� ������� ������ � ����",0,3};
          __root const int LongRightAction = 0;
          __root const CFG_CBOX_ITEM cfgcbox4_56[3]={"������","��������� ���","�������� �����"};    
        
        //Long press right in chat
          __root const CFG_HDR cfghdr34_64={CFG_CBOX,"������� ������� ����� � ����",0,2};
          __root const int LongLeftAction = 0;
          __root const CFG_CBOX_ITEM cfgcbox444_56[2]={"������","���������� ���"};
          
        
        //Long press vol up button in chat
          __root const CFG_HDR cfghdr23_1={CFG_CBOX,"������� ������� + � ����",0,2};
          __root const int LongVolUpAction = 1;
          __root const CFG_CBOX_ITEM cfgcbox23_2[2]={"������","������� ����.���������"};
        
        //Long press vol down button in chat
          __root const CFG_HDR cfghdr23_3={CFG_CBOX,"������� ������� - � ����",0,2};
          __root const int LongVolDownAction = 1;
          __root const CFG_CBOX_ITEM cfgcbox23_4[2]={"������","�������� �����"};
    
    
    
    
          __root const CFG_HDR cfghdr99_3_0={CFG_CBOX,"������� 0 � ��",0,14};
          __root const int longAct_0 = 10;
          __root const CFG_CBOX_ITEM cfgcbox99_4_0[14]={LG_NOT_ACTION,//0
                                                        LG_SOUND,//1
                                                        LG_VIBRA,//2
                                                        LG_INVIS,//3
                                                        LG_VIBRA_REM,//4
                                                        LG_CLAER_ALL_CHAT,//5
                                                        LG_AUTOMSG,//6
                                                        LG_SHOW_XSTATE_TIME,//7
                                                        LG_CONFIG,//8
                                                        LG_CONFIG_MOD,//9
                                                        LG_PING,//10
                                                        LG_HIHE_LOCK,//11
                                                        LG_IDLE,//12
                                                        LG_USERMENU//13
                                                      };
    
          __root const CFG_HDR cfghdr99_3_1={CFG_CBOX,"������� 1 � ��",0,14};
          __root const int longAct_1 = 1;
          __root const CFG_CBOX_ITEM cfgcbox99_4_1[14]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_SHOW_XSTATE_TIME,
                                                      LG_CONFIG,
                                                      LG_CONFIG_MOD,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE,
                                                      LG_USERMENU
                                                    };
    
          __root const CFG_HDR cfghdr99_3_2={CFG_CBOX,"������� 2 � ��",0,14};
          __root const int longAct_2 = 2;
          __root const CFG_CBOX_ITEM cfgcbox99_4_2[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_3={CFG_CBOX,"������� 3 � ��",0,14};
          __root const int longAct_3 = 3;
          __root const CFG_CBOX_ITEM cfgcbox99_4_3[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_4={CFG_CBOX,"������� 4 � ��",0,14};
          __root const int longAct_4 = 4;
          __root const CFG_CBOX_ITEM cfgcbox99_4_4[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_5={CFG_CBOX,"������� 5 � ��",0,14};
          __root const int longAct_5 = 5;
          __root const CFG_CBOX_ITEM cfgcbox99_4_5[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_6={CFG_CBOX,"������� 6 � ��",0,14};
          __root const int longAct_6 = 6;
          __root const CFG_CBOX_ITEM cfgcbox99_4_6[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_7={CFG_CBOX,"������� 7 � ��",0,14};
          __root const int longAct_7 = 7;
          __root const CFG_CBOX_ITEM cfgcbox99_4_7[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_8={CFG_CBOX,"������� 8 � ��",0,14};
          __root const int longAct_8 = 8;
          __root const CFG_CBOX_ITEM cfgcbox99_4_8[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_9={CFG_CBOX,"������� 9 � ��",0,14};
          __root const int longAct_9 = 9;
          __root const CFG_CBOX_ITEM cfgcbox99_4_9[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_10={CFG_CBOX,"������� # � ��",0,14};
          __root const int longAct_10 = 11;
          __root const CFG_CBOX_ITEM cfgcbox99_4_10[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_11={CFG_CBOX,"������� * � ��",0,14};
          __root const int longAct_11 = 12;
          __root const CFG_CBOX_ITEM cfgcbox99_4_11[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
          
          __root const CFG_HDR cfghdr99_3_RB={CFG_CBOX,"������� ������ � ��",0,14};
          __root const int longAct_RB = 13;
          __root const CFG_CBOX_ITEM cfgcbox99_4_RB[14]={LG_NOT_ACTION,
                                                        LG_SOUND,
                                                        LG_VIBRA,
                                                        LG_INVIS,
                                                        LG_VIBRA_REM,
                                                        LG_CLAER_ALL_CHAT,
                                                        LG_AUTOMSG,
                                                        LG_SHOW_XSTATE_TIME,
                                                        LG_CONFIG,
                                                        LG_CONFIG_MOD,
                                                        LG_PING,
                                                        LG_HIHE_LOCK,
                                                        LG_IDLE,
                                                        LG_USERMENU
                                                      };
    
          __root const CFG_HDR cfghdr99_2={CFG_LEVEL,"",0,0}; //LONG PRESS ACTION
          
    __root const CFG_HDR cfghdr223_5={CFG_LEVEL,"",0,0}; //�������� ������

__root const CFG_HDR cfghdrI_0={CFG_LEVEL,"",0,0};//���������


//-----------------------------------------------------------------------------//
//---------------------------------- ��� ����� --------------------------------//
//-----------------------------------------------------------------------------//

	
__root const CFG_HDR cfghdrSnd1={CFG_LEVEL,"���.�����",1,0};

    __root const CFG_HDR cfghdr181={CFG_CBOX,"Extended sounds",0,2};
    __root const unsigned int extended_sounds = 0;
    __root const CFG_CBOX_ITEM cfgcbox18_3_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr1812={CFG_CBOX,"Add sound type file",0,3};
    __root const unsigned int ext_type_sounds = 0;
    __root const CFG_CBOX_ITEM cfgcbox182_3_2[3]={"Mp3","Wav","Amr"};
    
    __root const CFG_HDR cfghdr15={CFG_STR_UTF8,"add online msg path",0,63};
    __root const char sndGlobal2[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\Uins_on\\";
  
    __root const CFG_HDR cfghdr161={CFG_STR_UTF8,"add new msg path",0,63};
    __root const char sndMsg2[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\Uins_msg\\";
    
    __root const CFG_HDR cfghdr16r1={CFG_STR_UTF8,"����",0,63};
    __root const char pingsnd[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndPing.amr";    
    
    __root const CFG_HDR cfghdr16r2={CFG_STR_UTF8,"����� �����",0,63};
    __root const char sndLostConn[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\SndLostConn2.amr";  

__root const CFG_HDR cfghdrSnd0={CFG_LEVEL,"",0,0};


//-----------------------------------------------------------------------------//
//------------------------------- STATUS SETTING ------------------------------//
//-----------------------------------------------------------------------------//
  
__root const CFG_HDR cfghdr_45={CFG_LEVEL,"�������",1,0};

//Status icon on mainscreen position
#ifdef NEWSGOLD
  #ifdef kluchnik
      __root const CFG_HDR cfghdr467_1 = {CFG_CBOX,"���������� ������ �������",0,5};
      __root const int ShowStatusIcon = 1;
      __root const CFG_CBOX_ITEM cfgcbox9_1[5]={"�� ������� ������","� ��������","� ��������(����� ��)","�����","�����"};
  #else
      __root const CFG_HDR cfghdr467_1 = {CFG_CBOX,"���������� ������ �������",0,5};
      __root const int ShowStatusIcon = 0;
      __root const CFG_CBOX_ITEM cfgcbox9_1[5]={"�� ������� ������","� ��������","� ��������(����� ��)","�����","�����"};
  #endif
#endif
        
//Show x-status on iconbar or mainscreen
#ifdef kluchnik
    __root const CFG_HDR cfghdrz_68 = {CFG_CBOX,"�����.������� ������",0,4};
    __root const int show_xstatus = 3;
    __root const CFG_CBOX_ITEM cfgcbox_8z[4]={"�����","�� ��","� ��������","�����"};
#else
  #ifdef NEWSGOLD
      __root const CFG_HDR cfghdrz_68 = {CFG_CBOX,"�����.������� ������",0,4};
      __root const int show_xstatus = 1;
      __root const CFG_CBOX_ITEM cfgcbox_8z[4]={"�����","�� ��","� ��������","�����"};
  #else
      __root const CFG_HDR cfghdrz_68 = {CFG_CBOX,"�����.������� ������",0,2};
      __root const int show_xstatus = 1;
      __root const CFG_CBOX_ITEM cfgcbox_8z[2]={"�����","�� ��"};
  #endif
#endif    
    __root const CFG_HDR cfghdrz_1={CFG_COORDINATES,"������� ������� �� ��",0,0};
    __root const unsigned int IDLEICON_XX=2;
    __root const unsigned int IDLEICON_YX=70;
    
    __root const CFG_HDR cfghdr780_1={CFG_CBOX,"�����.��������� ��������",0,2};
    __root const int ind_set_xstatus = 0;
    __root const CFG_CBOX_ITEM cfgcbox8_z2[2]={"No","Yes"};
    
#ifdef NEWSGOLD
    __root const CFG_HDR cfghdr780_11={CFG_CBOX,"������ ��� ���������",0,2};
    __root const int blink_mess = 0;
    __root const CFG_CBOX_ITEM cfgcbox81_z2[2]={"No","Yes"};
    
//Number first pic status in folder img for show in iconbar
    __root const CFG_HDR cfghdr19_22={CFG_UINT,"1� ������� ������� � ��������",0,65535};
    __root const unsigned int NumStatusToIconBar = 8000;  

//Number first pic x-status in folder img for show in iconbar    
    __root const CFG_HDR cfghdr19_242={CFG_UINT,"1� ������� �������� � ��������",0,65535};
    __root const unsigned int NumXStatusToIconBar = 8100;  

    __root const CFG_HDR cfghdr_32_1={CFG_LEVEL,"���. ������ � �������",1,0};
 
       __root const CFG_HDR cfghdr32_2={CFG_UINT,"UIN",0,0xFFFFFFFF};
       __root const unsigned int ADDITION_UIN=0;
    
       __root const CFG_HDR cfghdr332_3={CFG_CHECKBOX,"���������� ��� ��������",0,2};
       __root const int ShowAddStatusToIconBar = 0;
       
       __root const CFG_HDR cfghdr32_3={CFG_CHECKBOX,"���������� ������",0,2};
       __root const int show_add_state = 0;
//       __root const CFG_CBOX_ITEM cfgcbox32_4[2]={"No","Yes"};       

       __root const CFG_HDR cfghdr32_5={CFG_CHECKBOX,"���������� �������",0,2};
       __root const int show_add_xstate = 0;
//       __root const CFG_CBOX_ITEM cfgcbox32_6[2]={"No","Yes"};       

   __root const CFG_HDR cfghdr_32_0={CFG_LEVEL,"",0,0};//ADDITION STATUS TO ICONBAR
#endif
   
__root const CFG_HDR cfghdr_4={CFG_LEVEL,"",0,0};//STATUS SETTINGS
  
  
//-----------------------------------------------------------------------------//
//-------------------------------- PNGFONT SETTING ----------------------------//
//-----------------------------------------------------------------------------//
  
  
__root const CFG_HDR cfghdr1_45={CFG_LEVEL,"��� �����",1,0};
   
//Nember png-font or system
   __root const CFG_HDR cfghdr65z={CFG_UINT,"����� ������ (1=png/1xx=system)",0,115};
   __root const unsigned int fonto=1;
    
//Path to png-font
#ifdef kluchnik
   __root const CFG_HDR cfghdr98={CFG_STR_UTF8,"���� � ������",0,63};
   __root const char fontpath[64]="4:\\elf\\naticq\\font\\";
#else
   __root const CFG_HDR cfghdr98={CFG_STR_UTF8,"���� � ������",0,63};
   __root const char fontpath[64]="";
#endif
   __root const CFG_HDR cfghdr65y={CFG_UINT,"���������� ����� �������",0,999999};
   __root const unsigned int spaco1=1;
      
   //__root const CFG_HDR cfghdr65u={CFG_UINT,"Main msg space",0,999999};
   //__root const unsigned int spaco2=0;
    
__root const CFG_HDR cfghdrss_45={CFG_LEVEL,"",0,0};//PNGFONT SETTING
  

//-----------------------------------------------------------------------------//
//                             SMILES COLOR SETTING                            //
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdr41={CFG_LEVEL,"C�����",1,0};


  __root const CFG_HDR cfghdrSm_5={CFG_CBOX,"����������� �������",0,2};
  __root const int UseStandartSmileSel = 0;
  __root const CFG_CBOX_ITEM cfgcboxSm_6[2]={"No","Yes"};  
       
  __root const CFG_HDR cfghdr40_1={CFG_COLOR,"���� ����",0,0};
  __root const char SmBGcolor[4]={0xFF,0xFF,0xFF,100};
  
  __root const CFG_HDR cfghdr40_2={CFG_COLOR,"���� �������",0,0};
  __root const char SmSelectColor[4]={0xFF,0x00,0x00,0x2A};
  
  __root const CFG_HDR cfghdr40_3={CFG_COLOR,"���� ������ �������",0,0};
  __root const char SmFontColor[4]={0x00,0x00,0x00,100};

__root const CFG_HDR cfghdr40={CFG_LEVEL,"",0,0};

//-----------------------------------------------------------------------------//
//                                 IDLE SETTING
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_m71={CFG_LEVEL,"������� �����",1,0};

    __root const CFG_HDR cfghdrm71_2 = {CFG_CBOX,"���������� �����",0,2};
    __root const int Is_UPTIME_On = 0;
    __root const CFG_CBOX_ITEM cfgcbox71_2_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr699 = {CFG_COORDINATES,"������� �������",0,0};
    __root const unsigned int onlineTime_x = 77;
    __root const unsigned int onlineTime_y = 98;

    __root const CFG_HDR cfghdr698 = {CFG_CBOX,"����� ��� �����������",0,2};
    __root const int Pause_Time = 1;
    __root const CFG_CBOX_ITEM cfgcbox85[2]={"No","Yes"};

    
    __root const CFG_HDR cfghdr123698 = {CFG_CBOX,"���������� ����",0,2};
    __root const int isPingIdle = 0;
    __root const CFG_CBOX_ITEM cfgcbox825[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd66 = {CFG_COORDINATES,"���������� ���������",0,0};
    __root const unsigned int Ping_X = 2;
    __root const unsigned int Ping_Y = 262;
    
    __root const CFG_HDR cfghdr1z9a_1={CFG_UINT,"���� ����� �����(���)",0,50};
    __root const unsigned int ping_time_config = 29;
    
    __root const CFG_HDR cfghdr298 = {CFG_CBOX,"���������� �������������",0,2};
    __root const int Is_Unread_On = 0;
    __root const CFG_CBOX_ITEM cfgcbox86[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd67 = {CFG_COORDINATES,"���������� �������������",0,0};
    __root const unsigned int unread_X = 99;
    __root const unsigned int unread_Y = 65;
    
    __root const CFG_HDR cfghdr71_3 = {CFG_CBOX,"���������� �������.",0,2};
    __root const int Is_Online_On = 1;
    __root const CFG_CBOX_ITEM cfgcbox71_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr712_3 = {CFG_CBOX,"������������ �������.",0,3};
    __root const int Is_Online_On_Align = 1;
    __root const CFG_CBOX_ITEM cfgcbox271_522[3]={"�����","�����","������"};
    
    __root const CFG_HDR cfghd71_1 = {CFG_COORDINATES,"�������.��������",0,0};
    __root const unsigned int online_x = 120;
    __root const unsigned int online_y = 240;    
    
    
    
    __root const CFG_HDR cfghdr71_32 = {CFG_CBOX,"���������� ���������",0,2};
    __root const int Is_SMARTCPU_On = 0;
    __root const CFG_CBOX_ITEM cfgcbox271_2[2]={"No","Yes"};
    
    //Coordinates for message on idle
   __root const CFG_HDR cfghd83 = {CFG_COORDINATES,"������� ���������",0,0};
   __root const unsigned int msg_d_x = 0;
   __root const unsigned int msg_d_y = 65;   
   
   __root const CFG_HDR cfghdrs19_1={CFG_UINT,"���������� ������",0,1000};
   __root const unsigned int main_msg_timer = 300;
   
   __root const CFG_HDR cfghdr60_2={CFG_COLOR,"���� ������",0,0};
   __root const char fontColor[4]={0xFF,0xFF,0xFF,100};
  
  __root const CFG_HDR cfghdr60_3={CFG_COLOR,"���� ����",0,0};
  __root const char bgrColor[4]={0x00,0x00,0x00,100};

//Max show symbol on idle
   __root const CFG_HDR cfghdr19a_1={CFG_UINT,"���������� ��������",0,50};
   __root const unsigned int main_lenght = 24;
      
   __root const CFG_HDR cfghdr9z9={CFG_CBOX,"������������ .. � ������ ",0,2};
   __root const int use_cutter = 1;
   __root const CFG_CBOX_ITEM cfgcbox18a_4[2]={"No","Yes"};

    

__root const CFG_HDR cfghdr_m90={CFG_LEVEL,"",0,0};



//-----------------------------------------------------------------------------//
//                                AUTO STATUS                                  //
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_m61={CFG_LEVEL,"����������",1,0};

    __root const CFG_HDR cfghdr668 = {CFG_CBOX,"��������",0,2};
    __root const int Auto_Status = 0;
    __root const CFG_CBOX_ITEM cfgcbox89[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr_m61_1 = {CFG_CBOX,"���. ����������",0,2};
    __root const int IsOnNAStatus = 1;
    __root const CFG_CBOX_ITEM cfgcbox_m61_2[2]={"No","Yes"};

    __root const CFG_HDR cfghdr61_8={CFG_UINT,"����� �� ����������(���)",0,150};
    __root const unsigned int NAStatusTime=20;    
    
    __root const CFG_HDR cfghdr6678 = {CFG_CBOX,"�������� ������������� ���",0,13};
    __root const int isAutoStatusOn = 12;
    __root const CFG_CBOX_ITEM cfgcboxa1_153[13]={"��������","� ����", "������", "����������", "�� ����������", "�����", "����� �������", "���������", "���������", "����", "����", "���","�� ������"};

    
    __root const CFG_HDR cfghdr667={CFG_UINT,"����� (���)",0,500};
    __root const unsigned int AUTO_ENGADE=20;
    
    __root const CFG_HDR cfghdr61_18={CFG_UINT,"���������� �������",0,150};
    __root const unsigned int automsgcount=3;
    
    __root const CFG_HDR cfghdr_13={CFG_CBOX,"GMT ����",0,33};
    __root const int time_zone = 16;
    __root const CFG_CBOX_ITEM cfgcbox_13[33]={"-12:00", "-11:00", "-10:00", "-09:00", "-08:00", "-07:00", "-06:00", "-05:00", "-04:00", "-03:30", "-03:00", "-02:00", "-01:00", "00:00", "+01:00", "+02:00", "+03:00", "+03:30", "+04:00", "+04:30", "+05:00", "+05:30", "+05:45", "+06:00", "+06:30", "+07:00", "+08:00", "+09:00", "+09:30", "+10:00", "+11:00", "+12:00", "+13:00"};
    
    __root const CFG_HDR cfghdrzz_8={CFG_STR_WIN1251,"��������� �����������",0,23};
    __root const char awayHelloMsg[24]="������";
    
    __root const CFG_HDR cfghdrzz_9={CFG_STR_WIN1251,"��������� ����������",0,255};
    __root const char awayMsg[256]="� ������ �� ���� ��������!";
    
    __root const CFG_HDR cfghdr66_2 = {CFG_CBOX,"���������� ����������� �����",0,2};
    __root const int Reset_timer = 0;
    __root const CFG_CBOX_ITEM cfgcbox90[2]={"�������� ���������","������� ����� �������"};
  
//Show away time on idle
    __root const CFG_HDR cfghdr663 = {CFG_CBOX,"���������� �� ��",0,2};
    __root const int showAwayIdle = 0;
    __root const CFG_CBOX_ITEM cfgcbox82[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd6_7 = {CFG_COORDINATES,"�������",0,0};
    __root const unsigned int auto_x = 0;
    __root const unsigned int auto_y = 252;
  
__root const CFG_HDR cfghdr_m60={CFG_LEVEL,"",0,0};



//-----------------------------------------------------------------------------//
//                             AUTO CHANGE STATUS                              //
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdra1={CFG_LEVEL,"��������� ��������",1,0};

    __root const CFG_HDR cfghdra1_3={CFG_CBOX,"1 c�����",0,13};
    __root const int status_1 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_13[13]={"��������","� ����", "������", "����������", "�� ����������", "�����", "����� �������", "���������", "���������", "����", "����", "���","�� ������"};

    __root const CFG_HDR cfghdra1_2={CFG_TIME,"����� �����",0,0};
    __root const TTime tm_status_1={15,0};   
    

    __root const CFG_HDR cfghdra1_1={CFG_CBOX,"2 c�����",0,13};
    __root const int status_2 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_14[13]={"��������","� ����", "������", "����������", "�� ����������", "�����", "����� �������", "���������", "���������", "����", "����", "���","�� ������"};

    __root const CFG_HDR cfghdra1_4={CFG_TIME,"����� �����",0,0};
    __root const TTime tm_status_2={15,0};


    __root const CFG_HDR cfghdra1_5={CFG_CBOX,"3 c�����",0,13};
    __root const int status_3 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_15[13]={"��������","� ����", "������", "����������", "�� ����������", "�����", "����� �������", "���������", "���������", "����", "����", "���","�� ������"};
    
    __root const CFG_HDR cfghdra1_6={CFG_TIME,"����� �����",0,0};
    __root const TTime tm_status_3={15,0};


    __root const CFG_HDR cfghdra1_7={CFG_CBOX,"4 c�����",0,13};
    __root const int status_4 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_16[13]={"��������","� ����", "������", "����������", "�� ����������", "�����", "����� �������", "���������", "���������", "����", "����", "���","�� ������"};
    
    __root const CFG_HDR cfghdra1_8={CFG_TIME,"����� �����",0,0};
    __root const TTime tm_status_4={15,0};


    __root const CFG_HDR cfghdra1_9={CFG_CBOX,"5 c�����",0,13};
    __root const int status_5 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_17[13]={"��������","� ����", "������", "����������", "�� ����������", "�����", "����� �������", "���������", "���������", "����", "����", "���","�� ������"};
    
    __root const CFG_HDR cfghdra1_14={CFG_TIME,"����� �����",0,0};
    __root const TTime tm_status_5={15,0};


__root const CFG_HDR cfghdra0={CFG_LEVEL,"",0,0};
#ifdef NEWSGOLD
//------------------------------------------------------------------------------//
//                                SCREEN SAVER                                  //
//------------------------------------------------------------------------------//

__root const CFG_HDR cfghdr_91={CFG_LEVEL,"��������",1,0};

    __root const CFG_HDR cfghdr9_1 = {CFG_CBOX,"�������� ��������",0,2};
    __root const int isScreenSaverEna = 0;
    __root const CFG_CBOX_ITEM cfgcbox9_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr9_3={CFG_UINT,"����� �� ������������(1���=12)",0,1000};
    __root const unsigned int screenSaverTime=120;

__root const CFG_HDR cfghdr_90={CFG_LEVEL,"",0,0};
#endif

//------------------------------------------------------------------------------//
//                                POPUP MESSAGE                                 //
//------------------------------------------------------------------------------//

__root const CFG_HDR cfghdr_51={CFG_LEVEL,"����������� �����.",1,0};

    __root const CFG_HDR cfghdr51_0 = {CFG_CBOX,"����������",0,4};
    __root const int PopupShow = 3;
    __root const CFG_CBOX_ITEM cfgcbox51_2[4]={"������","����.�����.","�����.����.","���"};
    
    __root const CFG_HDR cfghdr51_1={CFG_STR_UTF8,"������ ������.����.",0,8};
    __root const char PopupMsgString[9]="%s";
    
    __root const CFG_HDR cfghdr51_2={CFG_STR_UTF8,"������ �����.��������",0,8};
    __root const char PopupOnlineString[9]=">%s<";  
    
    __root const CFG_HDR cfghdr51_3={CFG_UINT,"����� ������.�����",0,1000};
    __root const unsigned int PopupFont=FONT_SMALL;
    
    __root const CFG_HDR cfghdr51_6={CFG_COLOR,"��� ������.�����.",0,0};
    __root const char PopupBGCl[4]={0x00,0x00,0x00,100};
    
    __root const CFG_HDR cfghdr51_4={CFG_COLOR,"���� �����.��������",0,0};
    __root const char PopupOnlineCl[4]={0x00,0xFF,0x00,100};
    
    __root const CFG_HDR cfghdr51_5={CFG_COLOR,"���� ������.�����.",0,0};
    __root const char PopupMsgCl[4]={0xFF,0xFF,0xFF,100};
    
    
    __root const CFG_HDR cfghdr51_9={CFG_CHECKBOX,"���������� �����������",0,2};
    __root const int isPopupOff = 1;
    
    __root const CFG_HDR cfghdr51_7={CFG_COLOR,"��� �������������",0,0};
    __root const char PopupOffBGCl[4]={0x00,0x00,0x00,100};
    
    __root const CFG_HDR cfghdr51_8={CFG_COLOR,"���� �������������",0,0};
    __root const char PopupOffCl[4]={0xFF,0x00,0x00,100};

__root const CFG_HDR cfghdr_50={CFG_LEVEL,"",0,0};


//------------------------------------------------------------------------------//
//                                EXTRA CONFIG                                  //
//------------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_m51={CFG_LEVEL,"������ �����",1,0};

//Vibra count in vibra remaider
    __root const CFG_HDR cfghdr65={CFG_UINT,"���-�� �������� � ���.�������.",0,10};
    __root const unsigned int Vibra_cc=2;

#ifdef NEWSGOLD
//CSM for player
    __root const CFG_HDR cfghdr66={CFG_STR_UTF8,"CSM ������",0,8};
    __root const char CsmAdr[9]="A087B454";  //el=A087B454 //es=A068E244
    
//Text before name track
    __root const CFG_HDR cfghdr67={CFG_STR_WIN1251,"����� ��� ������ �����",0,31};
    __root const char playing[32]="NatICQ play:";
    
//Id3 tags is any status    
    __root const CFG_HDR cfghdr7820={CFG_CHECKBOX,"���������� Id3 ��� ���.����.",0,2};
    __root const int Is_ID3_ANY_STATUS = 1;
//    __root const CFG_CBOX_ITEM cfgcbox28_z[2]={"No","Yes"};    
    
#endif  
//Show command sending    
    __root const CFG_HDR cfghdr780={CFG_CHECKBOX,"���������� ������ ������",0,2};
    __root const int show_send_info = 1;
//    __root const CFG_CBOX_ITEM cfgcbox8_z[2]={"No","Yes"};

    //isLogStatusChange
    __root const CFG_HDR cfghdr7_80={CFG_CHECKBOX,"�����. ����� ���.����.��������",0,2};
    __root const int isLogStatusChange = 1;
//    __root const CFG_CBOX_ITEM cfgcbox8df_z[2]={"No","Yes"};
    
//Show Loopback contact in CL
    __root const CFG_HDR cfghdr71_30={CFG_CHECKBOX,"���������� Loopback � ��",0,2};
    __root const int IsShowLoopback = 1;
//    __root const CFG_CBOX_ITEM cfgcbox_3z[2]={"No","Yes"};   
    
     
    __root const CFG_HDR cfghdr71_31={CFG_CHECKBOX,"��������� ������",0,2};
    __root const int IsOpenBrowser = 0;
//    __root const CFG_CBOX_ITEM cfgcbox_32[2]={"No","Yes"};   
    
    __root const CFG_HDR cfghdr71_33={CFG_CHECKBOX,"������ ������",0,2};
    __root const int IsMultiBuffer = 1;
//    __root const CFG_CBOX_ITEM cfgcbox_34[2]={"No","Yes"};   

    
__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};





//------------------------------------------------------------------------------//
//                          SETTING LOADING SCREEN                              //
//------------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_q1={CFG_LEVEL,"����� ��������",1,0};

   __root const CFG_HDR cfghdr_s2 = {CFG_RECT, "�����������", 0, 0};
#ifdef ELKA
   __root const RECT PBRECT = {1,280,238,287};//LOadScreenRECT
#else
   __root const RECT PBRECT = {4,150,126,157};//LOadScreenRECT
#endif
   
  __root const CFG_HDR cfghdr_s3={CFG_COLOR,"���� ������������",0,0};
  __root const char PBcolor[4]={0xFF,0x00,0x00,0x20};//ProgressBarColor
  
  __root const CFG_HDR cfghdr_s7={CFG_COLOR,"��� ������������",0,0};
  __root const char BGPBcolor[4]={0xFF,0xFF,0xFF,0x00};//BackGroundProgressBarColor

  __root const CFG_HDR cfghdr_s4={CFG_CBOX,"������ �����",0,2};
  __root const int IsInvertSoftKey = 1;
  __root const CFG_CBOX_ITEM cfgcbox_s5[2]={"No","Yes"};  

  __root const CFG_HDR cfghdr_s6={CFG_CBOX,"���������� ����",0,2};
  __root const int IsShowInfo = 1;
  __root const CFG_CBOX_ITEM cfgcbox_s7[2]={"No","Yes"};    

__root const CFG_HDR cfghdr_q0={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdrL_1={CFG_LEVEL,"���",1,0};

    __root const CFG_HDR cfghdrL_2={CFG_CHECKBOX,"����� ���",0,2};
    __root const int IsLogEna = 1;

    __root const CFG_HDR cfghdrL_5={CFG_UINT,"�����",0,10};
    __root const unsigned int _fontlog=FONT_SMALL;    
    
    __root const CFG_HDR cfghdrL_3={CFG_COLOR,"���",0,0};
    __root const char BGLog[4]={0x00,0x00,0x00,0x64};//BackGroundProgressBarColor
    
    __root const CFG_HDR cfghdrL_4={CFG_COLOR,"�����",0,0};
    __root const char FontClrLog[4]={0xFF,0xFF,0xFF,0x64};//BackGroundProgressBarColor    

__root const CFG_HDR cfghdL_0={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdrIn_1={CFG_LEVEL,"����������",1,0};

    __root const CFG_HDR cfghdrIn_2={CFG_COLOR,"���",0,0};
    __root const char BGInfo[4]={0x00,0x00,0x00,0x45};//BackGroundProgressBarColor      
    
    __root const CFG_HDR cfghdrIn_3={CFG_COLOR,"�����",0,0};
    __root const char FontClrInfo[4]={0x00,0xFF,0x00,0x64};//BackGroundProgressBarColor      

__root const CFG_HDR cfghdrIn_0={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr11_10={CFG_STR_WIN1251,"��� ���",0,31};
#ifdef kluchnik
__root const char I_str[32]="pashka";
#else
__root const char I_str[32]="I";
#endif

__root const CFG_HDR cfghdr11_9={CFG_STR_UTF8,"��������� ����� ����",0,255};
__root const char ignor_list[256]="";


//#endif

