
playerCommands = {}

operators = {  }

operators["CptThomas"] = 1

playerCommands["quit"] = function (connection, channel, nick, command, words)
    if operators[nick] ~= nil then
        connection.say("Ok, " .. nick .. " see ya!", channel)
        connection.quit()
    else
        connection.say("Error, unauthorized access.", channel)
    end
end

playerCommands["join"] = function (connection, channel, nick, command, words)
    if operators[nick] ~= nil then
        connection.say("Ok, " .. nick .. "... you want me to join channel " .. words[2]..words[3] .. "?", channel)
        connection.join(words[2]..words[3])
        connection.say("Ok, I'm here.", words[2])
    else
        connection.say("Error, unauthorized access.")
    end
end

playerCommands["part"] = function (connection, channel, nick, command, words)
    if operators[nick] ~= nil then
        --connection.leave(words[2]..words[3])
		connection.part(words[2]..words[3])
        --connection.say("This needs fixed! :(")
		connection.say("words[2]"..words[2].."words[3]"..words[3])
		connection.say("I'm leavin'")
    else
        connection.say("You can't do that!")
    end
end

playerCommands["hello"] = function (connection, channel, nick, command, words)
    connection.say("Hello, " .. nick .. ".", channel)
end

playerCommands["use"] = function (connection, channel, nick, command, words)
    quests.questEngine.use(connection, channel, nick, command, words)
    print("exiting use function")
end

playerCommands["say"] = function (connection, channel, nick, command, words)
    quests.questEngine.say(connection, channel, nick, command, words)
    print("exiting say function")
end

local debugging = false

playerCommands["debug"] = function (connection, channel, nick, command, words)
    if table.getn(words) == 1 then
        if debugging then state = "true" else state  = "false" end
        connection.say("debug is " .. state, channel)
    else
        if words[2] == "false" then debugging = false end
        if words[2] == "true" then debugging = true end
    end
end

playerCommands["rename"] = function (connection, channel, nick, command, words)
	connection.send("NICK " .. words[2])
end

--Seems to only work if package is not already required
playerCommands["reload"] = function (connection, channel, nick, command, words)
    if words[2] ~= "main" then
      package.loaded[words[2]] = nil;
      reload(words[2]);
    else
      connection.say("No way!")
    end
end

function trace(event, line)
    lastExecutedLine = debug.getinfo(2).short_src .. ":" .. line
    if debugging then
        print(lastExecutedLine)
    end
end

