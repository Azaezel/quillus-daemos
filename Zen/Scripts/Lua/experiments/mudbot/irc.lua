local base = _G
require "socket"

module(..., package.seeall)

local metat = { __index = {} }

local connection = { __index = {} }

function connect(host, port)
    local c = assert(socket.connect(host, port))
    c:settimeout(1) --  Do a one-second polling loop

    local running = true

    local send = function(text)
        c:send(text .. "\r\n")
    end

    local readLoop = coroutine.create( function()
        while running do
            local s, status = c:receive("*l")
            if status == "closed" then 
                return 
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
    
    local errorFunctionHandler
    
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
                print(value)

                local first, last, part1, part2 = string.find(value, "^:(.-):(.-)$")

                if first ~= nil then
                    words = {}
                    for w in string.gfind(part1, "%S+") do
                        table.insert(words, w)                        
                    end
                    
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
                    
                else
                    -- Parse PING
                    local words = {}
                    for w in string.gfind(value, "%S+") do
                        table.insert(words, w)
                    end

                    if words[1] == "PING" then
                        c:send("PONG :indiezen")
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

    local nickname

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
                print("Error, cannot say a message in a channel which you have not joined.\r\n")
            end
        end
    end
    
    local errHandler = function(errFunc)
        errorFunctionHandler = errFunc
    end

    newConnection = { 
            send = send, run = run, addTask = addTask, addHandler = addHandler,  getChannel = getChannel,
            getCmd = getCmd, getText = getText, quit = quit, login = login, join = join,
            sendMessage = sendMessage, errHandler = errHandler, say = say
        }
    
    return newConnection
end
