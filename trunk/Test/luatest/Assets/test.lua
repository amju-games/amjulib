-- test.lua

-- function to be called from C++ 

function hello()
  io.write("Hello world, from ",_VERSION,"!\n");
  -- how to return multiple values, e.g. Vec3 or Matrix? - easy!!?
  return 1, 2, 3;
end

function TestMyRegisteredFunction()
  io.write("Calling C function...\n");
  mycfunc();  
end

--hello();

