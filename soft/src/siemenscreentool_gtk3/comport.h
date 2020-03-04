#ifndef __COMPORT_H
#define __COMPORT_H

#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/time.h>

/* ComPortTimeout */
class ComPortTimeout {
	private:
		struct timeval *start_time; 
		struct timeval *end_time; 
	public:
		ComPortTimeout(); 
		void start(); 
		unsigned long getTimeDelta(); 
		~ComPortTimeout(); 
}; 

/* ComPort */
class ComPort {
	private:
		struct ComPortOptions {
			int            fd; 
			char          *device; 
			unsigned int   speed; 
			unsigned int   multiplier_timeout; 
			unsigned int   timeout; 
		}; 
		char *error; 
		struct termios port_config; 
		ComPortOptions *com; 
		ComPortTimeout *timeout; 
	public:
		const static char *DEFAULT_DEVICE; 
		const static int DEFAULT_SPEED; 
		const static int DEFAULT_TIMEOUT; 
		
		ComPort(); 
		
		ComPort *open(); 
		ComPort *close(); 
		ComPort *setSpeed(int speed); 
		ComPort *setDevice(const char *device); 
		ComPort *setTimeout(unsigned int timeout); 
		ComPort *setMultiplierTimeout(unsigned int timeout); 
		
		int read(void *buf, int len); 
		int write(const void *buf, int len); 
		int readChar(); 
		int writeChar(unsigned char c); 
		
		inline int getSpeed() {
			return com -> speed; 
		}
		
		inline char *getDevice() {
			return com -> device; 
		}
		
		inline int getTimeout() {
			return com -> timeout; 
		}
		
		inline int getMultiplierTimeout() {
			return com -> multiplier_timeout; 
		}
		~ComPort(); 
}; 

/* ComPortException */
class ComPortException {
	private:
		char err_msg[1024]; 
		int  err_code; 
	public:
		const static int CANT_OPEN_DEVICE  = 0x0000001; 
		const static int DEVICE_NOT_TTY    = 0x0000002; 
		const static int CANT_GET_COM_ATTR = 0x0000003; 
		const static int CANT_SET_COM_ATTR = 0x0000004; 
		const static int INVALID_SPEED     = 0x0000005; 
		const static int INVALID_TIMEOUT   = 0x0000006; 
		const static int INVALID_DEVICE    = 0x0000007; 
		const static int CANT_SET_SPEED    = 0x0000008; 
		const static int CANT_SET_TIMEOUT  = 0x0000009; 
		
		ComPortException(); 
		ComPortException(const char *message, int code); 
		
		inline int getCode() {
			return err_code; 
		}
		
		inline const char *getMessage() {
			return err_msg; 
		}
		
		~ComPortException(); 
}; 

#endif
