


local ffi = require('ffi')
local _draw = ffi.load(sys.libDir()..'core\\draw.so')
local rettable = {}


ffi.cdef [[
  void ldrawLine(int x, int y, int x2, int y2, int type, const char *pen);
  void ldrawRectangle(int, int, int, int, int, const char *, const char *);
  void ldrawArc(int x1, int y1, int x2, int y2, int a1, int a2, int flag, char *pen, char *brush);
  void ldrawRoundedFrame(int x1, int y1, int x2, int y2, int x_round, int y_round, int flags, const char *pen, const char *brush);
  void ldrawImg(unsigned int x, unsigned int y, void *picture);
  void ldrawCanvas(void *data, int x1, int y1, int x2, int y2, int flag_one);
  void ldrawScrollString(const char *text, int len, int x1, int y1, int x2, int y2, int xdisp, int font,
                       int attr, const char *pen, const char *brush);
  void ldrawString(const char *text, int len, int x1, int y1, int x2, int y2,
                int font, int attr, const char *pen, const char *brush);
  void lsetPixel(int x,int y, const char *color);
  void ldirectRedrawGUI (void);
  void lredraw();
  int lstrW(const char *str, int strl, int font);
  int lgetFontH(int font);
  void *lbuildCanvas();
  
  void ldrawStringArray(char **array, int array_size, int x, int y, int x2, int y2, int step, int font, int attr, char *pen, char *brush);
  
]]

local ldrawRectangle = _draw.ldrawRectangle
local ldrawLine = _draw.ldrawLine
local ldrawArc = _draw.ldrawArc
local ldrawRoundedFrame = _draw.ldrawRoundedFrame
local ldrawImg = _draw.ldrawImg
local ldrawCanvas = _draw.ldrawCanvas
local ldrawScrollString = _draw.ldrawScrollString
local ldrawString = _draw.ldrawString
local lsetPixel = _draw.lsetPixel
local ldirectRedrawGUI = _draw.ldirectRedrawGUI
local lredraw = _draw.lredraw
local lstrW = _draw.lstrW
local lgetFontH = _draw.lgetFontH
local lbuildCanvas = _draw.lbuildCanvas
local ldrawStringArray = _draw.ldrawStringArray

function rettable.getColor(r, g, b, a)
  return ffi.new('char[4]', {r, g, b, a})
end


function rettable.drawRectangle(_x, _y, _x2, _y2, _flags, _pen, _brush)
  ldrawRectangle(_x, _y, _x2, _y2, _flags, _pen, _brush)
end


function rettable.drawLine(x, y, x2, y2, _type, pen)
  ldrawLine(x, y, x2, y2, _type, pen)
end


function rettable.drawArc(x1, y1, x2, y2, a1, a2, flag, pen, brush)
  ldrawArc(x1, y1, x2, y2, a1, a2, flag, pen, brush)
end


function rettable.drawRoundedFrame(x1, y1, x2, y2, x_round, y_round, flags, pen, brush)
  ldrawRoundedFrame(x1, y1, x2, y2, x_round, y_round, flags, pen, brush)
end


function rettable.drawImg(x, y, pic)
  ldrawImg(x, y, ffi.cast('void *', pic))
end


function rettable.drawCanvas(data, x1, y1, x2, y2, flag_one)
  ldrawCanvas(data, x1, y1, x2, y2, flag_one)
end


function rettable.drawScrollString(text, len, x1, y1, x2, y2, xdisp, font, attr, pen, brush)
  ldrawScrollString(text, len, x1, y1, x2, y2, xdisp, font, attr, pen, brush)
end


function rettable.drawString(text, len, x1, y1, x2, y2, font, attr, pen, brush)
  ldrawString(text, len, x1, y1, x2, y2, font, attr, pen, brush)
end


function rettable.setPixel(x, y, color)
  lsetPixel(x, y, color)
end


function rettable.directRedrawGUI()
  ldirectRedrawGUI()
end


function rettable.redraw()
  lredraw();
end


function rettable.strW(str, font)
  return lstrW(str, #str, font)
end


function rettable.getFontH(font)
  return lgetFontH(font)
end


function rettable.buildCanvas()
  return lbuildCanvas()
end


function rettable.drawStringArray(array, array_size, x, y, x2, y2, step, font, attr, pen, brush)
  ldrawStringArray(array, array_size, x, y, x2, y2, step, font, attr, pen, brush)
end



return rettable;



