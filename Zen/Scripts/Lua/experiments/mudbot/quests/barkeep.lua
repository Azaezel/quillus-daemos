require "quests.questEngine"

module(..., package.seeall)

local metat = { __index = {} }

npc = quests.questEngine.newNPC("Barkeep", {})

npc.states = {}
npc.states["init"] =
{
    name = "init",
    
    description = "Initial state for Barkeep.",

    input = {
        ["click"] = {
            name = "click",
            logic = function(npc, player)
                npc:transitionTo(player, "store")
            end
        }
    },

    onTransition = {
        -- Transition from init to clicked
        ["stage1"] = function(npc, player)
        end
    },

    onEnter = function(npc, player)
    end,

    onExit = function(npc, player)
    end
}

npc.states["store"] =
{
    name = "store",

    description = "Main barkeep Store dialog",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player) 
                npc:transitionTo(player, "init")
            end
        },
        ["buy ale"] = {
            name = "buy ale",
            logic = function(npc, player)
                if player:buy("Ale", 1) then
                else
                    npc:say(player, "I am sorry but you don not have enough coin.")
                end
                npc:transitionTo(player, "init")
            end
        },
        ["buy stout ale"] = {
            name = "buy stout ale",
            logic = function(npc, player)
                if player:buy("Stout Ale", 3) then
                else
                    npc:say(player, "I am sorry but you don not have enough coin.")
                end
                npc:transitionTo(player, "init")
            end
        }
    },

    onEnter = function(npc, player)
        npc:say(player, "How may I help you?")
        npc:option(player, "Buy an ale for 1 coin.", "buy ale")
        npc:option(player, "Buy a stout ale for 3 coins.", "buy stout ale")
        npc:option(player, "Exit", "exit")
        npc:emote("(" .. player:getName() .. " has " .. player:getCoins() .. " coins to spend.)")
    end,

    onExit = function(npc, player)
        npc:say(player, "Thank you for your business.")
        --npc:closeDialog(player)
    end
}
