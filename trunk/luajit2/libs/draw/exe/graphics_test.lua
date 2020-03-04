
local ffi = require('ffi')
local csm = require('csm')
local core = require('core')
local graphics = require('graphics')
local draw = require('draw')
local mycsm
local canvas



local red =  ffi.new('char[4]', {0xff, 0, 0, 0x64})
local white = ffi.new('char[4]', {0xff, 0xff, 0xff, 0x64})
local black = ffi.new('char[4]', {0, 0, 0, 0x64})



local function onRedraw()
  
  draw.drawRectangle(0, 0, 131, 175, 0, red, white);
  
  draw.drawString(0, "o_O", 3, 3, 20, 132, 176, 4, 34, red, black)
  --print('onRedraw')
end



function onCreate()
  print('onCreate')
end


function onClose()
  print('onClose')
end



function onFocus()
  print('onFocus')
end



function onUnFocus()
  print('onUnFocus')
end



function onKey(type, key)
  print('onKey')
  return 0
end



function csm_onCreate(csm_data)
  canvas = graphics.new(onRedraw,
		     onCreate,
		     onClose,
		     onFocus, 
		     onUnFocus,
		     onKey)
  
  csm.parentOfGui(csm_data, canvas:id())
end


function onExit(csm_data)
  print('Exiting...')
  canvas = nil
  sys.exit();
end



function onMessage(csm_data, msg)
  return 1
end


mycsm = csm.new(1, "Oo", csm_onCreate, onExit, onMessage)

