local base = _G

require "player"

module(..., package.seeall)

local metat = { __index = {} }

local npcs = {}

-- TODO Move this elsewhere?
local players = {}

function newNPC(_name, npc)
    local private = { name = _name }
    local getName = function()
        return private.name
    end
    
    local function checkState(npc, player)
        -- Default to the "init" state
        if npc.currentState[player] == nil then
            npc.currentState[player] = npc.states["init"]

            -- Fire the onEnter event if the init state exists
            if npc.currentState[player] ~= nil then
                if npc.currentState[player].onEnter ~= nil then
                    npc.currentState[player].onEnter(npc, player)
                end
            end
        end
    end

    local fireEvent = function(npc, player, eventName)
        print("Firing event " .. eventName .. " on npc " .. npc:getName())

        -- Check the state
        checkState(npc, player)
        
        -- If we have a valid event, fire it
        if npc.currentState[player] ~= nil then
            if npc.currentState[player].input[eventName] ~= nil then
                print("Valid event, about to fire it as " .. npc.currentState[player].input[eventName].name)
                local event = npc.currentState[player].input[eventName].logic
                print (type(event))
                print(event)
                if event ~= nil then
                    print("Fire!")
                    event(npc, player)
                    print("Hit!")
                else
                    print ("Invalid event " .. eventName)
                end
            else
                print("Cannot transition to " .. eventName .. " from " .. npc.currentState[player].name)
            end
        else
            print(npc.getName() .. " is in an invalid state.")
        end
        print("Finished firing event " .. eventName)
    end

    local transitionTo = function(npc, player, stateName)
        checkState(npc, player)

        if npc.states[stateName] == nil then
            player.sendMessage("Invalid state " .. stateName .. " for NPC " .. npc:getName())
            return
        end

        -- Find the transition event
        if npc.currentState[player].onTransition ~= nil then
            if npc.currentState[player].onTransition[stateName] ~= nil then
                npc.currentState[player].onTransition[stateName](npc, player)
            end
        end

        -- Fire the onExit event
        if npc.currentState[player].onExit ~= nil then
            npc.currentState[player].onExit(npc, player)
        end
        
        npc.currentState[player] = npc.states[stateName]

        -- Fire the onEnter event
        if npc.currentState[player].onEnter ~= nil then
            npc.currentState[player].onEnter(npc, player)
        end

    end

    npc.getName = getName
    npc.fireEvent = fireEvent
    npc.transitionTo = transitionTo
    
    setmetatable(npc, metat)

    npcs[_name] = npc

    -- Create the map from player to current state
    npc.currentState = {}
    
    -- Create a map of state names to states
    npc.states = {}

    return npc
end

function use(connection, channel, nick, command, words)
    if words[2] == nil then
        connection.sendMessage(nick, "Usage - USE {npc name}")
    elseif npcs[words[2]] ~= nil then
        npc = npcs[words[2]]
        
        if npc.say == nil then
            npc.say = function(self, player, message)
                connection.sendMessage(self:getName() .. " says to " .. player:getName(), message)
                -- Reset the sayOptions
                player.sayOptions = {}
            end

            npc.option = function(self, player, message, event)
                local count = table.getn(player.sayOptions) + 1
                connection.say(count .. " - " .. message)
                player.sayOptions[count] = event
            end

            npc.emote = function(self, message)
                connection.say("\001ACTION " .. message .. "\001")
            end

        end

        -- Create a player if it doesn't exist
        if players[nick] == nil then
            -- Make a new player
            players[nick] = player.new(connection, nick)
        end

        player = players[nick]

        -- First make sure the player isn't already using an NPC
        if player.use ~= nil then
            if player.use ~= npc then
                npc:fireEvent(player, "exit")
            else
                connection.sendMessage(nick, "You are already using that NPC")
            end
        end

        -- Fire the "click" event
        if npc.currentState[npc] == nil then
            npc.currentState[npc] = npc:fireEvent(player, "click")
            player.use = npc
        end
    else
        -- TODO Handle other types of useable objects
        connection.sendMessage(nick, "Unknown NPC or Object")
    end
    print("Done with use()")
end

function say(connection, channel, nick, command, words)
    local player = players[nick]
    if player == nil then
        connection.sendMessage(nick, "Use an NPC first.")
        return
    end

    local npc = player.use

    if npc == nil then
        connection.sendMessage(nick, "Use an NPC first.")
        return
    end
    
    if player.sayOptions[tonumber(words[2])] == nil then
        connection.sendMessage(nick, "Invalid option")
        for i,v in ipairs(player.sayOptions) do
            print(i .. ":" .. v)
        end
        return
    end

    npc:fireEvent(player, player.sayOptions[tonumber(words[2])])

end

