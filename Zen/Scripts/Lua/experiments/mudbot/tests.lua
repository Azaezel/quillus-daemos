package.path=package.path..";tests\\scripts\\?.lua"

require "avatar"
require "console"

function testConsole()
    local p = console.reader()
    local f = console.parser(p)
    console.consumer(f)
end

function testAvatar()
    local p1 = avatar.new("Rhapsody")
    p1:say("Hello!")
    
    local p2 = avatar.new("Tony")
    p2:say("Hello")
end

function testIRC()
    local connection = irc.connect("localhost", 16667)

    function doSomething()
        print("Waiting for a few things.")
        coroutine.yield(1)
        coroutine.yield(1)
        coroutine.yield(1)

        print("Sending USER")
        connection.send("USER luabot localhost localhost :Tony")
        coroutine.yield(1)
        coroutine.yield(1)
        coroutine.yield(1)

        print("Sending NICK")
        connection.send("NICK Rhapsody")
        coroutine.yield(1)

        print("Done")
        return true
    end
    
    function handleMode()
        print("Connected!")
        connection.send("JOIN #fractureduniverse")
        
    end

    function handleMsg()
        -- Never returning
        while true do
            print("Got message" .. connection.getText())

            coroutine.yield(1)
        end
    end
    
    connection.addTask(coroutine.create(function() return doSomething() end))
    connection.addHandler("MODE", coroutine.create(function() return handleMode() end))
    connection.addHandler("PRIVMSG", coroutine.create(function() return handleMsg() end))

    connection.run()
end

--testIRC()

function testParsing()
    x = {
        ":indiezen 042 Rhapsody 0IZAAADQW :your unique ID",
        ":indiezen 251 Rhapsody :There are 0 users and 24 invisible on 2 servers",
        ":indiezen 252 Rhapsody 5 :IRC Operators online",
        ":indiezen 254 Rhapsody 24 :channels formed",
        ":indiezen 255 Rhapsody :I have 17 clients and 1 servers",
        ":indiezen 265 Rhapsody :Current local users: 17  Max: 31",
        ":indiezen 266 Rhapsody :Current global users: 24  Max: 38",
        ":Rhapsody!~luabot@127.0.0.1 MODE Rhapsody :+i",
        nil
        }

    for i,v in ipairs(x) do
        first, last, part1, part2 = string.find(v, "^:(.-):(.-)$")
        
        if first ~= nil then
            print("start = " .. first)
            print("second = " .. last)
            print("part1 = " .. part1)
            print("part2 = " .. part2)

            local words = {}
            for w in string.gfind(part1, "%S+") do
                table.insert(words, w)
            end

            if words[2] == "MODE" then
                print(words[3] .. " nickname accepted")
            end
        else
            print("Didn't match: " .. v)
        end
    end
        
end

--testParsing()

