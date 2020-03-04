#include "../inc/swilib.h"
#include "lang.h"

char *LGP[LGP_TOTAL];

void def_init_lang(int id)
{
	const char *def_lgp[]={"Loading...\nPlease wait", "StopWatch", "Can't open file!", "Nothing to save!",
		"Error occured while saving!", "Saving", "NstopWatch configuration updated", "Exit?"};
	unsigned int len;
	while(id<LGP_TOTAL)
	{
		len=strlen(def_lgp[id]);
		LGP[id]=malloc(len+1);
		memcpy(LGP[id], def_lgp[id], len);
		LGP[id++][len]='\0';
	}
}

int Langpack_Init(void)
{
	extern char *root_folder;
	char fname[128];
	sprintf(fname, "%slang.txt", root_folder);
	
	unsigned int err;
	FSTATS fs;
	char *buf=NULL;
	int id=0;
	if(GetFileStats(fname, &fs, &err)==-1) goto DEFAULT;
	
	int fp=fopen(fname, A_ReadOnly + A_BIN, P_READ, &err);
	if(fp==-1) goto DEFAULT;
	
	buf=malloc(fs.size+1);
	if(fread(fp, buf, fs.size, &err)==-1) goto DEFAULT;
	fclose(fp, &err);
	
	char *tmp=NULL;
	int pos=0, len=0;
	while(id<LGP_TOTAL && pos<fs.size)
	{
		if(buf[pos]=='\n' || buf[pos]=='\r')
		{
			if(len)
			{
				LGP[id]=malloc(len+1);
				memcpy(LGP[id], tmp, len);
				LGP[id++][len]='\0';
				tmp=realloc(tmp, 1);
				len=0;
			}
		}
		else
		{
			tmp=realloc(tmp, len+1);
			tmp[len]=buf[pos];
			if(tmp[len]=='|') tmp[len]='\n';
			len++;
		}
		pos++;
	}
	if(tmp) mfree(tmp);
	
	if(len>0 && id<LGP_TOTAL)//eof
	{
		LGP[id]=malloc(len+1);
		memcpy(LGP[id], buf+fs.size-len, len);
		LGP[id++][len]='\0';
	}
	DEFAULT:
	if(buf) mfree(buf);
	def_init_lang(id);
	return 1;
}

void Langpack_Free(void)
{
	for(int i=0; i<LGP_TOTAL; i++)
		if(LGP[i]) mfree(LGP[i]);
}
