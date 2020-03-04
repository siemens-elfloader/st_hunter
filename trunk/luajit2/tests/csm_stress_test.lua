
-- Stress test (anti-picoff) debuging

local csm = require('csm')
local core = require('core')
local swi = require('swi')
local timer = require('timer')
local csm_id


local function showmsg(str)
  swi.call(0x148, 'none', 1, str)
end



local function kill()
  showmsg("o_O")
  -- убираем ссылку на обьект
  --csm_id = nil
  
  -- чистим мусор(обьекты не имеющие ссылки), повлечёт за собой закрытия цсма
  --collectgarbage("collect")
  os.exit()
end


function csm_onCreate()
  --print('csm_onCreate')
  --core.subproc(kill)
  --timer.start(kill, 1)
  kill()
end


function onExit()
  print('Exiting...')
  os.exit();
end



function onMessage()
  print('onMessage')
  return 1
end


csm_id = csm.new(1, "Oo", csm_onCreate, onExit, onMessage)
