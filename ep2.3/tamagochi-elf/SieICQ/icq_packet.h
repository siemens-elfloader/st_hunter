#ifndef _PACKET_H
#define _PACKET_H

#include "include.h"

typedef struct {
	int size;
	int offset;
	char *data;
} Packet;

/* prototypes */
Packet* PackNew();
void PackFree(Packet *packet);

void PackAdd(Packet *packet, char* data, int size);
void PackAdd8(Packet *packet, char data);
void PackAdd16(Packet *packet, int data);
void PackAdd16LE(Packet *packet, int data);
void PackAdd32(Packet *packet,long data);
void PackAdd32LE(Packet *packet,long data);
void PackAddStr(Packet *packet, char *str);

void PackAddPack(Packet *packet,Packet *packet2);

void PackAddTLV(Packet *packet,int type,int size);
void PackAddTLV16(Packet *packet, int type, int data);
void PackAddTLV32(Packet *packet, int type, long data);
void PackAddTLVStr(Packet *packet, int type, char *str);
void PackAddTLVRAW(Packet *packet, int type, int size, char* data);

int PackGet(Packet *packet, void* data, int datalen);
void PackGet8(Packet *packet, char* num);
void PackGet16(Packet *packet, short int* num);
void PackGet16LE(Packet *packet, short int* num);
void PackGet32(Packet *packet,int* num);
void PackGet32LE(Packet *packet,int* num);
void PackGetStr(Packet *packet, char* str, int len);
void PackGetTLV(Packet *packet,short int* type,short int* size);


#endif /* _PACKET_H */

 
 
