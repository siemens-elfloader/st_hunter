/*
 * File:   IPC_mes.h
 * Author: Tonich
 *
 * Created on 27 Октябрь 2011 г., 3:35
 */

#ifndef IPC_MES_H
#define	IPC_MES_H

#ifdef	__cplusplus
extern "C" {
#endif

#define	IPC_RECONFIG		3
#define	IPC_CONFASK		4
#define	IPC_GETEXT			5
#define	IPC_SETEXT			6



IPC_REQ IPCsend;

void SendIpc(int submess);                      //Просто инфа
void SendIpcData(int submess, void *data);      //Инфа с данными



#ifdef	__cplusplus
}
#endif

#endif	/* IPC_MES_H */

