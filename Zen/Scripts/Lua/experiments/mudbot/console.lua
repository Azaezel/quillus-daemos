
module(..., package.seeall)

function receive (prod)
  local status, value = coroutine.resume(prod)
  return value
end

function send (x)
  coroutine.yield(x)
end

function reader ()
  return coroutine.create(function ()
    while true do
      local x = io.read()     -- produce new value
      send(x)
    end
  end)
end

function parser (prod)
  return coroutine.create(function ()
    while true do
      local x = receive(prod)   -- get new value
      local words = {}
      for w in string.gfind(x, "%a+") do
          table.insert(words, w)
      end
      send(words)      -- send it to consumer
    end
  end)
end

function consumer (prod)
  while true do
    local x = receive(prod)   -- get new value
    
    for i,v in ipairs(x) do
        io.write(v, "\n")          -- consume new value
    end
  end
end

--p = reader()
--f = parser(p)
--consumer(f)

    
