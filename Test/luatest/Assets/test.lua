-- test.lua

-- function to be called from C++ 

function hello_ret3vals()
  io.write("lua: Hello world, from ",_VERSION,"!\n");
  -- how to return multiple values, e.g. Vec3 or Matrix? - easy!!?
  return 1, 2, 3;
end

function TestMyRegisteredFunction()
  io.write("lua: Calling C function...\n");
  t = {}
  t["x"] = 1
  t["y"] = 2
  t["z"] = 3
  return mycfunc(t, 78, "wow!", true);  
end

--hello();

