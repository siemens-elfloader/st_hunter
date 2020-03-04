/*
Structure EEPROM
(r) feanor for a good explanation

EELITE
--------------------------------------------------------------------------------
0xA0FE0000:
<begin>
EELITE's Blocks

EIT: (Upwards), descriptors of 16 bytes (4 / 4 / 4 / 4)

Status / Block number / length / offset from the beginning EELITE

Status: 0OFFFFFF - removed S0FFFFFF - working
length: size = length - 1
Displacement: Destination address = 0xA0FE0000 + offset + 1

<end>

0xA0FFFFF0
--------------------------------------------------------------------------------


EEFULL (1)
--------------------------------------------------------------------------------
0xA0220000:
<begin>
EEFULL's Blocks

EIT: (bottom to top), descriptors of 16 bytes (4 / 4 / 4 / 4)

Status / block number / length / offset from the beginning EEFULL (1)

Status: 0OFFFFFF - removed S0FFFFFF - working
block number: the real number of the block = block number + 5000
length: size = length - 1
Displacement: Destination address = 0xA0220000 + offset + 1

<end>

0xA023FFF0:
--------------------------------------------------------------------------------

EEFULL (2)
--------------------------------------------------------------------------------
0xA0240000:
<begin>
EEFULL's Blocks

EIT: (bottom to top), descriptors of 16 bytes (4 / 4 / 4 / 4)

Status / block number / length / offset from the beginning EEFULL (2)

Status: 0OFFFFFF - removed S0FFFFFF - working
block number: the real number of the block = block number + 5000
length: size = length - 1
Displacement: Destination address = 0xA0240000 + offset + 1

<end>

0xA025FFF0:
--------------------------------------------------------------------------------
*/

#include "config\config.h"  


__thumb unsigned long FindEEFULLBlock() //Searching for EEFULL "handles" through the EIT
{
 unsigned long result = 0;
 
  for (unsigned long *element = (unsigned long*)0xA023FFF0; element[1]!=0xFFFFFFFF; element-=0x04)
  {
   if (element[0]==0xFFFFFFC0 && element[1]==EEPROMBLOCKNUM-5000)
    {
     result = element[3]+0xA0220000+1;
     goto FIND;
    }
  }
  
  for (unsigned long *element = (unsigned long*)0xA025FFF0; element[1]!=0xFFFFFFFF; element-=0x04)
  {
   if (element[0]==0xFFFFFFC0 && element[1]==EEPROMBLOCKNUM-5000)
    {
     result = element[3]+0xA0240000+1;
     goto FIND;
    }
  }
 
 
FIND:
 
  return  result;
}



__thumb void SeekBlock()
{
  *(( (unsigned long *)RAMFORPATCHADDR)+0x03) = FindEEFULLBlock() + EEPROMOFFSET;

}


