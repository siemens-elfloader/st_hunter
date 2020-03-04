
////////////////////////////////////////////////////////////////////////////////
//                    Структуры плагина ICQ, 1st ID=100500 =)
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	void (*connect)();
	void (*connect)(); 
	void (*disconnect)();
	void (*ping)();
	void (*RequestXText)(char * id);
	void (*set_normal_status)(int StatusId);
	void (*set_extended_status)(int StatusId);
	void (*set_private_status)(int StatusId);
	void (*SendMSG)(char *id, char *msg);
	void (*get_extended_status)(char * id);
	void (*get_short_info)(char * id);
	void (*addcontact2list)(char * id, char * nickname, char * group); 
   void (*ClosePlugin)();
   int ThisID;
}ExecStruct;

typedef struct
{
	char hostname[128];// Список серверов
	char id[64];// Id пользователя
	char paswd[64];// Пароль пользователя
	void (*connected)();// - тут обработка изменения статуса
	void (*addcontact)(char * id, char * nikename);// - добавление контакта
	void (*movecontact)(char * id, char * group);//	- перемещение в группу
	void (*removecontact)(char * id);// - удаление контакта
	void (*changestatus)(char * id,int status);// - изменение статуса контакта
	void (*recvmsg)(char * id,char * msg);// - сообщение от контакта
	void (*srvask)(char * id);// - сообщение дошло до сервера
	void (*clientask)(char * id);// - сообщение дошло до контакта
	void (*changesextendedstatus)(char * id,int status);// - изменение дополнительного статуса контакта
	void (*privatestatus)(char * id, int status);// - изменение приватного статуса
   ExecStruct *Struct;
}ParamStruct;

extern ExecStruct     EStruct;
extern ParamStruct    PStruct;
