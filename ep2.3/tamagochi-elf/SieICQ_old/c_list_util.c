#include "include.h"
#include "items.h"
#include "c_list_util.h"
#include "iconpack.h"
#include "icq.h"
#include "capabilities.h"
#include "chat.h"

// Переменные - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 
  c_list_cursor_pos, // Позиция курсора
  c_list_cursor_item, // текущий Item
  c_list_max_contacts,   // Общее число контактов
  c_list_max_show_n, // Максимальное число контактов на экране
  header_height=0,
  items_y_disp=2;


// Структуры - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
extern 
  ITEM *Itemtop;
  CONTACT_LIST_DESC main_c_list={
    2,
    0,
    0,
    0,
    11,
    2
  };
  HEADER_ITEM head_c_list;
  SOFT_BUTTON_STRUCT c_list_soft={3, 3, "Выбор", "Меню", 1, 0};


// Функции - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Работа с курсором //
  
void CList_MoveCursorHome()
{
  if(!c_list_max_show_n)return;
  c_list_cursor_pos = 0;
  REDRAW();
}

void CList_MoveCursorEnd()
{
  if(!c_list_max_show_n)return;
  c_list_cursor_pos = c_list_max_contacts;
  REDRAW();
}

int MoveCursor_C_List(int mode_key, int type_key)
{
  switch(mode_key)
  {
  case KEY_DOWN:
    {
      switch(type_key)
      {
      case UP_BUTTON:
        
        if(!c_list_cursor_pos) c_list_cursor_pos = c_list_max_contacts-1;
        else c_list_cursor_pos--;
        
      break;
      
      case DOWN_BUTTON:
        
        if(c_list_cursor_pos==c_list_max_contacts-1) c_list_cursor_pos = 0;
        else c_list_cursor_pos++;

      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:
        // если группа , то 
        {
            ITEM *bmk = GetItem(c_list_cursor_item);
            if (bmk->ID==0)
            {
              GroupCollapsed(bmk->GroupID);
            }
            else
              CreateEditChat();
        }
      break;
      
      case '1': 
      CList_MoveCursorHome();
      break;
    
      case '7':
      CList_MoveCursorEnd();
      break;
      
      case RIGHT_SOFT: TYPE_DRAW = Draw_MainMenu;
      break;
      }
    }
    break;
  }
  
//  DirectRedrawGUI();
  return 0;
}
  // Работа с интерфейсом //

// Задаём параметры шрифта
void CLIST_SetItemsFont(CONTACT_LIST_DESC * data, int sets_font, int sets_font_type)
{
  data->items_font = sets_font;
  data->items_font_type = sets_font_type;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void DrawCListFon()
{
//    DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
    DrawImg(0,0,IconPack[IMG_BG]);
}


void GetShowsNumContacts(CONTACT_LIST_DESC * data, int head_h, int soft_h)
{
  data->y_disp = 2;
  c_list_max_contacts = data->max_nums = TotalContact();
  c_list_max_show_n = (ScrH - head_h - soft_h)/ (2*( data->y_disp) + GetImgHeight(IconPack[IMG_ONLINE]));
}



#define SHOW_WITH_GROUP 1
#define SHOW_WITHNOT_GROUP 0
int ShowType=0;

/*
static void DrawContactList(CONTACT_LIST_DESC *data)
{
  int 
    ALT_disp = 0,
    ALT_clist_count,
    
    ALT_cursor_pos,
    
    ALT_max_count=0,
    NEW_Y;
  
  int Y_DISP = data->y_disp;
  
  ALT_max_count = c_list_max_contacts;
  
  if( c_list_max_contacts > c_list_max_show_n ) 
    ALT_max_count = c_list_max_show_n;
  else
    ALT_max_count = c_list_max_contacts;
    
  if(c_list_cursor_pos > c_list_max_show_n )
  {
    ALT_cursor_pos = c_list_max_show_n;
    ALT_disp = c_list_cursor_pos  - c_list_max_show_n;
  }
  else  ALT_cursor_pos = c_list_cursor_pos;
  
  
  

  char header_text[128]="";
  sprintf(header_text,"%d/%d", c_list_cursor_pos, c_list_max_contacts);
  InitHeaderText(&head_c_list, header_text);
  DrawHeaderText(&head_c_list);
  

  WSHDR *item_data = AllocWS(128);
  
  int VisibleItem=CountVisibleItem();
  for(ALT_clist_count = 0; ALT_clist_count <= ALT_max_count; ALT_clist_count++)
  {

    NEW_Y =  header_height + (2*Y_DISP + Font_H) * (ALT_clist_count);

    if(ALT_clist_count==ALT_cursor_pos) 
      DrawRoundedFrame(0, NEW_Y+Y_DISP, ScrW, NEW_Y + Font_H+2*Y_DISP, 0, 0, 0 ,CURSOR_COLOUR, CURSOR_COLOUR_FRING);
      
    ITEM *it=GetItem(ALT_clist_count + ALT_disp);
    
    if(it->visible)
    if(it->ID!=0)
    {
    char * newname = convUTF8_to_ANSI_STR(it->Nick);
        
    int img=0;
    switch (it->Status)
    {
    case ICQ_STATUS_OFFLINE:   img=IMG_OFFLINE;break;
    case ICQ_STATUS_ONLINE:    img=IMG_ONLINE;break;
    case ICQ_STATUS_AWAY:      img=IMG_AWAY;break;
    case ICQ_STATUS_DND:       img=IMG_DND;break; 
    case ICQ_STATUS_NA:        img=IMG_NA;break;
    case ICQ_STATUS_OCCUPIED:  img=IMG_OCCUPIED;break;
    case ICQ_STATUS_FREE4CHAT: img=IMG_FFC;break;
    case ICQ_STATUS_INVISIBLE: img=IMG_INVISIBLE;break;
    case ICQ_STATUS_EVIL     : img=IMG_EVIL;break;
    case ICQ_STATUS_DEPRESSION:img=IMG_DEPRESSION;break;
    case ICQ_STATUS_HOME     : img=IMG_HOME;break;
    case ICQ_STATUS_WORK     : img=IMG_WORK;break;
    case ICQ_STATUS_LUNCH    : img=IMG_LUNCH;break;
    }
    DrawImg(0, NEW_Y + Y_DISP, IconPack[img]);

    int offset=16;
    /////// так для проверки
    if (it->XStatus>0)
    {
      DrawImg(offset, NEW_Y + Y_DISP, IconPack[IMG_XStatus0+it->XStatus]);
      offset+=16+1;
    }
    char fullname[128];
    if (it->client_id>0)
      sprintf(fullname,"%s (%s)",newname,clientDB[it->client_id-1]);  
   else
      sprintf(fullname,"%s",newname);
    
    wsprintf(item_data, percent_t, fullname);
    DrawString(item_data, offset, NEW_Y + Y_DISP, ScrW, NEW_Y + Y_DISP + Font_H, FONT, 32 , COLOUR, COLOUR_FRING);
    }
    else
    {//group
      char * newname = convUTF8_to_ANSI_STR(it->Nick);
      int img=0;
      if (it->iscollapsed)
        img = IMG_GROUP_ON;
      else
        img = IMG_GROUP_OFF;
      DrawImg(0, NEW_Y + Y_DISP, IconPack[img]);
      wsprintf(item_data, percent_t, newname);
      DrawString(item_data, 16, NEW_Y + Y_DISP, ScrW, NEW_Y + Y_DISP + Font_H, FONT, 32 , COLOUR, COLOUR_FRING);
    }
    
    
    
  }
  
  FreeWS(item_data); 
}
*/
static void DrawContactList(CONTACT_LIST_DESC *data)
{
  
  char header_text[128]="";
  sprintf(header_text,"%d/%d", c_list_cursor_pos, c_list_max_contacts);
  InitHeaderText(&head_c_list, header_text);
  DrawHeaderText(&head_c_list);
  
  
  int VisibleItem=CountVisibleItem();
  c_list_max_contacts=VisibleItem;
  int Y_DISP = data->y_disp;
  WSHDR *item_data = AllocWS(128);
  
  int itemH = 2*( data->y_disp) + GetImgHeight(IconPack[IMG_ONLINE]);
  
  int b_item=0;
  int e_item=c_list_max_show_n;
  int alt_b_item=0;
  // если все видимые не помещаются, то сдвигаем окно в зависимости от курсора
  if (c_list_max_show_n<VisibleItem)
  {
    if (c_list_cursor_pos>c_list_max_show_n/2)
      b_item=c_list_max_show_n/2-c_list_cursor_pos;

    if (VisibleItem+b_item < c_list_max_show_n)
        b_item=c_list_max_show_n - VisibleItem+1;
   alt_b_item=-b_item;
  }
  
  if(!Itemtop) return;
  ITEM *bmk;
  bmk=Itemtop;
  int c=0;
  while(bmk) 
  {
    if (bmk->visible)
    {
      if ((b_item>=0)&&(b_item<=e_item))
      {

        int NEW_Y =  header_height + (itemH) * (b_item);
        
        if(alt_b_item+b_item==c_list_cursor_pos)
        {
        DrawRoundedFrame(0, NEW_Y+Y_DISP, ScrW, NEW_Y + itemH, 0, 0, 0 ,CURSOR_COLOUR, CURSOR_COLOUR_FRING);
        c_list_cursor_item=c;
        }
    

        if(bmk->ID!=0) // контакт
            {
            char * newname = convUTF8_to_ANSI_STR(bmk->Nick);
                
            int img=0;
            switch (bmk->Status)
            {
            case ICQ_STATUS_OFFLINE:   img=IMG_OFFLINE;break;
            case ICQ_STATUS_ONLINE:    img=IMG_ONLINE;break;
            case ICQ_STATUS_AWAY:      img=IMG_AWAY;break;
            case ICQ_STATUS_DND:       img=IMG_DND;break; 
            case ICQ_STATUS_NA:        img=IMG_NA;break;
            case ICQ_STATUS_OCCUPIED:  img=IMG_OCCUPIED;break;
            case ICQ_STATUS_FREE4CHAT: img=IMG_FFC;break;
            case ICQ_STATUS_INVISIBLE: img=IMG_INVISIBLE;break;
            case ICQ_STATUS_EVIL     : img=IMG_EVIL;break;
            case ICQ_STATUS_DEPRESSION:img=IMG_DEPRESSION;break;
            case ICQ_STATUS_HOME     : img=IMG_HOME;break;
            case ICQ_STATUS_WORK     : img=IMG_WORK;break;
            case ICQ_STATUS_LUNCH    : img=IMG_LUNCH;break;
            }
            if (bmk->istyping!=0) img=IMG_TYPING;
            
            
            DrawImg(Y_DISP, NEW_Y + Y_DISP, IconPack[img]);
        
            int offset=Y_DISP+itemH;
            if (bmk->istyping!=0)
            {
              DrawImg(offset, NEW_Y + Y_DISP, IconPack[IMG_TYPING]);
              offset+=itemH;
            }
            if (bmk->isunread!=0) 
            {
              DrawImg(offset, NEW_Y + Y_DISP, IconPack[IMG_MESSAGE]);
              offset+=itemH;
            }
            /////// так для проверки
            if (bmk->XStatus>0)
            {
              DrawImg(offset, NEW_Y + Y_DISP, IconPack[IMG_XStatus0+bmk->XStatus]);
              offset+=itemH;
            }
            char fullname[128];
            if (bmk->client_id>0)
            {
              sprintf(fullname,"%s (%s)",newname,clientDB[bmk->client_id-1].name);  
              img=clientDB[bmk->client_id-1].img;
              DrawImg(ScrW-itemH, NEW_Y + Y_DISP, IconPack[img]);
            }
           else
              sprintf(fullname,"%s",newname);
            
            wsprintf(item_data, percent_t, fullname);
            DrawString(item_data, offset, NEW_Y + Y_DISP, ScrW, NEW_Y + Y_DISP + Font_H, FONT, 32 , COLOUR, COLOUR_FRING);
            }
            else
            {//group
              char * newname = convUTF8_to_ANSI_STR(bmk->Nick);
              int img=0;
              if (bmk->iscollapsed)
                img = IMG_GROUP_ON;
              else
                img = IMG_GROUP_OFF;
              DrawImg(Y_DISP, NEW_Y + Y_DISP, IconPack[img]);
              wsprintf(item_data, percent_t, newname);
              DrawString(item_data, itemH, NEW_Y + Y_DISP, ScrW, NEW_Y + itemH, FONT, 32 , COLOUR, COLOUR_FRING);
            }
        
      }
    ++b_item;
    }
    ++c;
    bmk=bmk->next;  
  }
  
 FreeWS(item_data);
  
}

  


void OnRedraw_C_List()
{
    DrawCListFon();
    DrawContactList(&main_c_list);
    DrawSoftButton(&c_list_soft);
}

          
void Init_C_List()
{
  GroupVisible(1);
  InitHeaderData(&head_c_list, "Контакты", 2, FONT, 4, COLOUR, COLOUR_FRING);

  
  GetShowsNumContacts(&main_c_list, Get_HeaderText_Height(&head_c_list),Get_SoftButton_Height(&c_list_soft));
  header_height = Get_HeaderText_Height(&head_c_list);
     
  //main_c_list.y_disp=header_height;
}



