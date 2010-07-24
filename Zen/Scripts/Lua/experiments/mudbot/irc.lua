local base = _G
require "socket"
require "mudbot.services"
require "save"

module(..., package.seeall)

local metat = { __index = {} }

local connection = { __index = {} }

function connect(host, port)
    local c = assert(socket.connect(host, port))
    c:settimeout(1) --  Do a one-second polling loop

    local running = true

    local send = function(text)
        print("SEND: " .. text);
        c:send(text .. "\r\n")
    end

    local readLoop = coroutine.create( function()
        while running do
            local s, status = c:receive("*l")
            if status == "closed" then 
                return 
            end
            if (s ~= nil) then
				-- DEBUG print out everything received.
                --print("RECV: " .. s);
            end
            coroutine.yield(s)
        end
    end)

    local tasks = {}
    local handlers = {}
    
    local fullString = ""
    local defaultChannel = nil
    local command = ""
    local text = ""
    local words = {}
    
    local nickname

    local errorFunctionHandler
    
	local getFullString = function()
		return fullString;
	end
	
    -- Empty log handler
    local doLog = function() end;
    
    local setLogHandler = function(logHandler)
        doLog = logHandler;
    end
    
    local run = function()
        while running do
            -- First do all of the background tasks
            local n = table.getn(tasks)
            local start = 1
            while n > 0 do
                for i = start, n do
                    local status, res = coroutine.resume(tasks[i])
                    if coroutine.status(tasks[i]) == "dead" then
                        -- The thread finished its task
                        table.remove(tasks, i)
                        break
                    end
                    start = i + 1
                end
                n = table.getn(tasks)
                if start > n then break end
            end
            
            if not running then
                return
            end
            
            local status, value = coroutine.resume(readLoop)

            fullString = value

            -- Parse the string we got from the server
            if value ~= nil then

                local first, last, part1, part2 = string.find(value, "^:(.-):(.-)$")

                if first ~= nil then
                    words = {}
                    for w in string.gfind(part1, "%S+") do
                        table.insert(words, w)             
                    end

					-- TODO Don't hard-code this server name
					if (words[1] == "indiezen") then
						-- Handle this as a server status message
						ircServices["ServerServ"].defaultHandler(bot.connection, words, part2);
					else
						-- Reset the channel and then see if we have a new one for this message
						defaultChannel = nil
						for i,word in ipairs(words) do
							if word == "PRIVMSG" then
								defaultChannel = words[i + 1]
								break
							end
						end

						text = part2
						-- Dispatch a handler
						if handlers[words[2]] ~= nil then
							command = words[2]
							local status, res = coroutine.resume(handlers[words[2]])
							if coroutine.status(handlers[words[2]]) == "dead" then
								-- Remove the task because it's done
								print("Removing handler for " .. words[2])
								handlers[words[2]] = nil
								errorFunctionHandler(words[2])
							end
						end
						
						doLog();
					end
                else
                    -- Parse Server commands
                    local words = {}
                    for w in string.gfind(value, "%S+") do
                        table.insert(words, w)
                    end
                    
                    command = ircServices["ServerServ"].commands[words[1]];
                    
                    if (command ~= nil) then
                        command.handler(bot.connection, words, part2);
					else
                        print(words[1])
                        print("ERR: Couldn't parse " .. value)
                    end
                end
            end
            
            -- Stop running if the read loop is dead
            if coroutine.status(readLoop) == "dead" then 
                running = false 
            end

        end
    end

    local addTask = function(task)
        table.insert(tasks, task)
    end
    
    local addHandler = function(command, handler)
        handlers[command] = handler
    end
    
    local getCmd = function(x)
        return words[x]
    end

    local getText = function()
        return text
    end
    
    local getChannel = function()
        return defaultChannel
    end
    
    local quit = function()
        print("Quitting")
        running = false
        c:close()
    end
    
    local channels = {}
	
	local setChannels = function(newChannels)
	
		bot.reload("savedChannels.lua");
				
		-- Set the channels to be the new channels (possibly temporarily save the current channels table
		local oldChannels = channels;
		channels = newChannels;
		
		-- Iterate through the new channels and join them if not already joined.
		for channelName, channel in channels do
			-- Check to see if the channel has already been joined
			-- We've already joined if the channel exists in the original channels table.
			if oldChannels[channelName] == nil then
				-- Join the channel because it wasn't in the old list.
				connection.join(channelName);
			else
				-- We're already in the channel, so remove it from the old list.
				-- This will make it so that when we're done, everything still
				-- in the old list we need to leave.
				oldChannels[channelName] = nil;
			end
		end
		
		
		-- Once you're done, any channels that remain in the old channels table you need to "leave".
		for channelName, channel in oldChannels do
			connection.part(channelName);
		end
	end
	
	local saveChannels = function()
		local file = io.open("savedChannels.lua", "w");

		file:write("channels = {}\n");
    
		for channelIndex, channel in pairs(channels) do
			file:write("channels[\"" .. channelIndex .. "\"] = channel(\"" .. channel .. "\");\n");
        end

		file:close();
	end
	
    local login = function(_nickname)
        nickname = _nickname
        send("USER luabot localhost localhost :" .. nickname)
        send("NICK " .. nickname)        
    end

    local join = function(_channel)
        send("JOIN ".. _channel)
        if defaultChannel ~= nil then
            defaultChannel = _channel
        end
        channels[_channel] = { name = _channel}
    end
    
    local leave = function(_channel)
        send("PART ".. _channel)
    end

    local sendMessage = function(toNick, message)
        send("PRIVMSG " .. defaultChannel .. " :" .. toNick .. ": " .. message)
    end

    local say = function(message, channel)
        if channel == nil then
            send("PRIVMSG " .. defaultChannel .. " :"  .. message)
        else
            -- Verify that the channel has been joined
            if channels[channel] ~= nil then
                send("PRIVMSG " .. channel .. " :"  .. message)
            else
                -- TODO If this channel doesn't start with # then it's a private message so this error is incorrect
                print("Error, cannot say a message in a channel which you have not joined (" .. channel .. "\r\n")
                
                send("PRIVMSG " .. channel .. " :"  .. message)
            end
        end
    end

    local broadcast = function(message)
        for key, value in pairs(channels) do
            say(message, value.name);
        end
    end
    
    local errHandler = function(errFunc)
        errorFunctionHandler = errFunc
    end

    newConnection = { 
            send = send, run = run, addTask = addTask, addHandler = addHandler,  getChannel = getChannel,
            getCmd = getCmd, getText = getText, quit = quit, login = login, join = join,
            sendMessage = sendMessage, errHandler = errHandler, say = say, broadcast = broadcast,
            setLogHandler = setLogHandler, saveChannels = saveChannels, setChannels = setChannels,
			getFullString = getFullString
        }
    
    return newConnection
end
