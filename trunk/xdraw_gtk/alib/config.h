#ifndef __CONFIG_H__
#define __CONFIG_H__


//In JPEG module
//#define USE_FILE_STREAM
//use file stream, else use file specific code


//#define MEM_BUFFER
//JPEG input is MEM_BUFFER, else FILE_STREAM

//implementation specific code in io.cpp


//#define NUCLEUS

#ifdef NUCLEUS

#include <swilib.h>
//#define USE_FILE_STREAM
#define MEM_BUFFER

#else

#define USE_FILE_STREAM

#endif

#endif
