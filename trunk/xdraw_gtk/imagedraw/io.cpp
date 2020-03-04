#include <alib/io.h>


#ifdef USE_FILE_STREAM
#include <stdio.h>
#endif

#ifdef USE_FILE_STREAM
int GetFileSize (char *fname){
	FILE *file=fopen (fname, "rb");
	if (!file) return -1;

	fseek (file, 0, SEEK_END);
	int size=ftell (file);
	fclose (file);

	return size;
}
#else
int GetFileSize (char *fname){
  unsigned int err;
  FSTATS fs;
  GetFileStats (fname, &fs, &err);

  return fs.size;
}
#endif


int FileReadToBuffer (char *fname,  char *buffer, int size){

#ifdef USE_FILE_STREAM
	 FILE *file=fopen (fname, "rb");

	 fread (buffer, size, 1, file);
	 fclose (file);
#else
	  unsigned int err;
	  int f=_open (fname, A_ReadOnly+A_BIN, P_READ, &err);

	  _read (f, buffer, size, &err);
	  _close (f, &err);
#endif

	  return 0;
}






