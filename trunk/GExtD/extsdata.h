/*
 * File:   extsdata.h
 * Author: Tonich
 *
 * Created on 2 Ноябрь 2011 г., 15:42
 */

#ifndef EXTSDATA_H
#define	EXTSDATA_H

#ifdef	__cplusplus
extern "C" {
#endif




typedef struct
{
  const char *ext;
  const char *small_png;
  int zero_small;
  const char *large_png;
  int zero_large;
  const char *elf;
  const char *altelf;
}ES;

typedef struct _ext_list{
    ES item;
    int unical_id;
    struct _ext_list *next,*prev;
}ESL;

struct _es_info{
    ESL *root,*cur,*end;
    int ES_count;
}ES_INFO;

int do_ext(WSHDR *filename, WSHDR *ext, void *param);
int do_alternate(WSHDR *filename, WSHDR *ext, void *param);
void ExtsClear(ESL *root);
void UnregAll();
ESL *FindByExt(const char *ext);
void AddToES_INFO(ES *ext,int id);
int AddExtFromStrs(SSIM *strlist);
int RegExt(ESL *data);
int RegExts(ESL *data);

#ifdef	__cplusplus
}
#endif

#endif	/* EXTSDATA_H */

