-- (c) Z.Vova


local ffi = require('ffi')
local csm = require('csm')
local swi = require('swi')
local core = require('core')
local graphics = require('graphics')
local draw = require('draw')
local clock = require('clock')
local ttf = require('ttf')
local timer = require('timer')
local mycsm
local canvas = nil
local dstr = ""

local counts_draw = 0
local timer_id = -1
local is_stoping = false
local fps = 0
local fps_count = 0
local cur_time = 0
local ftfont

local red =  draw.getColor(0xff, 0, 0, 0x64)
local white = draw.getColor(0xff, 0xff, 0xff, 0x64)
local black = draw.getColor(0, 0, 0, 0x64)


-- ******************************************************************

collectgarbage('setpause', 9000)

-- init font
ftfont = ttf.open('tahoma.ttf', 15)




local function redraw()
  draw.directRedrawGUI()
end


local function onRedraw()

  if is_stoping == true then
    return 0
  end
  
  draw.drawRoundedFrame(0, 0, 131, 175, 0, 0, 0, white, black)


  
  --draw.drawScrollString(dstr, #dstr, 25, 55, 132, 176, 1, 11, 34, red, black)
  
  ttf.drawstring(ftfont, dstr, 35, 55, 132, 176, 1, 0, 128, 0xFF00FF00)
  
  
  if cur_time ~= (clock.time().sec)  then
    fps = fps_count
    dstr = 'FPS '..fps;
    fps_count = 0
    cur_time = clock.time().sec
  end
  
  fps_count = fps_count+1
  timer_id = timer.start(redraw, 1)
end


local function onCreate()
  --print ("onCreate")
end


local function onClose()
  print ("gui onClose")
end


local function onFocus()
  --print ("onFocus")
  is_stoping = false
  
end


local function onUnfocus()
  --print ("onUnfocus")
  is_stoping = true
end


local function onKey(type, code)
  if type == 403 then

    if code == 4 then
      is_stoping = true
      timer_id:stop()
      --sys.exit()
      return 1
    elseif code == (string.byte('5')) then
      is_stoping = true
    end
     
  end
   
  draw.directRedrawGUI()
  return 0
end




local function csm_onCreate(csm_data)
  canvas = graphics.new(onRedraw,
	   onCreate,
	   onClose,
	   onFocus,
	   onUnfocus,
	   onKey)
	  
  
  csm.parentOfGui(csm_data, canvas:id())
end


local function onExit(csm_data)
  print('Exiting...')
  print('counts_draw: '..counts_draw)
  canvas = nil
  mycsm = nil
  ttf.close(ftfont)
  sys.exit();
end



local function onMessage(csm_data, msg)
  --print('onMessage', mycsm:id())
  return 1
end


mycsm = csm.new(1, "Oo", csm_onCreate, onExit, onMessage)



