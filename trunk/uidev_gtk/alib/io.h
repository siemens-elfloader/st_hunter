#include <alib/config.h>

#ifdef USE_FILE_STREAM
#include <stdio.h>
#endif

int GetFileSize (char *fname);

int FileReadToBuffer (char *fname, char *buffer, int size);
