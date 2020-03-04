
typedef struct
{
// dbg lib
   const int Debug;	// включен ли вывод лога
   const char  Path[128];	// Путь к файлу лога
// gprs lib
   const int trafic_size;		//
   const int trafic_gold;		//
   const int trafic_silver;	//
   const int trafic_blur;		//
   const char text_valute[64];	//  наименование валюты
   const int LIMIT_TRAF_ACTIVE;	// включено ли ограничение трафика
   const int LIMIT_TRAF_KB;	//  ограничение трафика в кб
   const int LIMIT_TRAF_SEC;  // ограничение трафика в секуднах
   const int session_time_enable;	// считать ли время сессии
// mes lib
   const int TypeMes;	// Тип сообщения
}


