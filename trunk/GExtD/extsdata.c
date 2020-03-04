#include <stdio.h>
#include <stdlib.h>
#include <swilib.h>
#include <string.h>

#include "parser.h"
#include "extsdata.h"

const char empty_str[]="";

int unk_exists=0;               //Флаг использования универсального расширения...

const char universal_ext[]="unk";
const char	universal_txt[]="txt";

const char *uni_elf="";
const char *uni_altelf="";

const char *uni_large="";
const char *uni_small="";

int unical_id_mask=0x56;


REGEXPLEXT reg=
#ifdef NEWSGOLD
{
  NULL,
  0x55,
  0xFF,
  8, //Каталог Misc
  MENU_FLAG2,
  NULL,
  NULL,
  (int)"Open",    //LGP "Открыть"
  (int)"AltOpen", //LGP "Изменить"
  LGP_DOIT_PIC,
  (void *)do_ext,
  (void *)do_alternate
};
#else
{
  NULL,
  0x55,
  0xFF,           //Опции без "Задать как..."
  7,
  MENU_FLAG2,
  NULL,
  NULL,
  (void *)do_ext,
  (void *)do_alternate
};
#endif

REGEXPLEXT unk_ext=
#ifdef NEWSGOLD
{
  empty_str,
  0,
  0xFF,
  8, //Папка Misc
  MENU_FLAG2,
  NULL,
  NULL,
  (int)"Open",    //LGP "Открыть"
  (int)"AltOpen", //LGP "Изменить"
  LGP_DOIT_PIC,
  (void *)do_ext,
  (void *)do_alternate
};
#else
{
  NULL,
  0x7FFFFFFF,
  0,
  7,
  MENU_FLAG2,
  NULL,
  NULL,
  (void *)do_ext,
  (void *)do_alternate
};
#endif

char *todown(char *str){		//Преобразование в нижний регистр
  char *val=str;
  while(*val){
         if(*val>='A' && *val<='Z')  *val+=' ';
    else if(*val>='А' && *val<='П')  *val+=' ';
    else if(*val>='Р' && *val<='Я')  *val+= 80;
    ++val;
  }
  return str;
}

void ExtsClear(ESL *root){
    ESL *temp=root;
    while(root){
        temp=root;
        root=root->next;
        free(temp);
    }
}

void UnregAll()
{
  ES_INFO.cur=ES_INFO.root;
  while(ES_INFO.cur)
  {
      if(!ES_INFO.cur->unical_id)      //Если нашли универсальное расширение
          reg.ext=empty_str;
          else reg.ext=ES_INFO.cur->item.ext;
    reg.unical_id=ES_INFO.cur->unical_id;
    UnRegExplorerExt(&reg);
    ES_INFO.cur=ES_INFO.cur->next;
  }
  ExtsClear(ES_INFO.root);
  ES_INFO.root=NULL;
  ES_INFO.cur=ES_INFO.end=ES_INFO.root;
}

int do_elf(WSHDR *filename, WSHDR *ext, void *param, int mode)
{
  int i=0;
  char s[128];
  ws_2str(ext,s,126);
//  ES *p=es;
  ES_INFO.cur=ES_INFO.root;
  if (!*s)strcpy(s,universal_txt);
  while(ES_INFO.cur){
      if(!strcmp(s,ES_INFO.cur->item.ext)){
          WSHDR *elfname=AllocWS(256);
          str_2ws(elfname,mode?ES_INFO.cur->item.altelf:ES_INFO.cur->item.elf,126);
          ws_2str(filename,s,126);
          i=ExecuteFile(elfname,NULL,s);
          FreeWS(elfname);
          return(i);
      }
      ES_INFO.cur=ES_INFO.cur->next;
  }
  return(0);
}

int do_ext(WSHDR *filename, WSHDR *ext, void *param)
{
  return do_elf(filename,ext,param,0);
}

int do_alternate(WSHDR *filename, WSHDR *ext, void *param)
{
  return do_elf(filename,ext,param,1);
}

ESL *FindByExt(const char *ext){
    ES_INFO.cur=ES_INFO.root;
    while(ES_INFO.cur){
        if(!strcmp(ES_INFO.cur->item.ext,ext))return ES_INFO.cur;
        ES_INFO.cur=ES_INFO.cur->next;
    }
    return NULL;
}

void AddToES_INFO(ES *ext,int id){

    if(!ES_INFO.end){                   //Эт первый элемент
        ES_INFO.root=(ESL *)malloc(sizeof(ESL));
        memset(ES_INFO.root,0,sizeof(ESL));

        memcpy(&ES_INFO.root->item,ext,sizeof(ES));

        ES_INFO.root->unical_id=id;

        ES_INFO.cur=ES_INFO.root;
        ES_INFO.end=ES_INFO.root;
    }else{
        if((ES_INFO.cur=FindByExt(ext->ext))){
            memcpy(&ES_INFO.cur->item,ext,sizeof(ES));
        }else{
        ES_INFO.end->next=(ESL *)malloc(sizeof(ESL));
        memset(ES_INFO.end->next,0,sizeof(ESL));
        ES_INFO.end->next->prev=ES_INFO.end;

        ES_INFO.end=ES_INFO.end->next;
        memcpy(&ES_INFO.end->item,ext,sizeof(ES));
        ES_INFO.end->unical_id=id;
        ES_INFO.cur=ES_INFO.end;

        }
    }

}

int AddExtFromStrs(SSIM *strlist){
    SSIM *templ=strlist;
    char *titem=NULL;
    int id=1;
    ES extt;
    memset(&extt,0,sizeof(ES));

    while(templ){
        if(templ->comtype){             //А не коммент ли это
            templ=templ->next;
            continue;
        }else if(*(templ->str.str)=='['){     //Нашли сигнатуру расширения
            templ->str.str=todown(templ->str.str);      //К нижнему регистру
            if(!extt.ext){               //Первая сигнатура.
                templ->str.str[strlen(templ->str.str)-1]=0;
                templ->str.str[0]=0;
                templ->str.str++;
                extt.ext=templ->str.str;
                extt.elf=uni_elf;
                extt.altelf=uni_altelf;
                extt.large_png=uni_large;
                extt.small_png=uni_small;
            }else{                      //Сохраним то, что было и создадим новое
                if((!strncmp(extt.ext,universal_ext,3))&&!unk_exists){	//проверка на универсальное расширение
                    uni_elf=extt.elf;
                    uni_altelf=extt.altelf;
                    uni_large=extt.large_png;
                    uni_small=extt.small_png;
                    unk_exists=1;
                    AddToES_INFO(&extt,0);
                }else{
                AddToES_INFO(&extt,id+unical_id_mask);
                id++;
                }
                memset(&extt,0,sizeof(ES));
                templ->str.str[strlen(templ->str.str)-1]=0;
                templ->str.str[0]=0;
                templ->str.str++;
                extt.ext=templ->str.str;
                extt.elf=uni_elf;
                extt.altelf=uni_altelf;
                extt.large_png=uni_large;
                extt.small_png=uni_small;
            }

        }else if(!strncmp(templ->str.str,"RUN=",4)){
            titem=templ->str.str+4;
            if(titem)extt.elf=titem;
        }else if(!strncmp(templ->str.str,"ALTRUN=",7)){
            titem=templ->str.str+7;
            if(titem)extt.altelf=titem;
        }else if(!strncmp(templ->str.str,"SMALL=",6)){
            titem=templ->str.str+6;
            if(titem)extt.small_png=titem;
        }else if(!strncmp(templ->str.str,"BIG=",4)){
            titem=templ->str.str+4;
            if(titem)extt.large_png=titem;
        }

        templ=templ->next;
    }

    if((!strncmp(extt.ext,universal_ext,3))&&unk_exists){
                    AddToES_INFO(&extt,0);
                    uni_elf=extt.elf;
                    uni_altelf=extt.altelf;
                    uni_large=extt.large_png;
                    uni_small=extt.small_png;
                    unk_exists=1;
                }else{
                AddToES_INFO(&extt,id+unical_id_mask);
                id++;
                }

    return 0;
}

int RegExt(ESL *data){
	ESL *temp=data;
	int id=0;
    WSHDR *extws;

    REGEXPLEXT* regit=malloc(sizeof(REGEXPLEXT));
    extws=AllocWS(256);

    str_2ws(extws,temp->item.ext,255);
	id=GetExtUid_ws(extws);
    if(id){
            TREGEXPLEXT *pr=get_regextpnt_by_uid(id);
            temp->unical_id=id;
#ifdef NEWSGOLD
            memcpy(regit,pr,sizeof(TREGEXPLEXT));
#else
            regit->ext=temp->item.ext;
            regit->unical_id=id;
            regit->enabled_options=pr->enabled_options;
            regit->obex_path_id=pr->obex_path_id;
            regit->menu_flag=pr->menu_flag;
#endif
	    UnRegExplorerExt(regit);
            // Разуплотнили,пишем картинки
#ifdef NEWSGOLD
            if(temp->item.small_png != uni_small) regit->icon1=(int *)&(temp->item.small_png);
            if(temp->item.large_png != uni_large) regit->icon2=(int *)&(temp->item.large_png);
            if(temp->item.elf != uni_elf) regit->proc = (void *)do_ext;
	    if(temp->item.altelf != uni_altelf) regit->altproc = (void *)do_alternate;
#else
            regit->icon1 = (temp->item.small_png != uni_small) ? (int *)&(temp->item.small_png) : (int*)pr->icon1;
            regit->icon2 = (temp->item.large_png != uni_large) ? (int *)&(temp->item.large_png) : (int*)pr->icon2;
            regit->proc = (temp->item.elf != uni_elf) ? (void *)do_ext : pr->proc;
            regit->altproc = (temp->item.altelf != uni_altelf) ? (void *)do_alternate : pr->altproc;
#endif
	  }else{                //Вроде как новое расширение

            memcpy(regit,&reg,sizeof(REGEXPLEXT));
            if(temp->unical_id)regit->ext=temp->item.ext;
            else{                               //А вдруг просто универсальное?
			regit->ext=empty_str;
            }
	  regit->icon1=(int *)&(temp->item.small_png);
	  regit->icon2=(int *)&(temp->item.large_png);
	  regit->unical_id=temp->unical_id;
          }
			RegExplorerExt(regit);
			free(regit);
			FreeWS(extws);
	return 0;
}

int RegExts(ESL *data){
    ESL *temp=data;
    int id=0;
    WSHDR *extws;

    REGEXPLEXT* regit=malloc(sizeof(REGEXPLEXT));
    extws=AllocWS(256);
    while(temp){        //Пока мы можем

                                //Сморим есть ли уже такое в зарегеных
	str_2ws(extws,temp->item.ext,255);
	id=GetExtUid_ws(extws);
        if(id){
            TREGEXPLEXT *pr=get_regextpnt_by_uid(id);
            temp->unical_id=id;
#ifdef NEWSGOLD
            memcpy(regit,pr,sizeof(TREGEXPLEXT));
#else
            regit->ext=temp->item.ext;
            regit->unical_id=id;
            regit->enabled_options=pr->enabled_options;
            regit->obex_path_id=pr->obex_path_id;
            regit->menu_flag=pr->menu_flag;
#endif
	    UnRegExplorerExt(regit);
            // Разуплотнили,пишем картинки
#ifdef NEWSGOLD
            if(temp->item.small_png != uni_small) regit->icon1=(int *)&(temp->item.small_png);
            if(temp->item.large_png != uni_large) regit->icon2=(int *)&(temp->item.large_png);
            if(temp->item.elf != uni_elf) regit->proc = (void *)do_ext;
	    if(temp->item.altelf != uni_altelf) regit->altproc = (void *)do_alternate;
#else
            regit->icon1 = (temp->item.small_png != uni_small) ? (int *)&(temp->item.small_png) : (int*)pr->icon1;
            regit->icon2 = (temp->item.large_png != uni_large) ? (int *)&(temp->item.large_png) : (int*)pr->icon2;
            regit->proc = (temp->item.elf != uni_elf) ? (void *)do_ext : pr->proc;
            regit->altproc = (temp->item.altelf != uni_altelf) ? (void *)do_alternate : pr->altproc;
#endif
	  }else{                //Вроде как новое расширение

            memcpy(regit,&reg,sizeof(REGEXPLEXT));
            if(temp->unical_id)regit->ext=temp->item.ext;
            else{                               //А вдруг просто универсальное?
			regit->ext=empty_str;
            }
	  regit->icon1=(int *)&(temp->item.small_png);
	  regit->icon2=(int *)&(temp->item.large_png);
	  regit->unical_id=temp->unical_id;
          }
        RegExplorerExt(regit);

        temp=temp->next;
    }
    free(regit);
    FreeWS(extws);
    return 0;


}

