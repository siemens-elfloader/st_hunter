#include "../inc/swilib.h"
#include  "../inc/cfg_items.h"
#include "conf_loader.h"

char *successed_config_filename;

#pragma segment="CONFIG_C"
int SaveConfigData(const char * fname)   // result 0 - конфига не создан, 1 - cоздан
{
    int f;
    unsigned int ul;
    int result=0;
    unsigned int wlen;
    unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

    if ((f=fopen(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
    {
        wlen=fwrite(f,__segment_begin("CONFIG_C"),len,&ul);
        fclose(f,&ul);
        if (wlen==len) result=1;
    }
    return(result);
}

int LoadConfigData(const char *fname)
{
    int f;
    unsigned int ul;
    char *buf;
    int result=0;
    void *cfg;
    unsigned int rlen, end;

    extern const CFG_HDR cfghdr1; //first var in CONFIG
    cfg=(void*)&cfghdr1;

    unsigned int len=(int)__segment_end("CONFIG_C")-(int)__segment_begin("CONFIG_C");

    if (!(buf=(char*)malloc(len))) return -1;
    if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
    {
        rlen=fread(f,buf,len,&ul);
        end=lseek(f,0,S_END,&ul,&ul);
        fclose(f,&ul);
        if (rlen!=end || rlen!=len)  goto L_SAVENEWCFG;
        memcpy(cfg,buf,len);
        result=1;
    }
    else
    {
L_SAVENEWCFG:
        result=SaveConfigData(fname);
    }
    mfree(buf);
    if (result>0)
    {
        if(!successed_config_filename) successed_config_filename=(char*)malloc(strlen(fname)+1);
        strcpy(successed_config_filename, fname);
    }
    return(result);
}

void InitConfig(void)
{
    if(LoadConfigData("4:\\ZBin\\etc\\InfoButton.bcfg")<=0)
    {
        LoadConfigData("0:\\ZBin\\etc\\InfoButton.bcfg");
    }
}
