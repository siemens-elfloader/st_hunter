
require('klass')
local ffi = require('ffi')
local libc = ffi.load('libc-1.0.so')
local drawengine = ffi.load('libdrawengine-0.1.so')
local _image = ffi.load(sys.libDir()..'core\\image.so')


ffi.cdef [[
  
  typedef struct
  {
    unsigned short w, h;
    unsigned int bpnum;
    unsigned char *bitmap;
    unsigned char blend;
  }image_t;
  
  image_t *image_createFromPng(const char *p, int bpnum);
  void free_image(image_t *);
  void drawimage(int x, int y, image_t *img);
  
  void drawpixel32(image_t *buffer, uint16_t x, uint16_t y, uint32_t color);
  uint32_t pixel32(image_t *buffer, uint16_t x, uint16_t y);
  
  void drawImage(int x, int y, const char *pic);
]]


Image = class('Image')
Image._bitmap = nil
Image._w = nil
Image._h = nil
Image._bpnum = nil
Image._image = nil



function Image:Constructor(png_file)
  if( png_file ~= nil ) then
    print('Loading '..png_file)
    if(self:loadFromPng(png_file) == nil) then
      print('Loading image error')
    end
  end
end


function Image:Destructor()
  self:release()
end



function Image:loadFromPng(png_file)
  local im = drawengine.image_createFromPng(png_file, 3)
  if( ffi.cast('int', im) == 0 ) then
    return nil
  end
  
  self._bitmap = im.bitmap
  self._w = im.w
  self._h = im.h
  self._bpnum = 32
  self._image = im
  return self
end


function Image:draw(x, y)
  if(self._image == nil) then return; end
  drawengine.drawimage(x, y, self._image)
end


function Image:release()
  if(self._image ~= nil) then
    drawengine.free_image(self._image); 
  end
  
  self._bitmap = nil
  self._w = nil
  self._h = nil
  self._bpnum = nil
  self._image = nil
end


function Image:width()
  return self._w
end


function Image:height()
  return self._h
end


function Image:bitmap()
  return ffi.cast('int *', self._bitmap)
end


function Image:setPixel(x, y, color)
  if(self._image == nil) then return; end
  drawengine.drawpixel32(self._image, x, y, color)
end


function Image:pixel(x, y)
  if(self._image == nil) then return; end
  return drawengine.pixel32(self._image, x, y)
end

-- static method for drawing pictures by way
function Image.drawImage(x, y, pic)
  _image.drawImage(x, y, pic)
end


return Image










