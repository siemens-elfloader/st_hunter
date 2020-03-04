

--local classlib = require('classlib')
local ffi = require('ffi')
local _ttf = ffi.load("libdeft-0.1.so")
local rettable = {}


ffi.cdef [[
  
  typedef struct
  {
    void *p;
    void *p2;
  }ft_font;
  
  ft_font *ft_open(const char *font, uint16_t size);
  void ft_close(ft_font *f);
  
  uint16_t drawstring32s_utf8(ft_font *ftf, const char *str, int16_t x, int16_t y,
                         int16_t w, int16_t h, int type, uint8_t leter_step, int32_t max_leter_cnt, uint32_t color);
  
]]

local drawstring32s_utf8 = _ttf.drawstring32s_utf8


function rettable.open(_name, _size)
  return _ttf.ft_open(_name, _size)
end


function rettable.close(ft_font)
  _ttf.ft_close(ft_font)
end



function rettable.drawstring(ft_font, str, x, y, w, h, _type, leter_step, max_leter_cnt, color)
  drawstring32s_utf8(ft_font, str, x, y, w, h, _type, leter_step, max_leter_cnt, color)
end



return rettable
