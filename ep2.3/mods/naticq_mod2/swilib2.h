typedef  unsigned char byte;
typedef  unsigned short word;
typedef  unsigned int dword;

#define PTT_BUTTON 0x27



#pragma swi_number=0x80D6
__swi __arm unsigned int* RamNetOnline();

#ifdef NEWSGOLD
#pragma swi_number=0x1FF
__swi __arm void MEDIA_PLAYLAST();

#pragma swi_number=0x1F8
__swi __arm unsigned int _GetPlayStatus(void);

#pragma swi_number=0x22C
__swi __arm int pdcache_getval(char* ret, int maxlen_of_return_str, char cachetype, char *pd_entry_identifier_class, char  *pd_entry_identifier_sub);
#pragma swi_number=0x22D
__swi __arm int pdcache_setval(char *new_value, char cachetype,  char *pd_entry_identifier_class, char *pd_entry_identifier_sub);

#endif

#pragma swi_number=0x41
__swi __arm char GetProvider(WSHDR *ws, int zero);
//thumb
//pattern=??,E7,??,B5,??,1C,??,1C,??,??,??,??,??,1C,??,1C,??,1C,??,??,??,FF,??,BD + 3

#ifdef ELKA
#pragma swi_number=54
__swi __arm void SLI_SetState(unsigned char state);
#endif


#ifdef NEWSGOLD
  //0080: 0xA0702CF5 ; 020: DrawScreenSaver
  #pragma swi_number=0x020          //by kluchnik 04.04.08
  __swi __arm int DrawScreenSaver();
  
  //0270: 0xA067109D ; 09C: CloseScreensaver
  #pragma swi_number=0x09C
  __swi __arm int CloseScreensaver();

  #pragma swi_number=0x0C5
  __swi __arm int IsIdleUiOnTop();//На сг нет этой функции
#else
  int IsIdleUiOnTop(void)
  { 
    void *icsm=FindCSMbyID(CSM_root()->idle_id); 
    if (IsGuiOnTop(((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4 ])) 
      return(1); 
    else
    return(0); 
  } 
#endif

