#include "bfc.h"

using namespace std; 
/* Bfc */
Bfc::Bfc() {
	out_buffer = new unsigned char[MAX_BFC_SIZE + 6 + 2]; 
	in_buffer = new unsigned char[MAX_BFC_SIZE + 6 + 2]; 
	error = new char[1024]; 
}

bool Bfc::writeBfc(unsigned char cmd, unsigned char type, unsigned char *data, int len, bool no_auth = false) {
	int length = 0; 
	if (this -> com == NULL)
        throw BfcException("Invalid ComPort!", BfcException::INVALID_COMPORT);
	if (len <= 0 || len > MAX_BFC_SIZE) // header (6) + crc (2) = 8
        throw BfcException("Invalid arguments!", BfcException::INVALID_ARGS);
	if (out_buffer[0] != cmd && !no_auth)
		if (!this -> auth(cmd))
			return BFC_AUTH_ERROR; 
	
	length += 6 + len; 
	this -> out_len = len; 
	memset(out_buffer, 0, MAX_BFC_SIZE + 6 + 2); 
	out_buffer[0] = cmd; // command
	out_buffer[1] = 0x01; // host id
	out_buffer[2] = (len >> 8) & 0xFF; // length
	out_buffer[3] = len & 0xFF; // length
	out_buffer[4] = type; // check type (0x04 - no check, 0x20 - CRC16, 0x30 - ???)
	out_buffer[5] = out_buffer[0] ^ out_buffer[1] ^ out_buffer[2] ^ out_buffer[3] ^ out_buffer[4]; // XOR
	memcpy(&out_buffer[6], data, len); // subdata
	
	if (type == 0x20) {
		unsigned short crc = crc16(out_buffer, 6 + len); 
		out_buffer[6 + len] = (crc >> 8) & 0xFF; 
		out_buffer[7 + len] = crc & 0xFF; 
		length += 2; 
	}
	
	return (com -> write(out_buffer, length) == length); 
}

int Bfc::readBfc() {
	int chk, type, c, i; 
	if (this -> com == NULL)
        throw BfcException("Invalid ComPort!", BfcException::INVALID_COMPORT);
	memset(in_buffer, 0, MAX_BFC_SIZE + 6 + 2); 
	
	if (com -> read(in_buffer, 6) != 6)
		return BFC_IO_ERROR; 
	
	do {
		if (in_buffer[0] == out_buffer[1] && in_buffer[1] == out_buffer[0]) {
			in_len = (in_buffer[2] << 8) + in_buffer[3]; 
			chk = in_buffer[0] ^ in_buffer[1] ^ in_buffer[2] ^ in_buffer[3] ^ in_buffer[4]; // XOR
			type = in_buffer[4]; 
			
			if (chk == in_buffer[5]) {
				if (com -> read((void *)((unsigned char *)in_buffer + 6), in_len) == in_len) {
					if (type == 0x20) {
						if (com -> read((void *)((unsigned char *)in_buffer + 6 + in_len), 2) == 2) {
							unsigned short crc = crc16(in_buffer, 6 + in_len); 
							if (in_buffer[6 + in_len] == ((crc >> 8) & 0xFF) && in_buffer[7 + in_len] == (crc & 0xFF)) {
								return BFC_OK; 
							} else
								return BFC_CRC_ERROR; 
						} else
							return BFC_IO_ERROR; 
					} else {
						return BFC_OK; 
					}
				} else
					return BFC_IO_ERROR; 
			} else
				return BFC_XOR_ERROR; 
		}
		for (i = 1; i < 6; i++)
			in_buffer[i - 1] = in_buffer[i]; 
		if ((c = com -> readChar()) == -1)
			return BFC_IO_ERROR; 
		in_buffer[i] = c; 
	} while(true); 
	return BFC_IO_ERROR; 
}

int Bfc::readMem(unsigned int address, void *buff, unsigned int length) {
	unsigned int pos = 0, block_id = 1, len = 0; 
	
	BFC_ReadMem mem; 
	mem.sub = 0x01; 
	mem.address = address; 
	mem.length = length; 
	
	if (!this -> writeBfc(0x06, 0x20, mem.data, sizeof(mem)))
		return -1; 
	if (this -> readBfc() != BFC_OK)
		return -1; 
	if ((unsigned short)in_buffer[6] != 0x0001)
		return -1; 
	
	while (pos < length) {
		if (this -> readBfc() != BFC_OK)
			return pos; 
		if (in_len != (int)length) {
			if ((in_buffer[6] & 0x80) != 0) {
				if ((length != pos + in_len - 1) || (in_buffer[6] != (block_id | 0x80)))
					return -1; 
			} else if (in_buffer[6] != block_id)
				return -1; 
			len = in_len - 1; 
			if (length < pos + len - 1)
				len = length - pos; 
			memcpy((void *)((unsigned char *)buff + pos), &in_buffer[7], len); 
			pos += len; 
			block_id++; 
		} else {
			len = in_len - 1; 
			if (length < pos + len - 1)
				len = length - pos; 
			memcpy((void *)((unsigned char *)buff + pos), &in_buffer[7], len); 
			return in_len; 
		}
	}
	return pos; 
}

bool Bfc::redirectKeypad() {
	unsigned char cmd[1] = {0x01}; 
	if (!this -> writeBfc(0x09, 0x20, cmd, 1))
		return false; 
	if (this -> readBfc() != BFC_OK)
		return false; 
	return true; 
}

bool Bfc::restoreKeypad() {
	unsigned char cmd[1] = {0x02}; 
	if (!this -> writeBfc(0x09, 0x20, cmd, 1))
		return false; 
	if (this -> readBfc() != BFC_OK)
		return false; 
	return true; 
}

bool Bfc::pressKey(unsigned char key_code) {
	unsigned char cmd[2] = {0x03, key_code}; 
	if (!this -> writeBfc(0x09, 0x20, cmd, 2))
		return false; 
	if (this -> readBfc() != BFC_OK)
		return false; 
	return true; 
}

bool Bfc::sendAT(const char *cmd, int len, unsigned char *answer) {
	if (!this -> writeBfc(0x17, 0x20, (unsigned char *)cmd, len))
		return false; 
	if (this -> readBfc() != BFC_OK)
		return false; 
	if (answer != NULL)
		memcpy(answer, &in_buffer[6], in_len); 
	return true; 
}

bool Bfc::getDisplayInfo(BFC_DisplayInfo *info, unsigned char display_id) {
	unsigned char cmd[2] = {0x07, display_id}; 
	if (!this -> writeBfc(0x0A, 0x20, cmd, 2))
		return false; 
	if (this -> readBfc() != BFC_OK)
		return false; 
	memcpy(info, &in_buffer[6], sizeof(BFC_DisplayInfo)); 
	return true; 
}

bool Bfc::getDisplayBufferInfo(BFC_DisplayBufferInfo *info, unsigned char client_id) {
	unsigned char cmd[2] = {0x09, client_id}; 
	if (!this -> writeBfc(0x0A, 0x20, cmd, 2))
		return false; 
	if (this -> readBfc() != BFC_OK)
		return false; 
	memcpy(info, &in_buffer[6], sizeof(BFC_DisplayBufferInfo)); 
	return true; 
}

bool Bfc::auth(unsigned int auth) {
	unsigned char cmd[2] = {0x80, 0x11}; 
	if (!this -> writeBfc(auth, 0x04, cmd, 2, true))
		return false; 
	if (this -> readBfc() != BFC_OK) {
		return false; 
	}
	return true; 
}

Bfc *Bfc::setComPort(ComPort *com) {
	if (com == NULL)
        throw BfcException("Invalid ComPort!", BfcException::INVALID_COMPORT);
	this -> com = com; 
	return this; 
}

Bfc::~Bfc() {
	delete[] out_buffer; 
	delete[] in_buffer; 
	delete[] error; 
}
