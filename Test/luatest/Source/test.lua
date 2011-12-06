-- test.lua

-- function to be called from C++ 

function hello()
  io.write("Hello world, from ",_VERSION,"!\n");
  return 1;
end

--hello();

