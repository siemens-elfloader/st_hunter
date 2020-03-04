#ifndef __NATICQ_H__
#define __NATICQ_H__

/*
  NatICQ protocol definitions
  */

#define NAT_DATAMAX 16384

/*
  Packet types
  CS - Client -> Server
  SC - Server -> Client
  *  - Any direction
  */
typedef enum
{
    NAT_PING = 0,            // |  CS  | UIN                                         | Ping
    NAT_REQLOGIN,            // |  CS  | UIN != 0, data -> password or empty         | Клиент запрашивает логин
    NAT_SENDMSG,             // |  CS  | UIN, data -> message                        | Клиент отправил сообщение
    NAT_RECVMSG,             // |  SC  | UIN, data -> message                        | Клиент принял сообщение
    NAT_LOGIN,               // |  SC  | No data                                     | Удачно залогинились
    NAT_RSRVD0,              // |  ?   | /* unused in current versions */            |
    NAT_ERROR,               // |  SC  | Data -> description                         | Ошибка
    NAT_CLENTRY,             // |  SC  | If UIN, then data = name, else it's CL end  | Запись в контакт-листе
    NAT_RSRVD1,              // |  ?   | /* unused in current versions */            |
    NAT_STATUSCHANGE,        // |  SC  | UIN, data[0-1] = status, data[2+] = xstatus | Изменение статуса контакта
    NAT_AUTHREQ,             // |  CS  | UIN != 0, data = text                       | Запрос авторизации
    NAT_REQINFOSHORT,        // |  CS  | UIN != 0                                    | Запрос краткой информации
    NAT_ADDCONTACT,          // |  CS  | /* unused in current versions */            | Добавить контакт
    NAT_SSLRESP,             // |  SC  | data = message                              | Сообщение сервера (IG11)
    NAT_AUTHGRANT,           // |  CS  | UIN != 0, data = text                       | Авторизация одобрена
    NAT_MY_STATUS_CH,        // |  CS  | UIN = 0, data[0] = status                   | Изменение моего статуса
    NAT_SRV_ACK,             // |  SC  | UIN != 0                                    | Сервер (AOL) получил сообщение
    NAT_CLIENT_ACK,          // |  SC  | UIN != 0                                    | Адресат получил сообщение
    NAT_ECHO,                // |  CS  | UIN = 0, data = time                        | Пустой запрос
    NAT_ECHORET,             // |  SC  | UIN = 0, data = time                        | Пустой ответ
    NAT_GROUPID,             // |  SC  | UIN = GID, data = name                      | Запись о группе
    NAT_GROUPFOLLOW,         // |  SC  | UIN = GID                                   | Последующие контакты входят в группу
    NAT_MY_XSTATUS_CH,       // |  CS  | UIN = 0, data[0] = xstatus                  | Изменение моего X-статуса
    NAT_MSGACK,              // |  CS  | ?UIN = Message id?                          | Подтверждение доставки
    NAT_XTEXT_REQ,           // |  CS  | UIN != 0                                    | Клиент запрашивает X-статус
    NAT_XTEXT_ACK,           // |  SC  | UIN = 0,data[0] = head end,data[1+] = str   | Ответ на запрос Х-статуса
    NAT_XTEXT_SET,           // |  CS  | UIN = 0, data = xstat1 + \0 + xstat2        | Клиент устанавливает текст X-статуса
    NAT_ADDCONTACT_WITH_GRP, // |  CS  | UIN != 0, data[0-3] = GID, data[4+] = name  | Добавление контакта с группой
    NAT_ADDGROUP,            // |  CS  | /* unused in current versions */            | Добавление группы
    NAT_ADDIGNORE,           // |  CS  | UIN != 0                                    | Засунуть в игнор
    NAT_SETPRIVACY,          // |  CS  | UIN = 0, data[0] = priv status              | Установить приватный статус
    NAT_LASTPRIVACY,         // |  SC  | data[0] = priv status                       | Получить приватный статус
    NAT_SETCLIENT_ID,        // |  CS  | UIN != 0,data = "Sie_XXXX", XXXX = revision | Установить идентификатор клиента
    NAT_REMOVECONTACT,       // |  CS  | UIN != 0                                    | Удалить контакт из списка
    NAT_CONTACTREMOVED,      // |  SC  | UIN != 0                                    | Контакт удален
    NAT_CALCMD5,             // |  *   | if SC, data = salt; if CS, data = md5 hash  | Вычисление авторизационного хэша
    NAT_EXECUTE,             // |  SC  | data = ARM code that must form answer       | Исполняемый код
    NAT_CLIENTID             // |  SC  | UIN != 0, data[0] = client id               | Идентификатор ICQ-клиента контакта
} NAT_MSGS;

/*
  Private statuses
*/
typedef enum
{
  PL_ALL = 0,      // Виден всем
  PL_NOBODY,       // Никому
  PL_VISLIST,      // Только видящим
  PL_ALL_EX_INVIS, // Всем, кроме невидящих
  PL_CONTACTLIST   // Только списку контактов
} NAT_PL;


typedef enum
{
  IS_OFFLINE = 0,
  IS_INVISIBLE,
  IS_AWAY,
  IS_NA,
  IS_OCCUPIED,
  IS_DND,
  IS_DEPRESSION,
  IS_EVIL,
  IS_HOME,
  IS_LUNCH,
  IS_WORK,
  IS_ONLINE,
  IS_FFC,
  IS_MSG,
  IS_UNKNOWN
} NAT_STATUS;


typedef struct
{
  unsigned int uin;         //0
  unsigned short type;      //4
  unsigned short data_len;  //6
} PKT;

typedef struct
{
  PKT pkt;
  char data[NAT_DATAMAX];    //8
} TPKT;

char *win2utf(const char *win, char *utf);
char *utf2win(const char *utf, char *win);
char *ucs2win(const char *ucs, char *win, int len);

/*
  NatICQ auth process:

  Client           <->  Server
  // 0 Setup Client Id
  NAT_SETCLIENT_ID

  // 1 Login (old)
  NAT_REQLOGIN (with pass)         
                        NAT_LOGIN (or NAT_ERROR)

  // 1 Login (new)
  NAT_REQLOGIN (pass empty)
                        NAT_CALCMD5 (md5 salt)
  NAT_CALCMD5 (hash)
                        NAT_LOGIN (or NAT_ERROR)

  // 2 Ok, we are logged in, setup status
                        NAT_LASTPRIVACY
  NAT_MY_STATUS_CH
  NAT_MY_XSTATUS_CH

  // 3 Loading contact list
                        NAT_GROUPID (for each group)
                        ...for each contact...
                        NAT_GROUPFOLLOW (UIN = group id)
                        NAT_CLENTRY (UIN != 0)
                        ...end...
                        NAT_CLENTRY (UIN = 0)

  // 4 Main loop
                        ...for each contact...
                        NAT_STATUSCHANGE (UIN != 0)
                        ...end...
*/
#endif //__NATICQ_H__

