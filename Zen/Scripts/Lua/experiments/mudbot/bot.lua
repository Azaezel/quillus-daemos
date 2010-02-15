
require "irc"

require "quests.questEngine"
require "playerCommands"

local lastExecutedLine = ""

function runBot(hostname, port, nickname, channel)
    local connection = irc.connect(hostname, port)

    function login()
        connection.login(nickname)
        return true
    end

    -- Handle the MODE command
    function handleMode()
        print("Connected!")
        connection.join(channel)
        --connection.send("PRIVMSG " .. channel .. " :\001ACTION salutes SgtFlame|Zen\001")
    end

    function handleMsg()
        -- Never returning
        while true do
            print("Got message " .. connection.getText())

            -- Parse the message 
            local first, last, part1, part2 = string.find(connection.getText(), "^(.-):(.-)$")
            
            if first ~= nil then
                if part1 == nickname then
                    first, last, nick = string.find(connection.getCmd(1), "(.-)!")
                    if nick == nil then 
                        print("Error parsing nick")
                    else
                        handlePlayerCommand(connection, connection.getChannel(), nick, part2)
                        print("Finished handlePlayerCommand")
                    end
                else
                    -- You can handle messages not directed at the bot here.
                end
            else
                -- This is where you would add parsing of messages not directed at the bot
                print("Not meant for me, ignoring \"" .. connection.getText() .. "\"")
            end
            
            coroutine.yield(1)
        end
    end

    connection.addTask(coroutine.create(function() return login() end))
    connection.addHandler("MODE", coroutine.create(function() return handleMode() end))
    connection.addHandler("PRIVMSG", coroutine.create(function() return handleMsg() end))

    -- Create an error handler that will re-create the PRIVMSG handler in case a registered handler fails
    connection.errHandler(
        function(handler)
            if handler == "PRIVMSG" then
                print("Error executing handler: PRIVMSG " .. lastExecutedLine)
                connection.sendMessage("Error", lastExecutedLine)
                connection.addHandler("PRIVMSG", coroutine.create(function() return handleMsg() end))
            end
        end
    )

    connection.run()
end

function handlePlayerCommand(connection, channel, nick, command)
    print("handlePlayerCommand");
    -- Parse into words
    local words = {}
    for w in string.gfind(command, "%S+") do
        table.insert(words, w)
    end

    print("Checking dispatch table")
    print("words[1] is " .. words[1])
    if playerCommands[words[1]] ~= nil then
        print("Found a command handler for " .. words[1])
        debug.sethook(trace, "l")
        playerCommands[words[1]](connection, channel, nick, command, words)
        debug.sethook(nil, "l")
    else
        print("Didn't find a command handler for " .. command .. " or " .. words[1])        
    end
    print("done")
end
