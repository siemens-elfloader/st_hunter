

local csm = require('csm')
local core = require('core')
local csm_id



function kill()
  -- убираем ссылку на обьект
  csm = nil
  
  -- чистим мусор(обьекты не имеющие ссылки), повлечёт за собой закрытия цсма
  collectgarbage("collect")
end


function csm_onCreate(csm_data)
  core.subproc('kill')
end


function onExit(csm_data)
  print('Exiting...')
  sys.exit();
  sys.exit();
end



function onMessage(csm_data, msg)
  return 1
end


csm_id = csm.new(1, "Oo", 'csm_onCreate', 'onExit', 'onMessage')
