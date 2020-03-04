

local timer = require('timer');
sys.keep_alive();


function test_func()
  print("Exiting ")
  sys.exit();
end


timer.start(test_func, 60);

