#ifndef _COLORMAP_H
#define _COLORMAP_H

//Colors cash
#define clBD 0
#define clBG 1
#define clListBD 2
#define clListBG 3
#define clCurBD 4
#define clCurBG 5

#define clInfoBD 6
#define clInfoBG 7

#define clMenuBD 8
#define clMenuBG 9

#define clMenuCurBD 10
#define clMenuCurBG 11

#define clScreenBD 12
#define clScreenBG 13

#define clProgrBD 14
#define clProgrBG 15
#define clCurProgrBG 16

#define clScBar 17
#define clScBarBG 18

#define clCheckBD 19
#define clCheckBG 20
#define clCheckEnter 21
#define clCheckSel 22

#define clFileNormal 23
#define clSelFileNormal 24
#define clFileHidden 25
#define clSelFileHidden 26
#define clFileReadOnly 27
#define clSelFileReadOnly 28
#define clInfoTxt 29

#define clHeadTxt 30
#define clHeadTxtBD 31

#define clSoft 32
#define clSoftBD 33

#define clMenuTXTNormal 34
#define clMenuTXTSelect 35

#define clInfo 36
////////////////////////
#define clMAX clInfo

#define MCCS_hdr mccfg_hdr

#define mccs_sig (*(int*)"mccs")
#define mccs_ver 2
#define mccs_size ((clMAX+1)*4)
#define mccs_offset sizeof(MCCS_hdr)
#define mccs_minsize (sizeof(MCCS_hdr)+mccs_size)

extern const char mccs_deffile[];
extern const char mccs_ext[];

void InitCS();
void SaveCS(char* filename);
int LoadCS(char* filename);

#endif
