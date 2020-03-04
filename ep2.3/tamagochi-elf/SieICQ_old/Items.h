#ifndef _ITEMS_H_
  #define _ITEMS_H_


enum TypeItem {GROUP, // группа
               BUDDY  // контакт
              };

enum TypeHist {
                IN_MESS, // входящие
                OUT_MESS, // исходящие
                STATUS,   // сообщение Status
                XSTATUS,   // сообщение XStatus
                AUTREQ,// Authorization request message
	        AUTDEN,// Authorization denied message
	        AUTGIV,// Authorization given message
	        YOUADD// "You-were-added" message
              };

typedef struct
{
  void *next;
  TDate date;
  TTime time;
  int type;
  char *text;
}HIST;

typedef struct
{
  void *next;
  void *prev;
  unsigned int ID;            // ID контакта , для группы = 0
  unsigned int GroupID;       // ID группы , для контакта - в какой группе
  unsigned int UIN;
  unsigned short Type;
  char Nick[64];
  unsigned short Status;
  char *StatusText;
  unsigned short XStatus;
  char* XStatusText;
  unsigned short isunread;  // есть непрочитанные
  unsigned short istyping; // набирает сообщение  //0x0000 - typing finished
                                                  //0x0001 - text typed
                                                  //0x0002 - typing begun
  unsigned short iscollapsed;
  unsigned short visible;
  unsigned short enable_typing;
  unsigned short client_id;
  
  HIST *history;
  
}ITEM;


extern ITEM *Itemtop;

ITEM *AddItem(unsigned int ID,  unsigned int GroupID, unsigned int UIN, unsigned short Type,  char* Nick);
void DeleteItem(int curitem);

ITEM *GetItem(int curitem);
ITEM *GetItemByID(int ID); // возвращает контакт по ID
ITEM *GetGroupByGroupID(int GroupID); // группу по GroupID
ITEM *GetItemByUIN(int UIN); // контакт по его UIN(число)
ITEM *GetItemByUINstr(char *UIN); // контакт по его Uin строке
ITEM *GetItemByName(char *name); // по имени



void GroupCollapsed(int GroupID); // свернуть развернуть группу
void GroupVisible(int visible); // скрыть группы
void OfflineVisible(int visible); // скрыть контакты не в сети
int CountVisibleItem(); // количество видимых итемов


int GetContactInGroup(int GroupID);
int TotalItems();
int TotalContact();
int TotalGroup();

//// history
void AddHistory(ITEM* item, TDate date, TTime time, int type, char * text);
void DelHistory(ITEM* item);





//// file CL
void LoadCL(char *pathfile);
void SaveCL(char *pathfile);



void FreeItemsList();

#endif
