-- $FILE klass.lua
-- $VERSION 0.0.5
-- $AUTHOR Ketmar
-- $DESCRIPTION
--   another simple prototype-based class engine with finalizers and support for 'inherited' calls
--
-- $LICENSE
--   WTFPL
--
-- $DEPENDS
--   kmodule
--
-- $USAGE
--   local newclass = class(nil, "baseclass")
--   function newclass:Method (...) print(self); end;
--
--   local class2 = class(newclass)
--   function class2:Method (...) print(self); inherited(self, ...); end;
--
--   local inst = class2("argument to Constructor!");
--
--   iherited() will do nothing if there's no inherited method. %-)
--   class can have 2 special methods:
--     :Constructor (...)
--     :Destructor ()
--   `Destructor' will be called when class is going to be destroyed by GC
--
--   DO NOT FORGET TO PASS `self' TO inherited()!
--
--   earch method can know it's name from variable (not field!) method
--
--   one can add/alter methods in class or instance in runtime.
--
-- $HISTORY
--   [+] works with Lua 5.2 beta
--   [+] added :isA(obj)
--

-- cache global functions
local assert, type, rawget, rawset = assert, type, rawget, rawset;
local getmetatable, setmetatable = getmetatable, setmetatable;
--local newproxy = newproxy;
local setfenv, getfenv = setfenv, getfenv;
local getinfo, getupvalue, setupvalue, upvaluejoin;
local newlua = false;
local _klassG;

--local print, tostring = print, tostring;


if not setfenv then
  require "debug";
  getinfo, getupvalue, setupvalue, upvaluejoin = debug.getinfo, debug.getupvalue, debug.setupvalue, debug.upvaluejoin;
  newlua = true;

  setfenv = function (f, t)
    f = (type(f) == 'function' and f or getinfo(f+1, 'f').func);
    local up, name = 0;
    repeat
      up = up+1;
      name = getupvalue(f, up)
    until name == '_ENV' or name == nil;
    if name then
      upvaluejoin(f, up, function() return name; end, 1); -- use unique upvalue
      setupvalue(f, up, t);
    end;
  end;

  getfenv = function (f)
    f = (type(f) == 'function' and f or getinfo(f+1, 'f').func);
    local up, name, val = 0;
    repeat
      up = up+1;
      name, val = getupvalue(f, up);
    until name == '_ENV' or name == nil;
    return val;
  end;

  _klassG = _ENV;
end;


module(..., package.seeall);


local specialFields = {
  _className=true,
  _parent=true,
};


-- $DESCR
--   find field in class or in it's parents
-- $IN
--   class
--     class definition or instance
--   fld
--     field name (of any type)
-- $OUT
--  $OR good
--   value
--  $OR bad
--   nil
local function findField (class, fld)
  if specialFields[fld] then return rawget(class, fld); end;
  local res;
  repeat
    --print("findField", fld, class._className);
    res = rawget(rawget(class, "_fields"), fld); if res ~= nil then return res; end;
    -- not found, go up
    class = rawget(class, "_parent");
  until not class;
  -- default result: nil
end;


-- metatable for 'see everything'
--local seeallmt = { __index = _G };

-- $DESCR
--   add field or method, install environment for method
-- $IN
--   class
--     class definition or instance
--   fld
--     field name (of any type)
--   value
--     field value or metod
-- $OUT
local function newField (class, fld, value)
  local fields = rawget(class, "_fields");
  if type(value) == "function" then
    -- new method, change envtable to make inherited() visible
    assert(not specialFields[fld]);
    local inh = function (self, ...)
      --print("inherited", fld, class._className);
      local m = rawget(class, "_parent");
      if m then
        m = findField(m, fld); -- search upwards
        if m then return m(self, ...); end;
      end;
    end;
    --local env = { method = fld };
    --setmetatable(env, seeallmt); setfenv(inh, env);
    --print(tostring(_ENV), tostring(package.seeall));
    local seeallmt;
    if newlua then
      seeallmt = { __index = _ENV };
    else
      seeallmt = { __index = _G };
    end;
    local env = { method = fld, inherited = inh };
    setmetatable(env, seeallmt); setfenv(value, env);
  end;
  rawset(specialFields[fld] and class or fields, fld, value);
end;


local classNew;
-- $DESCR
--   create new class (with possible inheritance)
-- $IN
--   class parent
--     parent or nil
--   nil/str className
--     class name
-- $OUT
--   class
function class (parent, className)
  if className == nil and type(parent) == "string" then className, parent = parent, nil; end;
  local res = {
    New = classNew,
    _className = className or "<unnamed>",
    _parent = parent,
    _fields = {},
    __call = classNew,
    __index = findField,
    __newindex = newField,
  };
  setmetatable(res, res);
  if not res.isA then
    -- no 'isA' method, add it
    res.isA = function (self, class)
      while self do
        if self == class then return true; end;
        self = rawget(self, "_parent");
      end;
      return false;
    end;
  end;
  return res;
end;


-- $DESCR
--   create new class instance
-- $IN
--   class
--     class definition
--   ...
--     constructor args
-- $OUT
--   class_instance
classNew = function (classDef, ...)
  local res = class(classDef, rawget(classDef, "_className"));
  if not newlua then
    -- this trick registers "object finalizer" in Lua 5.1
    local proxy = newproxy(true); -- create proxy (userdata) with empty metatable
    local mt = getmetatable(proxy);
    mt.__gc = function ()
      local dd = res.Destructor;
      if dd then dd(res); end;
    end;
    rawset(res, "__gc", proxy);
  else
    -- this trick registers "object finalizer" in Lua 5.2
    local mt = getmetatable(res);
    rawset(mt, "__gc", function ()
      local dd = res.Destructor;
      if dd then dd(res); end;
    end);
    setmetatable(res, mt);
  end;
  -- end of "object finalizer" trick
  local cc = res.Constructor; if cc then cc(res, ...); end;
  return res;
end;


if newlua then
  _klassG.class = class;
else
  _G.class = class;
end;
