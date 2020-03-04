//sdk_work.h - Библиотека ф-ий для работы с фаилами карт sdk программы Sudoku Generator

#ifdef  __EXTWORK__
#define __SDKWORK__

typedef struct {             //Структура *.sdk фаила программы Sudoku Generator 
	  char  level;       //Уровень сложности: 0x00 - Simple, 0x01 - Average, 0x02 - Hard
 unsigned short scell[9][9]; //Карта ячеек, которые выводится на экран
 unsigned short rcell[9][9]; //Карта верных ячеек
}SDKHDR;

#define SDK_FILE_SIZE 325    //Размер фаила SDK (в байтах), константа

//Инфо (!)
//SDKHDR->scell[][]:
//0x01FF - скрытая ячейка
//Известные ячейки:
//0x0001 - 1, 0x0002 - 2, 0x0004 - 3, 0x0008 - 4, 0x0010 - 5, 0x0020 - 6, 0x0040 - 7, 0x0080 - 8, 0x0100 - 9;
//Ячейки, которые вводятся пользователем или *решаются самой программой:
//0x8001 - 1, 0x8002 - 2, 0x8004 - 3, 0x8008 - 4, 0x8010 - 5, 0x8020 - 6, 0x8040 - 7, 0x8080 - 8, 0x8100 - 9;
//*решаются - программа не решает, а заполняет их из заранее подготовленной карты.
//SDKHDR->rcell[][]:
//0x0001 - 1, 0x0002 - 2, 0x0004 - 3, 0x0008 - 4, 0x0010 - 5, 0x0020 - 6, 0x0040 - 7, 0x0080 - 8, 0x0100 - 9;
//Т.е. известные ячейки.

//Функции ----------------------------------------------------------------------
SUDOKUMAP *CreateSUDOKUMAPbySDKFILE(char *path);
//------------------------------------------------------------------------------



char sdkcell_2_sdmcell(unsigned short sdk_cell) //Конвертирует ячейку sdk в ячейку SUDOKUMAP 
{
 char res = 0xFF;
 
 if (sdk_cell == 0x8001) res = 0x01;
 if (sdk_cell == 0x8002) res = 0x02;
 if (sdk_cell == 0x8004) res = 0x03;
 if (sdk_cell == 0x8008) res = 0x04;
 if (sdk_cell == 0x8010) res = 0x05;
 if (sdk_cell == 0x8020) res = 0x06;
 if (sdk_cell == 0x8040) res = 0x07;
 if (sdk_cell == 0x8080) res = 0x08;
 if (sdk_cell == 0x8100) res = 0x09;
 
 if (sdk_cell == 0x0001) res = 0x21;
 if (sdk_cell == 0x0002) res = 0x22;
 if (sdk_cell == 0x0004) res = 0x23;
 if (sdk_cell == 0x0008) res = 0x24;
 if (sdk_cell == 0x0010) res = 0x25;
 if (sdk_cell == 0x0020) res = 0x26;
 if (sdk_cell == 0x0040) res = 0x27;
 if (sdk_cell == 0x0080) res = 0x28;
 if (sdk_cell == 0x0100) res = 0x29;
 
 if (sdk_cell == 0x01FF) res = 0x00;
 
 return res;
}

SUDOKUMAP *CreateSUDOKUMAPbySDKFILE(char *path) // Создаёт SUDOKUMAP из фаила sdk программы Sudoku Generator 
{
 FSTATS stat;
 SUDOKUMAP *sdm=malloc(sizeof(SUDOKUMAP));
 SDKHDR *sdkh=malloc(sizeof(SDKHDR));
 if (GetFileStats(path, &stat, 0)!=-1 && stat.size)
  {
    int sdk=fopen(path,A_ReadOnly+A_BIN,P_READ,0);
    
    fread(sdk,(void *)&sdkh->level, 1,0);
    fread(sdk,(void *)&sdkh->scell, 9*9*2,0);
    fread(sdk,(void *)&sdkh->rcell, 9*9*2,0);
    
    fclose(sdk,0);

    sdm->level = 0x3F;
    if (sdkh->level == 0x00) sdm->level = 'L';
    if (sdkh->level == 0x01) sdm->level = 'M';
    if (sdkh->level == 0x02) sdm->level = 'H';
     
    sdm->name[0] = 0x00;
    sdm->preview = 0x00;
    sdm->x       = 0x04;
    sdm->y       = 0x04;
    sdm->sec     = 0x00;
    sdm->error   = 0x00;
    sdm->warn    = 0x02;
    
    if (stat.size == SDK_FILE_SIZE && sdkh->level < 0x03)
     {     
      char temp[9][9];
      for (char i=0;i<9;i++) for (char j=0;j<9;j++)
       {
        if (sdkcell_2_sdmcell(sdkh->scell[i][j]) >  0x20) temp[i][j] = 0x01; else temp[i][j] = 0x00;
        if (sdkcell_2_sdmcell(sdkh->scell[i][j]) == 0xFF) sdm->error = 0x04;
       }
    
      for (char i=0;i<9;i++) for (char j=0;j<9;j++)
       {
        if (temp[i][j] == 0x01) sdm->gcell[i][j] = sdkcell_2_sdmcell(sdkh->rcell[i][j]);
        if (temp[i][j] == 0x00) sdm->gcell[i][j] = sdkcell_2_sdmcell(sdkh->rcell[i][j])-0x20;
       }
    
      sdm = gcell_2_rcell(sdm);
      if (sdm->error != 0x04) sdm->error = CheckSUDOKUMAP(sdm);
    } else sdm->error=0x02;
  } else sdm->error=0x01;
 mfree(sdkh);
 
 return sdm;
}

#endif
