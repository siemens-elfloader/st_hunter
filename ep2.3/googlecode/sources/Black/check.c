#include "..\inc\swilib.h"

//#pragma swi_number=0x0248
//__swi void GetCalleeNumber(int index, WSHDR *ws);

extern void GetCalleeNumber(int index, WSHDR *ws);

__arm int do_check(int group)
{
        char path[]="0:\\Misc\\BlackList.txt";
        unsigned int err;
        int f=fopen(path, A_ReadOnly, P_READ, &err);
        if(f<0)
                return 0;//0,不在黑名单或者名单不存在
        char *buf=malloc(4096);
        int size=fread(f, buf, 4096, &err);
        fclose(f, &err);
        char *p;
        p=strstr(buf, "[Profile]:");
        if(p)
        {
                p+=10;
                if((*p)>='1'&&(*p)<='8')
                {
                        if((*p-'1')!=GetProfile())
                                return 0;
                }
                //int pf=GetProfile();
        }
        p=strstr(buf, "[Group]:");
        if(p)
        {
                p+=8;
                if((*p)>='1'&&(*p)<='9')
                {
                        if(group==(*p)-0x2e)
                                return 1;
                }
        }
        if(size>=0)
                buf[size]=0;
        char num[32];
        WSHDR *ws=AllocWS(32);
        GetCalleeNumber(IsCalling(), ws);
        ws_2str(ws, num, 32);
        char *s=num;
        if(strncmp(num, "+86", 3)==0)
                s+=3;
        //char *
        p=strstr(buf, s);
        int rec;
        s=p-1;
        if(p)
        {
                if(s)
                {
                        if((*s)>='0'&&(*s)<='9')
                                rec=0;
                        else
                                rec=1;
                }
                else
                        rec=1;//拒接
        }
        else
                rec=0;
        mfree(buf);
        FreeWS(ws);
        return rec;
}
