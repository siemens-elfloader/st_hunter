#ifndef _SIEIM_H_
  #define _SIEIM_H_

#ifdef NEWSGOLD
  #define DEFAULT_DISK "4"
#else
  #define DEFAULT_DISK "4"
#endif

extern const char ELF_PATH[];

extern char * path_rd (char *subpath);
extern void * return_id (char * id);
extern void   exec(char *exename, void *fname);
extern int    check_id_type (char * id);
extern unsigned int id2uin(char *attr);

#endif
