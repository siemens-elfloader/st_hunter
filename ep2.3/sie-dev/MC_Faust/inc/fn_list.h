#ifndef _FN_LIST_H
#define _FN_LIST_H

typedef struct
{
	int ftype;
	int pname;
        char* sname;
	char* full;
        char* zipPath;
	void* next;
} FN_ITM;

typedef struct
{
	int count;
	int type;
	FN_ITM *items;
} FN_LIST;

void fn_zero(FN_LIST *list);
void fn_fill(FN_LIST *list, char* path);
void fn_add(FN_LIST *list, int type, int ftype, int pname, char* full, char* zipPath, char* sname);
void fn_rev(FN_LIST *list);
void fn_free(FN_LIST *list);
void fn_log(FN_LIST *list);
int fn_inlist(FN_LIST *list, char *full);

#endif
