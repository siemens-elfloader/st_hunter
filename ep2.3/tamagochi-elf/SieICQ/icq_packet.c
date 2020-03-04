#include "include.h"
#include "icq_packet.h"


Packet* PackNew() {
        Packet *packet = new Packet;
        packet->data = new char;
	packet->size=0;
	packet->offset=0;
        return packet;
}

void PackFree(Packet *packet) {
        delete (packet->data);
	delete (packet);
}
extern void _WriteLog(char *buf);

void PackAdd(Packet *packet, char* data, int size) {
	/* Make the space for the new data */
       // _WriteLog("realloc");
        packet->data = (char*)realloc(packet->data, packet->size + size);
	/* Copy the new data in */
       // _WriteLog("data");
        memcpy((char*)(packet->data + packet->size), data, size);
	/* Recalculate the size */
       // _WriteLog("size");
	packet->size += size;
       
}

void PackAdd16(Packet *packet, int data) {
    int tmp=htons(data);
    PackAdd(packet,(char*)&tmp,2);
}

void PackAdd32(Packet *packet, long data) {
    long tmp = htonl(data); 
    PackAdd(packet,(char*)&tmp,4);
}

void PackAddStr(Packet *packet, char *str) {
    PackAdd(packet,str,strlen(str));
    //_WriteLog(str);
}

void PackAddTLV(Packet *packet,int type,int size){
    PackAdd16(packet,type);
    PackAdd16(packet,size);
}

void PackAddTLV16(Packet *packet, int type, int data)
{
    PackAddTLV(packet,type,2);
    PackAdd16(packet,data);
}

void PackAddTLV32(Packet *packet, int type, long data)
{
    PackAddTLV(packet,type,4);
    PackAdd32(packet,data);
}


void PackAddTLVStr(Packet *packet, int type, char *str)
{
    PackAddTLV(packet,type,strlen(str));
    PackAddStr(packet,str);
}

void PackAddTLVRAW(Packet *packet, int type, int size, char* data)
{
    PackAddTLV(packet,type,size);
    PackAdd(packet,data,size);
}

///////////////////////////////////
int PackGet(Packet *packet, void* data, int datalen) {
	if (!packet) return 0;
	if (packet->size - packet->offset < datalen) return 0;
        void *pack=packet->data + packet->offset;
	memcpy(data, pack, datalen);
        packet->offset += datalen;
	return datalen;
}

void PackGetType(Packet *packet,void* val, int size)
{
      PackGet(packet,val,size);
}

void PackGet16(Packet *packet, short int* num){
    short int value=0;
    
    PackGetType(packet,(short int*)num,2);
    
//   char *tmp_msg=new char[64];
//    sprintf(tmp_msg, percent_d, *num);
//    _WriteLog(tmp_msg);
    
    //num = htons(value);
    value = htons(*num);
    *num=value;
    
//    sprintf(tmp_msg, percent_d, *num);
//   _WriteLog(tmp_msg);
//    delete tmp_msg;
}

void PackGet32(Packet *packet,int* num){
    PackGetType(packet,(int*)num,4);
    int value=htonl(*num);
    *num = value;
}


void PackGetTLV(Packet *packet,short int* type,short int* size){
    PackGet16(packet,type);
    PackGet16(packet,size);
}


 
 
