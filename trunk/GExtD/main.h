/*
 * File:   main.h
 * Author: Tonich
 *
 * Created on 27 РћРєС‚СЏР±СЂСЊ 2011 Рі., 3:32
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif
//Ерроры
#define ERR_OPEN_CFG    0
#define ERR_FILE_LEN    1
#define ERR_ALLOC_MEM   2
#define ERR_FILE_READ   3
#define ERR_DATA_COUNT  4

//Мессаги
#define	EMSG_RECONFIGURE	0
#define	EMSG_RECONFWPARM	1



typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;

typedef struct
{
  int err;
  char *param;
}IPC_CHECK;

CSM_RAM *under_idle;



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

