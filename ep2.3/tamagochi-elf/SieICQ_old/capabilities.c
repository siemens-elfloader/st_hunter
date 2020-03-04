#include "include.h"
#include "capabilities.h"

int capcmp(const char *s, const char *d, int len)
{
  for (int i=0; i<len; i++)
    if (s[i]!=d[i])
      return 0;
  return 1;
}


int FindCapabilities(char * buf, int len, const char * cap)
{
  int count= len/0x10;
  for (int i=0; i<count; i++)
  {
    int f=capcmp(&buf[i*0x10] ,cap,0x10);
    if (f) return 1;
  }
  return 0;
}

int FindXStatus(char * buf, int len)
{
  int count= len/0x10;
  for (int i=0; i<count; i++)
  {
    for (int x=0; x<XSTATUS_COUNT; x++)
    {  
     int f=capcmp(&buf[i*0x10] ,&capXStatus[x*0x10],0x10);
     if (f) return x+1;
    }
  }
  return 0;
}

/*
*	 WildCompare
*	 Compare 'name' string with 'mask' strings.
*	 Masks can contain '*' or '?' wild symbols
*	 Asterics '*' symbol covers 'empty' symbol too e.g WildCompare("Tst","T*st*"), returns TRUE
*	 In order to handle situation 'at least one any sybol' use "?*" combination:
*	 e.g WildCompare("Tst","T?*st*"), returns FALSE, but both WildCompare("Test","T?*st*") and
*	 WildCompare("Teeest","T?*st*") return TRUE.
*
*	 Function is case sensitive! so convert input or modify func to use _qtoupper()
*
*	 Mask can contain several submasks. In this case each submask (including first)
*	 should start from '|' e.g: "|first*submask|second*mask".
*
*   Dec 25, 2006 by FYR:
*   Added Exception to masks: the mask "|^mask3|mask2|mask1" means:
*   if NOT according to mask 3 AND (mask1 OR mask2)
*   EXCEPTION should be BEFORE main mask:
*     IF Exception match - the comparing stops as FALSE
*     IF Exception does not match - comparing continue
*     IF Mask match - comparing stops as TRUE
*     IF Mask does not not match comparing continue
*/
/*
BOOL FASTCALL WildCompare(char * name, char * mask)
{

	if (*mask!='|') return WildCompareProc(name,mask);

	{
		int s=1,e=1;
//        static char temp[100];  //lets made temp static local var - should be faster than dynamic
// STATIC VAR IS NOT THREAD SAFE! // by Bio
// STATIC VAR IS NOT FASTER THAN ALLOCA // by Bio
		char *temp = _alloca(strlen(mask));
		int bExcept;

		while (mask[e]!='\0')
		{
			s=e;
			while(mask[e]!='\0' && mask[e]!='|') e++;

			// exception mask
			bExcept= (*(mask+s)=='^');
			if (bExcept) s++;

			memcpy(temp,mask+s,e-s);
			temp[e-s]='\0';

			if (WildCompareProc(name,temp))
               return !bExcept;

			if (mask[e]!='\0')
				e++;
			else
				return FALSE;
		}
		return FALSE;
	}
}

//#define _qtoupper(_c) (((_c)>='a' && (_c)<='z')?((_c)-'a'+'A'):(_c))
//#define _qtoupper(_c) (((_c)>='a' && (_c)<='z')?((_c)&0x5F):(_c))

BOOL __inline WildCompareProc(char * name, char * mask)
{
	char * last='\0';
	for(;; mask++, name++)
	{
		if(*mask != '?' && *mask != *name) break;
		if(*name == '\0') return ((BOOL)!*mask);
	}
	if(*mask != '*') return FALSE;
	for(;; mask++, name++)
	{
		while(*mask == '*')
		{
			last = mask++;
			if(*mask == '\0') return ((BOOL)!*mask);	 // true 
		}
		if(*name == '\0') return ((BOOL)!*mask);	// *mask == EOS 
		if(*mask != '?' && *mask != *name) name -= (size_t)(mask - last) - 1, mask = last;
	}
}
*/
/////////////////////////////////////////////////////////////////


int ClientID(char * buf, int len)
{
  int count= len/0x10;
  for (int i=0; i<count; i++)
  {
    for (int x = 0; x < sizeof(clientDB)/(sizeof(STDCAPINFO)); ++x)
    {
       int f=capcmp(&buf[i*0x10] ,clientDB[x].caps,clientDB[x].capSize);
       if (f) return x+1;
      
    }
  }
  return 0;
}
