#include <swilib.h>
#include <alib\io.h>


#ifndef NUCLEUS_FILE
#include <stdio.h>
#endif

#ifndef NUCLEUS_FILE
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

#ifndef NUCLEUS_FILE
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






