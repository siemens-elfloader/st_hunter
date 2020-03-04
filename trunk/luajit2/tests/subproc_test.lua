

local core = require('core');
sys.keep_alive();


function test_func(o, t, s)
  print("Exiting "..o..' '..t..' '..s)
  sys.exit();
end


core.subproc(test_func, 1, 2, 'good bay');

