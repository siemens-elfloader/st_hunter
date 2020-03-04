







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

















































 




























 






__root const CFG_HDR cfghdr23_4={9,"Настройка кнопок",1,0};


  __root const CFG_HDR cfghdr4_67={5,"Длинное нажатие вправо в чате",0,3};
  __root const int LongRightAction = 2;
  __root const CFG_CBOX_ITEM cfgcbox4_56[3]={"Ничего","Слудующий чат","Вставить смайл"};    


  __root const CFG_HDR cfghdr34_64={5,"Длинное нажатие влево в чате",0,2};
  __root const int LongLeftAction = 0;
  __root const CFG_CBOX_ITEM cfgcbox444_56[2]={"Ничего","Предыдущий чат"};
  
  
    __root const CFG_HDR cfghdr78_4520={5,"Действие на # в КЛ",0,2};
    __root const int ActionResh = 0;
    __root const CFG_CBOX_ITEM cfgcbox_668_z[2]={"Очистить выделенный чат","След.акт.чат"};
    

  __root const CFG_HDR cfghdr34_642={5,"Действие на громк.вниз",0,2};
  __root const int isVolDownButton = 1;
  __root const CFG_CBOX_ITEM cfgcbox434_56[2]={"Нет","Да"};
  
  __root const CFG_HDR cfghdr34_6342={5,"Действие на громк.вверх",0,2};
  __root const int isVolUpButton = 1;
  __root const CFG_CBOX_ITEM cfgcbox4134_56[2]={"Нет","Да"};
  
__root const CFG_HDR cfghdr223_5={9,"",0,0}; 
	
	



  
__root const CFG_HDR cfghdr_45={9,"Настройка статусов",1,0};


    __root const CFG_HDR cfghdr467_1 = {5,"Показывать иконку статуса",0,5};
    __root const int ShowStatusIcon = 1;
    __root const CFG_CBOX_ITEM cfgcbox9_1[5]={"На главном экране","В иконбаре","В иконбаре(кроме гэ)","Везде","Нигде"};
    


 
        

    __root const CFG_HDR cfghdrz_68 = {5,"Показ.хстатус иконку",0,4};
    __root const int show_xstatus = 2;
    __root const CFG_CBOX_ITEM cfgcbox_8z[4]={"Нигде","На гэ","В иконбаре","Везде"};
    
    __root const CFG_HDR cfghdrz_1={7,"Позиция хстатус на гэ",0,0};
    __root const unsigned int IDLEICON_XX=32;
    __root const unsigned int IDLEICON_YX=291;
    


    __root const CFG_HDR cfghdr19_22={1,"1я картика статуса в иконбаре",0,65535};
    __root const unsigned int NumStatusToIconBar = 8000;  


    __root const CFG_HDR cfghdr19_242={1,"2я картика статуса в иконбаре",0,65535};
    __root const unsigned int NumXStatusToIconBar = 8100;  
    
__root const CFG_HDR cfghdr_4={9,"",0,0};
  
  



  
  
__root const CFG_HDR cfghdr1_45={9,"Настройка пнгшрифта",1,0};
   

   __root const CFG_HDR cfghdr65z={1,"Номер шрифта (1=png/1xx=system)",0,115};
   __root const unsigned int fonto=1;
    

   __root const CFG_HDR cfghdr98={2,"Путь к шрифту",0,63};
   __root const char fontpath[64]="";


   __root const CFG_HDR cfghd83 = {7,"Позиция на гэ",0,0};
   __root const unsigned int msg_d_x = 0;
   __root const unsigned int msg_d_y = 65;   
   
  __root const CFG_HDR cfghdr60_2={8,"Цвет шрифта",0,0};
  __root const char fontColor[4]={0xFF,0xFF,0xFF,100};
  
  __root const CFG_HDR cfghdr60_3={8,"Цвет фона",0,0};
  __root const char bgrColor[4]={0x00,0x00,0x00,100};


   __root const CFG_HDR cfghdr19a_1={1,"Количество символов",0,50};
   __root const unsigned int main_lenght = 24;
      
   __root const CFG_HDR cfghdr9z9={5,"Использовать .. в тектсе ",0,2};
   __root const int use_cutter = 1;
   __root const CFG_CBOX_ITEM cfgcbox18a_4[2]={"No","Yes"};

   __root const CFG_HDR cfghdr65y={1,"Расстояние между буквами",0,999999};
   __root const unsigned int spaco1=0;
      
   
   
    
__root const CFG_HDR cfghdrss_45={9,"",0,0};
  

 




__root const CFG_HDR cfghdr_m71={9,"Время онлайн",1,0};

    __root const CFG_HDR cfghdr699 = {7,"Позиция времени",0,0};
    __root const unsigned int onlineTime_x = 77;
    __root const unsigned int onlineTime_y = 98;

    __root const CFG_HDR cfghdr698 = {5,"Пауза при дисконнекте",0,2};
    __root const int Pause_Time = 1;
    __root const CFG_CBOX_ITEM cfgcbox85[2]={"No","Yes"};

__root const CFG_HDR cfghdr_m70={9,"",0,0};







__root const CFG_HDR cfghdr_m91={9,"Автопинг/непрочитанные",1,0};

    __root const CFG_HDR cfghd66 = {7,"Координаты автопинга",0,0};
    __root const unsigned int Ping_X = 2;
    __root const unsigned int Ping_Y = 262;
    
    __root const CFG_HDR cfghdr1z9a_1={1,"Звук пинга после(times)",0,50};
    __root const unsigned int ping_time_config = 29;
    
    __root const CFG_HDR cfghdr298 = {5,"Показывать непрочитанные",0,2};
    __root const int Is_Unread_On = 1;
    __root const CFG_CBOX_ITEM cfgcbox86[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd67 = {7,"Координаты непрочитанных",0,0};
    __root const unsigned int unread_X = 99;
    __root const unsigned int unread_Y = 65;

__root const CFG_HDR cfghdr_m90={9,"",0,0};








__root const CFG_HDR cfghdr_m61={9,"Автостатус",1,0};

    __root const CFG_HDR cfghdr668 = {5,"Включить",0,2};
    __root const int Auto_Status = 1;
    __root const CFG_CBOX_ITEM cfgcbox89[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr667={1,"Время (1min=12)",120,500};
    __root const unsigned int AUTO_ENGADE=120;
    
    __root const CFG_HDR cfghdr_13={5,"GMT зона",0,33};
    __root const int time_zone = 16;
    __root const CFG_CBOX_ITEM cfgcbox_13[33]={"-12:00", "-11:00", "-10:00", "-09:00", "-08:00", "-07:00", "-06:00", "-05:00", "-04:00", "-03:30", "-03:00", "-02:00", "-01:00", "00:00", "+01:00", "+02:00", "+03:00", "+03:30", "+04:00", "+04:30", "+05:00", "+05:30", "+05:45", "+06:00", "+06:30", "+07:00", "+08:00", "+09:00", "+09:30", "+10:00", "+11:00", "+12:00", "+13:00"};
    
    __root const CFG_HDR cfghdrzz_8={4,"Сообщение приветствия",0,23};
    __root const char awayHelloMsg[24]="Привет";
    
    __root const CFG_HDR cfghdrzz_9={4,"Сообщение отсутствия",0,63};
    __root const char awayMsg[64]="я сейчас не могу ответить!";
    
    __root const CFG_HDR cfghdr66_2 = {5,"Отключение автостатуса после",0,2};
    __root const int Reset_timer = 0;
    __root const CFG_CBOX_ITEM cfgcbox90[2]={"Отправки сообщения","Нажатия любой клавиши"};
  

    __root const CFG_HDR cfghdr663 = {5,"Показывать на гэ",0,2};
    __root const int showAwayIdle = 1;
    __root const CFG_CBOX_ITEM cfgcbox82[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd6_7 = {7,"Позиция",0,0};
    __root const unsigned int auto_x = 0;
    __root const unsigned int auto_y = 252;
  
__root const CFG_HDR cfghdr_m60={9,"",0,0};







__root const CFG_HDR cfghdr_m51={9,"Экстра опции",1,0};


    __root const CFG_HDR cfghdr467 = {5,"Показывать в заголовке КЛ",0,4};
    __root const int TIME_OR_MSG = 3;
    __root const CFG_CBOX_ITEM cfgcbox91[4]={"Время","Сообщения","Контакты","Все"};
  

    __root const CFG_HDR cfghdr65={1,"Кол-во вибраций в реж.напомин.",0,10};
    __root const unsigned int Vibra_cc=2;
    

    __root const CFG_HDR cfghdr66={2,"CSM плеера",0,8};
    __root const char CsmAdr[9]="A068E244";  
    

    __root const CFG_HDR cfghdr67={4,"Текст для вывода песни",0,12};
    __root const char playing[13]="MediaPlayer:";
  

    __root const CFG_HDR cfghdr780={5,"Отображать запрос команд",0,2};
    __root const int show_send_info = 1;
    __root const CFG_CBOX_ITEM cfgcbox8_z[2]={"Off","On"};


    __root const CFG_HDR cfghdr7_80={5,"Показывать время откл.контакта",0,2};
    __root const int isLogOffline = 1;
    __root const CFG_CBOX_ITEM cfgcbox8df_z[2]={"No","Yes"};
    

    __root const CFG_HDR cfghdr7_81={5,"Показывать время подкл.контакта",0,2};
    __root const int isLogOnline = 1;
    __root const CFG_CBOX_ITEM cfgcbox9df_z[2]={"No","Yes"};
   

    __root const CFG_HDR cfghdr71_30={5,"Показывать Loopback в КЛ",0,2};
    __root const int IsShowLoopback = 1;
    __root const CFG_CBOX_ITEM cfgcbox_3z[2]={"Off","On"};
    






 
    
__root const CFG_HDR cfghdr_m50={9,"",0,0};


__root const CFG_HDR cfghdr11_9={2,"Small ignor-list",0,255};
__root const char ignor_list[256]="";

