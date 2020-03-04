

-- [[ 

  Демонстариця новой возможности ffi - кальбаки. 
  В примере показана загрузка либы с функцией таймера
  и создание кальбака с помощью каста
  
--]]

local ffi = require('ffi')
local ffitest = ffi.load(sys.libDir()..'core\\fficore.so')
local timer

sys.keep_alive()

ffi.cdef [[
  
  typedef void (__stdcall *CallBackProc)();
  
  void stop_timer(unsigned int tmr);
  unsigned int start_timer(CallBackProc call, int ticks);
  
  
]]


local cb = ffi.cast("CallBackProc", function(hwnd, l)
  print('Exit')
  sys.exit()
end)


timer = ffitest.start_timer(cb, 12);











