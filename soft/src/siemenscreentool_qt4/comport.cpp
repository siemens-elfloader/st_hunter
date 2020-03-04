#include "comport.h"

using namespace std; 
/* ComPort */
const char *ComPort::DEFAULT_DEVICE = "/dev/ttyS0"; 
const int ComPort::DEFAULT_SPEED    = B115200; 
const int ComPort::DEFAULT_TIMEOUT  = 0; 

ComPort::ComPort() {
    com.fd = -1;
    com.speed = 0;
    com.timeout = 0;
    setDevice((char *)DEFAULT_DEVICE).setSpeed(DEFAULT_SPEED).setTimeout(DEFAULT_TIMEOUT);
}

ComPort & ComPort::open() {
    if (com.fd != -1)
		this -> close(); 
	
    com.fd = ::open(getDevice(), O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
    if (com.fd < 0) {
        snprintf(error, 1024, "Can't open device '%s'!", getDevice());
        throw ComPortException(error, ComPortException::CANT_OPEN_DEVICE);
	}
	
    if (!isatty(com.fd)) {
        snprintf(error, 1024, "This device '%s' not is TTY!", getDevice());
        throw ComPortException(error, ComPortException::DEVICE_NOT_TTY);
	}
	
    if (tcgetattr(com.fd, &port_config) < 0) {
        snprintf(error, 1024, "Can't get configuration for port '%s'!", getDevice());
        throw ComPortException(error, ComPortException::CANT_GET_COM_ATTR);
	}
	
    port_config.c_cflag = com.speed | (PARODD | HUPCL | CS8 | CLOCAL | CREAD);
	port_config.c_iflag = IGNPAR | IGNBRK; 
	port_config.c_oflag = NLDLY; 
	port_config.c_lflag = 0; 
	port_config.c_cc[VTIME] = 0; 
	port_config.c_cc[VMIN] = 0; 
    tcflush(com.fd, TCIOFLUSH);
	
    if (tcsetattr(com.fd, TCSANOW, &port_config) < 0) {
        snprintf(error, 1024, "Can't get configuration for port '%s'!", getDevice());
        throw ComPortException(error, ComPortException::CANT_SET_COM_ATTR);
	}
	
    return *this;
}

ComPort & ComPort::close() {
    if (com.fd != -1) {
        tcsendbreak(com.fd, 0);
        ::close(com.fd);
	}
    com.fd = -1;
    return *this;
}

ComPort & ComPort::setSpeed(int speed) {
	if (speed < B0 || speed > B4000000)
        throw ComPortException("Speed should be in the range B0 .. B4000000!", ComPortException::INVALID_SPEED);
    if (com.fd != -1) {
        tcgetattr(com.fd, &port_config);
		port_config.c_cflag = speed | (PARODD | HUPCL | CS8 | CLOCAL | CREAD); 
        if (tcsetattr(com.fd, TCSANOW, &port_config) < 0)
            throw ComPortException("Can't set speed!", ComPortException::CANT_SET_SPEED);
	}
    com.speed = speed;
    return *this;
}

ComPort & ComPort::setDevice(const char *device) {
	if (strlen(device) < 1 || strlen(device) > 64)
        throw ComPortException("Length of device should be in the range 1 .. 64 chars!", ComPortException::INVALID_DEVICE);
    com.device = device;
    if (com.fd != -1)
		this -> close(); 
    return *this;
}

ComPort & ComPort::setTimeout(unsigned int timeout) {
    if (timeout > 60000)
        throw ComPortException("Timeout should be in the range 0 .. 60 sec!", ComPortException::INVALID_TIMEOUT);
    com.timeout = timeout;
    return *this;
}

ComPort & ComPort::setMultiplierTimeout(unsigned int timeout) {
    if (timeout > 60000)
        throw ComPortException("Timeout should be in the range 0 .. 60 sec!", ComPortException::INVALID_TIMEOUT);
    com.multiplier_timeout = timeout;
    return *this;
}

int ComPort::read(void *buf, int len) {
	int pos = 0, res = 0; 
    unsigned int timeout = com.timeout + com.multiplier_timeout * len;
	
    if (com.fd != -1) {
		if (timeout == 0) {
            return com.fd != -1 ? ::read(com.fd, buf, len) : -1;
		} else {
            this -> timeout.start();
            while (this -> timeout.getTimeDelta() <= timeout) {
                res = ::read(com.fd, (void *)((unsigned char *)buf + pos), len - pos);
				pos = res > 0 ? pos + res : pos; 
				if (pos == len)
					break; 
				
			}
		}
		return pos; 
	}
	return -1; 
}

int ComPort::readChar() {
	char buff = 0; 
    return com.fd != -1 && this -> read(&buff, 1) == 1 ? buff & 0xFF : -1;
}

int ComPort::write(const void *buf, int len) {
    return com.fd != -1 ? ::write(com.fd, buf, len) : -1;
}

int ComPort::writeChar(unsigned char c) {
    return com.fd != -1 ? ::write(com.fd, &c, 1) : -1;
}

ComPort::~ComPort() {
    if (com.fd > -1)
        this->close();
}

/* ComPortTimeout */
ComPortTimeout::ComPortTimeout() {

}

void ComPortTimeout::start() {
    gettimeofday(&start_time, NULL);
}

unsigned long ComPortTimeout::getTimeDelta() {
	int sec, usec; 
    gettimeofday(&end_time, NULL);
    sec = end_time.tv_sec - start_time.tv_sec;
    usec = end_time.tv_usec - start_time.tv_usec;
	if (usec < 0) {
        usec = 1000000 - start_time.tv_usec + end_time.tv_usec;
		sec--; 
	}
	return sec * 1000 + usec / 1000; 
}

ComPortTimeout::~ComPortTimeout() {

}
