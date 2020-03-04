#ifndef __BFC_H
#define __BFC_H
#define MAX_BFC_SIZE 0xFFFF

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "crc16.h"
#include "comport.h"

#pragma pack(push, 1)
typedef union {
	struct {
		unsigned char  subcmd; 
		unsigned short width; 
		unsigned short height; 
		unsigned char  client_id; 
	}; 
	unsigned char data[6]; 
} BFC_DisplayInfo; 

typedef union {
	struct {
		unsigned char sub; 
		unsigned int  address; 
		unsigned int  length; 
	}; 
	unsigned char data[9]; 
} BFC_ReadMem; 

typedef union {
	struct {
		unsigned char  subcmd; 
		unsigned char  client_id; 
		unsigned short width; 
		unsigned short height; 
		unsigned short offet_x; 
		unsigned short offet_y; 
		unsigned int   address; 
		unsigned char  type; 
	}; 
	unsigned char data[15]; 
} BFC_DisplayBufferInfo; 
#pragma pack(pop)

/* Swup */
class Bfc {
	private:
		char *error; 
		ComPort *com; 
		int out_len; 
		unsigned char *out_buffer; 
		int in_len; 
		unsigned char *in_buffer; 
	public:
		const static int BFC_OK         = 0x0000000; 
		const static int BFC_CRC_ERROR  = 0x0000001; 
		const static int BFC_XOR_ERROR  = 0x0000002; 
		const static int BFC_IO_ERROR   = 0x0000003; 
		const static int BFC_AUTH_ERROR = 0x0000004; 
		
		Bfc(); 
		Bfc *setComPort(ComPort *com); 
		
		bool auth(unsigned int cmd); 
		bool writeBfc(unsigned char cmd, unsigned char type, unsigned char *data, int len, bool no_auth); 
		bool sendAT(const char *cmd, int len, unsigned char *answer = NULL); 
		bool getDisplayInfo(BFC_DisplayInfo *info, unsigned int display_id = 0x01); 
		bool getDisplayBufferInfo(BFC_DisplayBufferInfo *info, unsigned int client_id = 0x01); 
		bool pressKey(unsigned int key_code); 
		bool redirectKeypad(); 
		bool restoreKeypad(); 
		
		int readBfc(unsigned char *buff); 
		int readMem(unsigned long address, void *buff, unsigned long length); 
		
		Bfc *isNewSGold(bool flag); 
		bool isNewSGold(); 
		~Bfc(); 
}; 

/* SwupException */
class BfcException {
	private:
		char err_msg[1024]; 
		int err_code; 
	public:
		const static int INVALID_COMPORT = 0x0000001; 
		const static int INVALID_ARGS    = 0x0000002; 
		
		BfcException(); 
		BfcException(const char *message, int code); 
		
		inline int getCode() {
			return err_code; 
		}
		
		inline const char *getMessage() {
			return err_msg; 
		}
		
		~BfcException(); 
}; 

#endif

