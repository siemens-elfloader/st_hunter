

local ffi = require('ffi')
local swi = require('swi')
local ffcore = ffi.load(sys.libDir()..'core\\fficore.so')
require('classlib')


ffi.cdef[[
  
typedef struct
{
  char* body;
  unsigned int bin_size;
} Elf32_Exec;

typedef struct
{
  void *lib;
  void *next;
  void *prev;
} Global_Queue;


typedef struct
{
  char soname[64];
  Elf32_Exec * ex;
  int users_cnt;
  void *glob_queue;
} Elf32_Lib;

]]


function getTopShared()
  return ffi.cast('Global_Queue *', swi.call(0x2F9, 'userdata'))
end

function ShowMSG(str)
  return swi.call(0x148, 'none', 1, str)
end

function dlclean_cache()
  return swi.call(0x2F8, 'number')
end

-- class базы
class.SharedBase()
SharedBase.soname = ""
SharedBase.binsz = 0
SharedBase.ladr = 0
SharedBase.users_cnt = 0

-- массив информации либ
local SharedInfo = {}
local SharedInfo_cnt = 0
local script_dir = sys.pDir()


-- получаем инфу
function rescanShareds()
  
  SharedInfo = {}
  SharedInfo_cnt = 0
  local top = getTopShared()
  local nexttop = top
  while ffi.cast('int', nexttop) ~=0 do
  
    elib = ffi.cast('Elf32_Lib *', nexttop.lib)
    exec = ffi.cast('Elf32_Exec *', elib.ex)
  
    SharedInfo[SharedInfo_cnt] = SharedBase()
  
    SharedInfo[SharedInfo_cnt].soname = ffi.string( elib.soname )
    SharedInfo[SharedInfo_cnt].binsz = exec.bin_size
    SharedInfo[SharedInfo_cnt].ladr = tonumber(ffi.cast('int', exec.body))
    SharedInfo[SharedInfo_cnt].users_cnt = elib.users_cnt
    
    print(' =========== '..SharedInfo[SharedInfo_cnt].soname..' ========== ')
    print('Uses: '..SharedInfo[SharedInfo_cnt].users_cnt)
    

    SharedInfo_cnt = SharedInfo_cnt+1
    nexttop = ffi.cast('Global_Queue *', nexttop.prev)
  end

end

rescanShareds()


