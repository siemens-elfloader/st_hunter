#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"
//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

 __root const CFG_HDR cfghdr0={CFG_COLOR,"Bord color",0,0};
 __root const char CLR_BORD[4]={0xFF,0x00,0x00,0x64};
 
 __root const CFG_HDR cfghdr32={CFG_COLOR,"Background color",0,0};
 __root const char CLR_BACK[4]={0x00,0x00,0x00,0x64};

 __root const CFG_HDR cfghdr34={CFG_COLOR,"Current day",0,0};
 __root const char CLR_CURDAY[4]={0x00,0x00,0x00,0x64};

  __root const CFG_HDR cfghdr33={CFG_COORDINATES,"Start coordinates",0,0};//настраиваем координаты
#ifdef ELKA
  __root const unsigned int X_POS = 50;
  __root const unsigned int Y_POS = 50;
#else
  __root const unsigned int X_POS = 0;
  __root const unsigned int Y_POS = 10;
#endif  

  __root const CFG_HDR cfghdr35={CFG_INT,"Размер шрифта",0,99};//Вводим цифры, 0-99 это прмежуток от 0 до 99 тока можно писать
  __root const int FONT = FONT_SMALL;
  
 
 __root const CFG_HDR cfghdrCl_1={CFG_LEVEL,"Colors",1,0};   

      __root const CFG_HDR cfghdr31={CFG_COLOR,"1",0,0};
      __root const char CLR_1[4]={0xFF,0x00,0x00,0x64};
      
      __root const CFG_HDR cfghdr1={CFG_COLOR,"2",0,0};
      __root const char CLR_2[4]={0xFF,0x00,0x00,0x64};
      
      __root const CFG_HDR cfghdr2={CFG_COLOR,"3",0,0};
      __root const char CLR_3[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr3={CFG_COLOR,"4",0,0};
      __root const char CLR_4[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr4={CFG_COLOR,"5",0,0};
      __root const char CLR_5[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr5={CFG_COLOR,"6",0,0};
      __root const char CLR_6[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr6={CFG_COLOR,"7",0,0};
      __root const char CLR_7[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr7={CFG_COLOR,"8",0,0};
      __root const char CLR_8[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr8={CFG_COLOR,"9",0,0};
      __root const char CLR_9[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr9={CFG_COLOR,"10",0,0};
      __root const char CLR_10[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr10={CFG_COLOR,"11",0,0};
      __root const char CLR_11[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr11={CFG_COLOR,"12",0,0};
      __root const char CLR_12[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr12={CFG_COLOR,"13",0,0};
      __root const char CLR_13[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr13={CFG_COLOR,"14",0,0};
      __root const char CLR_14[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr14={CFG_COLOR,"15",0,0};
      __root const char CLR_15[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr15={CFG_COLOR,"16",0,0};
      __root const char CLR_16[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr16={CFG_COLOR,"17",0,0};
      __root const char CLR_17[4]={0xFF,0xFF,0x00,0x64};
      
      __root const CFG_HDR cfghdr17={CFG_COLOR,"18",0,0};
      __root const char CLR_18[4]={0xFF,0x00,0xFF,0x64};
      
      __root const CFG_HDR cfghdr18={CFG_COLOR,"19",0,0};
      __root const char CLR_19[4]={0x00,0xFF,0xFF,0x64};
      
      __root const CFG_HDR cfghdr19={CFG_COLOR,"20",0,0};
      __root const char CLR_20[4]={0x00,0xFF,0x00,0x64};
      
      __root const CFG_HDR cfghdr20={CFG_COLOR,"21",0,0};
      __root const char CLR_21[4]={0x00,0x00,0xFF,0x64};
      
      __root const CFG_HDR cfghdr21={CFG_COLOR,"22",0,0};
      __root const char CLR_22[4]={0x00,0x00,0xFF,0x64};
      
      __root const CFG_HDR cfghdr22={CFG_COLOR,"23",0,0};
      __root const char CLR_23[4]={0xFF,0x00,0x00,0x64};
      
      __root const CFG_HDR cfghdr23={CFG_COLOR,"24",0,0};
      __root const char CLR_24[4]={0xFF,0x00,0x00,0x64};
      
      __root const CFG_HDR cfghdr24={CFG_COLOR,"25",0,0};
      __root const char CLR_25[4]={0xFF,0x00,0x00,0x64};
      
      __root const CFG_HDR cfghdr25={CFG_COLOR,"26",0,0};
      __root const char CLR_26[4]={0xFF,0x00,0x00,0x64};
      
      __root const CFG_HDR cfghdr26={CFG_COLOR,"27",0,0};
      __root const char CLR_27[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr27={CFG_COLOR,"28",0,0};
      __root const char CLR_28[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr28={CFG_COLOR,"29",0,0};
      __root const char CLR_29[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr29={CFG_COLOR,"30",0,0};
      __root const char CLR_30[4]={0xFF,0xFF,0xFF,0x00};
      
      __root const CFG_HDR cfghdr30={CFG_COLOR,"31",0,0};
      __root const char CLR_31[4]={0xFF,0xFF,0xFF,0x00};

 __root const CFG_HDR cfghdrCl_2={CFG_LEVEL,"",0,0};   
 
 

