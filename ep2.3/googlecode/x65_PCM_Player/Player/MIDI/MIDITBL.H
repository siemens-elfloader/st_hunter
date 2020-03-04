/*
 * Internal header file: tables needed in module playing
 */

extern signed char retrigaddtable[];
extern signed char retrigmultable[];
extern signed char vibratotable[4][256];
extern unsigned linearfreqtable[];
extern unsigned short s3mperiodtable[];
extern unsigned short periodtable[16][12];
extern unsigned short s3mfinetunetable[];
extern unsigned char vibratotypetable[];
extern unsigned amigafreqtable[];

int judas_locktables(void);
