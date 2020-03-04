

local dir = require('dir')




local d, v = dir.findFirstFile('0:\\*.*')

if(d == nil) then
  print('Cannot open dir');
  return 0
end


while(v ~= 0 and v ~= nil) do
  print("Name: "..d:name());
  v = d:findNextFile()
end

d:findClose()
d = nil

