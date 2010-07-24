function rndError(nick)
  rnd = math.random(2)
  errors = { "You cannot do that, ".. nick..".", "HELP! HELP! ABUSE!" }  
  return errors[rnd]
end 


--function items()
--  rnd = math.random(4)
--  items = { "small trout", "blunt sword", "blunderbuss", "ship's deck" }
--  return rnd
--  return items
--end

playerCommands["help"] = function (connection, channel, nick, command, words)
  connection.send("PRIVMSG " .. channel .. " :Whaddya think I am, a deckhand?")
end

playerCommands["thanks"] = function (connection, channel, nick, command, words)
  connection.say("No problem " .. nick)
end

playerCommands["kick"] = function (connection, channel, nick, command, words)
  connection.say("This isn't working yet.")
end

playerCommands["log"] = function (connection, channel, nick, command, words)
  connection.say("This isn't working yet.")
end

playerCommands["uptime"] = function (connection, channel, nick, command, words)
  if operators[nick] ~= nil then
    local up = io.popen("uptime")
    local input = up:read("*a")
    local time = {} 
    time = explode(",",input)
    connection.say(time[1]..","..time[2])
    up:close()
  else
    connection.say(rndError(nick))
  end
end

playerCommands["logme"] = function (connection, channel, nick, command, words)
  connection.say("Trying to commit to chanlog...")
  local size = words:size())
  local iSize = size
  local i = 1
  connection.say("DEBUG: Word Count is " .. size .. ".")
  while i < iSize 
  connection.say("DEBUG: " .. words[i])
  i = i + 1
  end
  --io.open("chanlog", "rw")
  --io.write(nick .. "%" .. words .. "\n")
  --io.input():close()
  --connection.say("Wrote to file. w00t!")
  --connection.say("Motherfrakking W#%#$%^^")
  --connection.say("It failed. " .. nick .. ":(")
end
