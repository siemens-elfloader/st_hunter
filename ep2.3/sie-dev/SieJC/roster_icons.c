#include "../inc/swilib.h"
#include "main.h"
#include "jabber.h"
#include "clist_util.h"
#include "jabber_util.h"
#include "string_util.h"
#include "history.h"
#include "transports_icons.h"

unsigned int conference = 0x18C;

unsigned int noauth = 0x185;
unsigned int ask = 0x185;

unsigned int message = 0x24C; //0x22E;
unsigned int system = 0x24C;

unsigned int online = 0x22C; //0x22D;
unsigned int unavailable = 0x306;
unsigned int error = 0x185;
unsigned int chat = 0x18C; //0x22B;
unsigned int away = 0x22F;
unsigned int xa = 0x17A;
unsigned int dnd = 0x231;
unsigned int invisible = 0x306;

extern const char* PRESENCES[PRES_COUNT];
extern const char PATH_TO_PIC[];


char* Roster_getIconByStatus(char* path_to_pic, char status)   //вернет путь к картинке по статусу
{
	if (!path_to_pic)
		path_to_pic = malloc(256);
	strcpy(path_to_pic, PATH_TO_PIC);

	// Если непрочитанных сообщений нет
	if(status < PRESENCE_INVISIBLE) // Если адекватный статус
		strcat(path_to_pic, PRESENCES[status]);
	else if( status == 50)
		strcat(path_to_pic, "message");
	else
		strcat(path_to_pic, PRESENCES[PRESENCE_OFFLINE]); // Иначе типа оффлайн
	strcat(path_to_pic, ".png");
	return path_to_pic;
}

void Roster_getIcon(char* path_to_pic, CLIST* ClEx, TRESOURCE* resEx)
{
	strcpy(path_to_pic, PATH_TO_PIC);

	// Если это конференция
	if(resEx->entry_type == T_CONF_ROOT && !resEx->has_unread_msg)
	{
		strcat(path_to_pic, ClEx->IsVisible ? "conference_on" : "conference_off");
		goto L_DONE;
	}

	//отображаем компосинг только если нет не прочитаных сообщений, и это просто контакт\участник конфы
	if((resEx->compos) && (!resEx->has_unread_msg) && ((resEx->entry_type == T_NORMAL) || (resEx->entry_type == T_CONF_NODE))) //если ктото чтото набирает...
	{
		strcat(path_to_pic, "composing");
		goto L_DONE;
	}

	// Если это члены конференции и они живы
	if(resEx->entry_type == T_CONF_NODE && !resEx->has_unread_msg && resEx->status <= PRESENCE_INVISIBLE)
	{
		GetTransportIconsPath(path_to_pic, resEx);
		if(resEx->status == PRESENCE_ONLINE)
		{
			if(resEx->muc_privs.role == ROLE_VISITOR)strcat(path_to_pic, "vis_");
			if(resEx->muc_privs.role == ROLE_PARTICIPANT && resEx->muc_privs.aff == AFFILIATION_NONE)strcat(path_to_pic, "part_");
			if(resEx->muc_privs.role == ROLE_MODERATOR)strcat(path_to_pic, "admin_");
		}
		else
		{
			// Если статус не онлайн, а другой
			if(resEx->muc_privs.role == ROLE_MODERATOR)
			{
				strcat(path_to_pic, "admin_");
			}
			else if(resEx->muc_privs.aff == AFFILIATION_NONE) strcat(path_to_pic, "vis_part_");
		}

		strcat(path_to_pic, PRESENCES[resEx->status]);
		goto L_DONE;
	}


	// Если это группа
	if(resEx->entry_type == T_GROUP)
	{
		ClEx->IsVisible == 1 ? strcat(path_to_pic, "groupon") : strcat(path_to_pic, "groupoff");
		goto L_DONE;
	}

	// Если у нас нет подписки и у контакта нет непрочитанных сообщений
	if(((ClEx->subscription == SUB_FROM) || (ClEx->subscription == SUB_NONE)) && !resEx->has_unread_msg)
	{
		GetTransportIconsPath(path_to_pic, resEx);
		strcat(path_to_pic, "noauth"); // иконка "нет авторизации"
		goto L_DONE;
	}

	// Если стоит флаг запроса подписки
	if(ClEx->wants_subscription)
	{
		GetTransportIconsPath(path_to_pic, resEx);
		strcat(path_to_pic, "ask");
		goto L_DONE;
	}

	// Если у нормального контакта есть непрочитанные сообщения
	if(resEx->has_unread_msg)
	{
		// Если у него к тому же и статус адекватный
		if(resEx->status <= PRESENCE_INVISIBLE)
		{
			strcat(path_to_pic, "message");
		}
		else
		{
			strcat(path_to_pic, "system");      // А иначе он что-то замутил с подпиской
		}
	}
	else
	{
		if(resEx->status < PRESENCE_ERROR)
			GetTransportIconsPath(path_to_pic, resEx);
		// Если же непрочитанных сообщений нет
		if(resEx->status <= PRESENCE_ERROR) // Если адекватный статус
		{
			strcat(path_to_pic, PRESENCES[resEx->status]);
		}
		else
		{
			strcat(path_to_pic, PRESENCES[PRESENCE_OFFLINE]); // Иначе типа оффлайн
		}
	}
L_DONE:
	strcat(path_to_pic, ".png");
}

void Roster_DrawIcon(unsigned short x, unsigned short y, unsigned int img_num)
{
	DrawImg(x, y, img_num);
}

int Roster_getIconWidth(char* path_to_pic)
{
	return GetImgWidth((int)path_to_pic);
}

int Roster_getIconHeight(char* path_to_pic)
{
	return GetImgHeight((int)path_to_pic);
}
