
local draw = require('draw')
local ttf = require('ttf')
local font
local fontY

local ftfont12 = ttf.open('tahoma.ttf', 12)

function _menu_export(_font, _fonty)
  font = _font
  fontY = _fonty
end

function _menuopen()
  setMenuDrawing(false)
  enter_to()
  __onredraw()
end


function _menu_deleteBox(del)
  if del == 0 then
    return 0
  end
  
  i = 0
  names = nil
  num = nil
  notselect = nil
  names, num, notselect = chekedFileList()
  

  if notselect == true  then
    
    print("element '"..curentFolder()..nameByIndex(names).."' has ben cheked")
    setFileUnChecked(names)
    
  else 
    while i < num do
    
      if names[i] == true then
	print("element '"..curentFolder()..nameByIndex(i).."' has ben cheked")
	setFileUnChecked(i)
      end
    
      i = i+1
    end
  end
 
end


function _menu_delete()
  setMenuDrawing(false)
  --alert.msg("Удалить выделенные файлы?!", '_menu_deleteBox')
end


function _menu_rename()
  setMenuDrawing(false)
  --ui.msg(1, "Not support")
end


function _menu_exit()
  setMenuDrawing(false)
  --ui.msg(1, "Not support")
  sys.exit()
end


local menu_cursorpos = 0
local menu_elements_c = 4
local menu_pos = { 1, 60, 95, 58 }
local menu_elements = {
  "Открыть",
  "Удалить",
  "Переименовать",
  "Выход" }

  
local menu_elements_do = {
  _menuopen,
  _menu_delete,
  _menu_rename,
  _menu_exit }

function keyMenu(type, code)
  
  if type == 403 or type == 405 then
    
    if code == 4 then 
      setMenuDrawing(false)
      __onredraw()
      return 0
      
    elseif code == 26 then
      menu_elements_do[menu_cursorpos+1]()
      
    elseif code == 59 then
      menu_cursorpos = menu_cursorpos-1
      
    elseif code == 60 then
      menu_cursorpos = menu_cursorpos+1
      
    end
  end
  
  if menu_cursorpos >= menu_elements_c then
    menu_cursorpos = 0
    
  elseif menu_cursorpos < 0 then
    menu_cursorpos = menu_elements_c-1
  end
  
  drawMenu()
  return 0
end

local black = draw.getColor(0,0,0,100)
local hred = draw.getColor(50, 255, 50, 100)
local hyellow = draw.getColor(255, 255, 50, 100)
local red = draw.getColor(255, 0, 0, 100)
local br = 2


function drawMenu()
  
  draw.drawRoundedFrame(menu_pos[1], menu_pos[2], menu_pos[1]+menu_pos[3],
			menu_pos[2]+menu_pos[4], 0, 0, 0, 
			hred, black)
  

  draw.drawRoundedFrame(menu_pos[1]+1, 1+menu_pos[2]+(fontY*menu_cursorpos)+(br*menu_cursorpos),
			menu_pos[1]+menu_pos[3]-1, 
			menu_pos[2]+fontY+(fontY*menu_cursorpos)+1+(br*menu_cursorpos)+br, 
			0, 0, 0, hyellow, black)

  i = 0
  sk = 0
  
  while i < menu_elements_c do
    
    --draw.drawScrollString(menu_elements[i+1], #menu_elements[i+1], menu_pos[1]+2, menu_pos[2]+2+sk,
	--				    menu_pos[1]+menu_pos[3], 
	--				    menu_pos[2]+menu_pos[4], 1, 11, 34, 
	--				    hyellow, red)
    
    ttf.drawstring(ftfont12, menu_elements[i+1], menu_pos[1]+1, menu_pos[2]+5+sk,
					    menu_pos[1]+menu_pos[3], 
					    menu_pos[2]+menu_pos[4],
					    8, 0, 128, 0xFFFFFF32)
    
    i = i+1
    sk = sk+fontY+br
  end
  
end


