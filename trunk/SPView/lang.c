#include <swilib.h>
#include "lang.h"

char *lgp[lgp_total];

void def_init_lang(int id)
{
    const char *def_lgp[]={"Options", "Exit", "Enter number", "Save...",
    "About", "Select", "Back", "Pic ID:", "View", /*"Error\nPath not found!",*/
    "Save", "First:", "Last:", "%d icons saved"};
    unsigned int len;
    while(id<lgp_total)
    {
        len=strlen(def_lgp[id]);
        lgp[id]=malloc(len+1);
        memcpy(lgp[id], def_lgp[id], len);
        lgp[id++][len]='\0';
    }
}

int InitLangpack(void)
{
    extern char main_folder[128];
    char fname[128];
    sprintf(fname, "%slang.txt", main_folder);

    unsigned int err;
    FSTATS fs;
    unsigned int id=0;
    char *buf=NULL;

    if(GetFileStats(fname, &fs, &err)==-1) goto DEFAULT;

    int fp=_open(fname, A_ReadOnly + A_BIN, P_READ, &err);
    if(fp==-1) goto DEFAULT;

    buf=malloc(fs.size+1);
    if(_read(fp, buf, fs.size, &err)==-1) goto DEFAULT;
    _close(fp, &err);

    char *tmp=NULL;
    unsigned int pos=0, len=0;
    while(id<lgp_total && pos<fs.size)
    {
	if(buf[pos]=='\n' || buf[pos]=='\r')
	{
	    if(len)
	    {
		lgp[id]=malloc(len+1);
		memcpy(lgp[id], tmp, len);
		lgp[id++][len]='\0';
		tmp=realloc(tmp, 1);
		len=0;
	    }
	}
	else
	{
	    tmp=realloc(tmp, len+1);
	    tmp[len]=buf[pos];
	    //if(tmp[len]=='|') tmp[len]='\n';
	    len++;
	}
	pos++;
    }
    mfree(tmp);

    if(len>0 && id<lgp_total)//eof
    {
	lgp[id]=malloc(len+1);
	memcpy(lgp[id], buf+fs.size-len, len);
	lgp[id++][len]='\0';
    }
    DEFAULT:
    if(buf) mfree(buf);
    if(id<lgp_total) def_init_lang(id);
    return 1;
}

void FreeLangPack(void)
{
    for(int i=0; i<lgp_total; i++)
    {
	if(lgp[i]) mfree(lgp[i]);
    }
}
