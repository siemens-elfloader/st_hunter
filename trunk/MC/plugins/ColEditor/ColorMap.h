#ifndef _COLORMAP_H
#define _COLORMAP_H

#define clMAX 36

#define MCCS_hdr mccfg_hdr

typedef struct
{
 int sig;
 int ver;
 int size;
 int offset;
} mccfg_hdr;

#define mccs_sig (*(int*)"mccs")
#define mccs_ver 2
#define mccs_size ((clMAX+1)*4)
#define mccs_offset sizeof(MCCS_hdr)
#define mccs_minsize (sizeof(MCCS_hdr)+mccs_size)

void SaveCS();
int LoadCS(char* filename);
#endif
