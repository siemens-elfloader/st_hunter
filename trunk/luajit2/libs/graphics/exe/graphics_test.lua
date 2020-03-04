

local csm = require('csm')
local core = require('core')
local graphics = require('graphics')
local mycsm
local canvas



function onRedraw()
  print('onRedraw')
end



function onCreate()
  print('onCreate')
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
  canvas = graphics.new("onRedraw",
		     "onCreate",
		     "",
		     "onFocus", 
		     "onUnFocus",
		     "onKey")
  
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


mycsm = csm.new(1, "Oo", 'csm_onCreate', 'onExit', 'onMessage')

