

local ffi = require('ffi')
local swicall = require('swi')

ffi.cdef [[
  typedef struct 
  {
    unsigned short *wsbody;
    void *ws_malloc;
    void *ws_mfree;
    int isbody_allocated;
    unsigned short maxlen;
    unsigned short unk1;
} WSHDR;

]]



 -- AllocWS
local data = swicall.call(0x0125, 'pointer', 128)

-- str_2ws
swicall.call(0x016C, 'none', data, "Hello World", 11);

-- convert to metatable
local ws = ffi.cast("WSHDR *", data)
print(string.format('Size: %d', ws.wsbody[0]) )


-- FreeWS
swicall.call(0x0129, 'none', data)

