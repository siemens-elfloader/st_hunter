

local csm = require('csm')
local core = require('core')
local graphics = require('graphics')
local Image = require('image')
local draw = require('draw')
local clock = require('clock')
local timer = require('timer')
local mycsm
local canvas
local break_fill = false
local fps = 0
local fps_count = 0
local cur_time = 0
local pic_way = '0:\\Pictures\\background.png'


-- Класс Image
local fon = Image(pic_way)

local x = 0
local y = 0
local h = fon:height()
local w = fon:width()


local function onRedraw()
  
  if cur_time ~= (clock.time().sec)  then
    fps = fps_count
    print('fps: '..fps)
    fps_count = 0
    cur_time = clock.time().sec
  end
  
  fps_count = fps_count+1
  --fon:draw(0, 0)
  Image.drawImage(0, 0, pic_way)
end


local function fillBackground()
  if break_fill == true then return; end
  
  fon:setPixel(x, y, 0xFF00FF00)
  x = x+1
  fon:setPixel(x, y, 0xFF00FF00)
  x = x+1
  if x >= w then
    y = y+1
    x = 0
  end
  
  if y >= h then
    return
  end
  
  onRedraw()
  timer.start(fillBackground, 1)
end


local function onCreate()
  print('onCreate')
  fillBackground()
end


local function onClose()
  print('onCreate')
  break_fill = true
  fon = nil
end



local function onFocus()
  print('onFocus')
end



local function onUnFocus()
  print('onUnFocus')
end



local function onKey(_type, key)

  if _type == 403 then

    if key == 4 then
      print("Return!!")
      return 1
    end
  end
  
  fon:setPixel(x, y, 0xFF00FF00)
  x = x+1
  fon:setPixel(x, y, 0xFF00FF00)
  x = x+1
  if x >= w then
    y = y+1
    x = 0
  end
  
  --draw.directRedrawGUI()
  onRedraw()
  return 0
end



local function csm_onCreate(csm_data)
  canvas = graphics.new(onRedraw,
		     onCreate,
		     onClose,
		     onFocus, 
		     onUnFocus,
		     onKey)
  
  csm.parentOfGui(csm_data, canvas:id())
end


local function onExit(csm_data)
  print('Exiting...')
  canvas = nil
  mycsm = nil
  sys.exit();
end



local function onMessage(csm_data, msg)
  return 1
end


mycsm = csm.new(1, "Oo", csm_onCreate, onExit, onMessage)

