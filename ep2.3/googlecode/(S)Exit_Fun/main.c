//(S)Exit_fun.elf - Экзотический PickOFF! 
//(c)Dimadze

//Если не компилит? (Не?слов?"Linking"), Используйт?"Clean"

#include "..\inc\swilib.h"
#include "conf_loader.h"


int start = 1;

//Переменные из BCFG - конфиг?
extern const int bcfg_phone;
extern const char bcfg_errfname[];
extern const char bcfg_errstring[];
extern const char bcfg_errcode[];



short str2hex(const char *str)
{
  short i;
  sscanf(str,"%08X",&i);
  return i;
}

void StoreinSExit(const char *Filename, const char *string, short code)
{
  char s[48];
  sprintf(s,"\1\1%s",Filename);
  
  if (bcfg_phone == 0)
  {   
   extern void StoreErrInfoAndAbort_cx75v12(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_cx75v12(const char *);
   StoreErrString_cx75v12(string);
   loopback2();
   StoreErrInfoAndAbort_cx75v12(code,s,2,2);
  }
  if (bcfg_phone == 1)
  {   
   extern void StoreErrInfoAndAbort_cx75v13(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_cx75v13(const char *);
   StoreErrString_cx75v13(string);
   loopback2();
   StoreErrInfoAndAbort_cx75v13(code,s,2,2);
  }
  if (bcfg_phone == 2)
  {   
   extern void StoreErrInfoAndAbort_cx75v25(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_cx75v25(const char *);
   StoreErrString_cx75v25(string);
   loopback2();
   StoreErrInfoAndAbort_cx75v25(code,s,2,2);
  }
  if (bcfg_phone == 3)
  {   
   extern void StoreErrInfoAndAbort_m75v25(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_m75v25(const char *);
   StoreErrString_m75v25(string);
   loopback2();
   StoreErrInfoAndAbort_m75v25(code,s,2,2);
  }
  if (bcfg_phone == 4)
  {   
   extern void StoreErrInfoAndAbort_c72v22(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_c72v22(const char *);
   StoreErrString_c72v22(string);
   loopback2();
   StoreErrInfoAndAbort_c72v22(code,s,2,2);
  }
  if (bcfg_phone == 5)
  {   
   extern void StoreErrInfoAndAbort_c75v22(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_c75v22(const char *);
   StoreErrString_c75v22(string);
   loopback2();
   StoreErrInfoAndAbort_c75v22(code,s,2,2);
  }
  if (bcfg_phone == 6)
  {   
   extern void StoreErrInfoAndAbort_cx70v56(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_cx70v56(const char *);
   StoreErrString_cx70v56(string);
   loopback2();
   StoreErrInfoAndAbort_cx70v56(code,s,2,2);
  }
 if (bcfg_phone == 7)
  {   
   extern void StoreErrInfoAndAbort_s65v58(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_s65v58(const char *);
   StoreErrString_s65v58(string);
   loopback2();
   StoreErrInfoAndAbort_s65v58(code,s,2,2);
  }
 if (bcfg_phone == 8)
  {   
   extern void StoreErrInfoAndAbort_sk65v50(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_sk65v50(const char *);
   StoreErrString_sk65v50(string);
   loopback2();
   StoreErrInfoAndAbort_sk65v50(code,s,2,2);
  }
 if (bcfg_phone == 9)
  {   
   extern void StoreErrInfoAndAbort_sl65v53(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_sl65v53(const char *);
   StoreErrString_sl65v53(string);
   loopback2();
   StoreErrInfoAndAbort_sl65v53(code,s,2,2);
  }
 if (bcfg_phone == 10)
  {   
   extern void StoreErrInfoAndAbort_s68v47(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_s68v47(const char *);
   StoreErrString_s68v47(string);
   loopback2();
   StoreErrInfoAndAbort_s68v47(code,s,2,2);
  }
 if (bcfg_phone == 11)
  {   
   extern void StoreErrInfoAndAbort_S7Cv47(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_S7Cv47(const char *);
   StoreErrString_S7Cv47(string);
   loopback2();
   StoreErrInfoAndAbort_S7Cv47(code,s,2,2);
  }
  if (bcfg_phone == 12)
  {   
   extern void StoreErrInfoAndAbort_s75v47(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_s75v47(const char *);
   StoreErrString_s75v47(string);
   loopback2();
   StoreErrInfoAndAbort_s75v47(code,s,2,2);
  }
  if (bcfg_phone == 13)
  {   
   extern void StoreErrInfoAndAbort_s75v52(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_s75v52(const char *);
   StoreErrString_s75v52(string);
   loopback2();
   StoreErrInfoAndAbort_s75v52(code,s,2,2);
  }
  if (bcfg_phone == 14)
  {   
   extern void StoreErrInfoAndAbort_sl75v47(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_sl75v47(const char *);
   StoreErrString_sl75v47(string);
   loopback2();
   StoreErrInfoAndAbort_sl75v47(code,s,2,2);
  }
  if (bcfg_phone == 15)
  {   
   extern void StoreErrInfoAndAbort_sl75v52(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_sl75v52(const char *);
   StoreErrString_sl75v52(string);
   loopback2();
   StoreErrInfoAndAbort_sl75v52(code,s,2,2);
  }
  if (bcfg_phone == 16)
  {   
   extern void StoreErrInfoAndAbort_c81v51(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_c81v51(const char *);
   StoreErrString_c81v51(string);
   loopback2();
   StoreErrInfoAndAbort_c81v51(code,s,2,2);
  }
  if (bcfg_phone == 17)
  {   
   extern void StoreErrInfoAndAbort_m81v51(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_m81v51(const char *);
   StoreErrString_m81v51(string);
   loopback2();
   StoreErrInfoAndAbort_m81v51(code,s,2,2);
  }
  if (bcfg_phone == 18)
  {   
   extern void StoreErrInfoAndAbort_e71v45(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_e71v45(const char *);
   StoreErrString_e71v45(string);
   loopback2();
   StoreErrInfoAndAbort_e71v45(code,s,2,2);
  }
  if (bcfg_phone == 19)
  {   
   extern void StoreErrInfoAndAbort_el71v41(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_el71v41(const char *);
   StoreErrString_el71v41(string);
   loopback2();
   StoreErrInfoAndAbort_el71v41(code,s,2,2);
  }
  if (bcfg_phone == 20)
  {   
   extern void StoreErrInfoAndAbort_el71v45(int code,const char *module_name,int type,int unk3);
   extern void StoreErrString_el71v45(const char *);
   StoreErrString_el71v45(string);
   loopback2();
   StoreErrInfoAndAbort_el71v45(code,s,2,2);
  }

}
extern void kill_data(void *p, void (*func_p)(void *));

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

int main()
{
  InitConfig();
  if (start == 1) StoreinSExit(bcfg_errfname, bcfg_errstring, str2hex(bcfg_errcode));
  SUBPROC((void *)Killer);
  return 0;
}
