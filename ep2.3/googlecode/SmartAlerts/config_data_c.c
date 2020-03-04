#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"

__root const CFG_HDR cfghdr_m5={CFG_LEVEL,"满电提醒",1,0};

__root const CFG_HDR cfghdr20={CFG_CHECKBOX,"满电提醒",0,0};
__root const int ch_bat=0;

__root const CFG_HDR cfghdr21={CFG_UINT,"满电音效",0,200};
__root const unsigned int melody2=51;

__root const CFG_HDR cfghdr_m4={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_m7={CFG_LEVEL,"运行文件",1,0};

__root const CFG_HDR cfghdr200={CFG_CHECKBOX,"运行文件",0,0};
__root const int runfile=0;

__root const CFG_HDR cfghdr201={CFG_UINT,"运行小时1",0,23};
__root const unsigned int rhour1=0;

__root const CFG_HDR cfghdr202={CFG_UINT,"运行分钟1",0,59};
__root const unsigned int rminute1=0;

__root const CFG_HDR cfghdr203={CFG_STR_UTF8,"文件目录1",0,63};
__root const char name1[64]="";

__root const CFG_HDR cfghdr204={CFG_UINT,"运行小时2",0,23};
__root const unsigned int rhour2=0;

__root const CFG_HDR cfghdr205={CFG_UINT,"运行分钟2",0,59};
__root const unsigned int rminute2=0;

__root const CFG_HDR cfghdr206={CFG_STR_UTF8,"文件目录2",0,63};
__root const char name2[64]="";

__root const CFG_HDR cfghdr207={CFG_UINT,"运行小时3",0,23};
__root const unsigned int rhour3=0;

__root const CFG_HDR cfghdr208={CFG_UINT,"运行分钟3",0,59};
__root const unsigned int rminute3=0;

__root const CFG_HDR cfghdr209={CFG_STR_UTF8,"文件目录3",0,63};
__root const char name3[64]="";


__root const CFG_HDR cfghdr210={CFG_UINT,"运行小时4",0,23};
__root const unsigned int rhour4=0;

__root const CFG_HDR cfghdr211={CFG_UINT,"运行分钟4",0,59};
__root const unsigned int rminute4=0;

__root const CFG_HDR cfghdr212={CFG_STR_UTF8,"文件目录4",0,63};
__root const char name4[64]="";

__root const CFG_HDR cfghdr213={CFG_UINT,"运行小时5",0,23};
__root const unsigned int rhour5=0;

__root const CFG_HDR cfghdr214={CFG_UINT,"运行分钟5",0,59};
__root const unsigned int rminute5=0;

__root const CFG_HDR cfghdr215={CFG_STR_UTF8,"文件目录5",0,63};
__root const char name5[64]="";

__root const CFG_HDR cfghdr_m6={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_m9={CFG_LEVEL,"定时提醒",1,0};

__root const CFG_HDR cfghdr10={CFG_UINT,"起始时间",0,23};
__root const unsigned int min=0;

__root const CFG_HDR cfghdr11={CFG_UINT,"结束时间",0,23};
__root const unsigned int max=24;

__root const CFG_HDR cfghdr12={CFG_UINT,"提示音效",0,200};
__root const unsigned int melody=60;

__root const CFG_HDR cfghdr14={CFG_UINT,"提示分钟",0,59};
__root const unsigned int minute=0;

//__root const CFG_HDR cfghdr16={CFG_UINT,"提醒情景禁用",0,8};
//__root const unsigned int filter=0;

__root const CFG_HDR cfghdr_m8={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"情景切换",1,0};

__root const CFG_HDR cfghdr100={CFG_UINT,"切换分钟",0,59};
__root const unsigned int minute3=0;

__root const CFG_HDR cfghdr101={CFG_STR_UTF8,"星期一",0,24};
__root const char day1[25]="000000000000000000000000";

__root const CFG_HDR cfghdr102={CFG_STR_UTF8,"星期二",0,24};
__root const char day2[25]="000000000000000000000000";

__root const CFG_HDR cfghdr103={CFG_STR_UTF8,"星期三",0,24};
__root const char day3[25]="000000000000000000000000";

__root const CFG_HDR cfghdr104={CFG_STR_UTF8,"星期四",0,24};
__root const char day4[25]="000000000000000000000000";

__root const CFG_HDR cfghdr105={CFG_STR_UTF8,"星期五",0,24};
__root const char day5[25]="000000000000000000000000";

__root const CFG_HDR cfghdr106={CFG_STR_UTF8,"星期六",24,24};
__root const char day6[25]="000000000000000000000000";

__root const CFG_HDR cfghdr107={CFG_STR_UTF8,"星期日",0,24};
__root const char day7[25]="000000000000000000000000";

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};





