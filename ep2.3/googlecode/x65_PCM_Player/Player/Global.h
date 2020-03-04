
#ifndef _global
#define _global

void SavePLTime();
extern void( * GetSound ) ( unsigned short *, int );
extern int( * GetPlayingTime ) ();
extern unsigned short channels;;
extern int samplerate;
extern char AudioActive;
extern char Paused;
extern signed char Playing;

#endif
