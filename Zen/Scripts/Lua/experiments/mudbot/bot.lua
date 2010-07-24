
require "irc"
require "functions"

require "playerCommands"

local lastExecutedLine = ""

bot = {}

function initBot(hostname, port, nickname, channel)
    local connection = irc.connect(hostname, port);
    bot.nickname = nickname;

    bot.connection = connection;
    bot.logDirectory = "logs/";

    function login()
        connection.login(bot.nickname)
        return true
    end

    -- Handle the MODE command
    function handleMode()
        print("Connected!")
        connection.join(channel)
    end

    function handleMsg()
        -- Never returning
        while true do
            --print("Got message " .. connection.getText())

            -- Parse the message 
            local first, last, part1, part2 = string.find(connection.getText(), "^(.-):(.-)$")
            -- Would like to make a way for her to listen to Ginger, as well as her name appended.
            
            local handled = false;
            
            if first ~= nil then
                if part1 == bot.nickname then
                    first, last, nick = string.find(connection.getCmd(1), "(.-)!")
                    if nick == nil then 
                        print("Error parsing nick")
                    else
                        local channel = connection.getChannel();
                        if channel == bot.nickname then
                            channel = nick;
                        end
                        handlePlayerCommand(connection, channel, nick, part2)
                        print("Finished handlePlayerCommand")
                        handled = true;
                    end
                end
            end
            
            if not handled then
                -- This is where you would add parsing of messages not directed at the bot
                local channel = connection.getChannel();
                first, last, nick = string.find(connection.getCmd(1), "(.-)!")
                
                -- If this is a private message, assume that it's directed at the bot
                if channel == bot.nickname then
                    channel = nick;
                    handlePlayerCommand(connection, channel, nick, connection.getText());
                else
                    print("Not meant for me, ignoring \"" .. connection.getText() .. "\"")
                end
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
          lastExecutedLine = connection.getText()
            if handler == "PRIVMSG" then
                print("Error executing handler: PRIVMSG " .. lastExecutedLine)
                connection.sendMessage("Error", lastExecutedLine)
                connection.addHandler("PRIVMSG", coroutine.create(function() return handleMsg() end))
            end
        end
    )
    
end

function runBot(hostname, port, nickname, channel)
    initBot(hostname, port, nickname, channel);
    bot.connection.run();
end

function handlePlayerCommand(connection, channel, nick, command)
    print("handlePlayerCommand");
    -- Parse into words
    local words = {}
    for w in string.gfind(command, "%S+") do
        -- Check to see if the word has any leading or trailing punctuation marks
        local first, last = string.find(w, "^%p+");
        local prefix = nil;
        if first ~= nil then
            prefix = string.sub(w, first, last);
        end
        if prefix ~= nil and prefix ~= "" then
            table.insert(words, prefix)
            w = string.gsub(w, "^%p+", "")
        end
        table.insert(words, w)
    end

    print("Checking dispatch table for " .. words[1])
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
function reload(packageName)
    package.loaded[packageName] = nil;
    require(packageName)
    
    if package.loaded[packageName] ~= nil then
        print("Reloaded " .. packageName)
        --connection.sendMessage("Reloaded ", packageName)
    else
        print("Error reloading " .. packageName)
        --connection.sendMessage("Error reloading ", packageName)
    end
end

