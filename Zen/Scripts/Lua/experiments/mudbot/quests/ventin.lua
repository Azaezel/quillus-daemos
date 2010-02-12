require "quests.questEngine"

module(..., package.seeall)

local metat = { __index = {} }

npc = quests.questEngine.newNPC("Ventin", {})

npc.states = {}
npc.states["init"] =
{
    name = "init",
    
    description = "Initial state for Ventin.",

    input = {
        ["click"] = {
            name = "click",
            logic = function(npc, player)
                npc:transitionTo(player, "drink1")
            end
        }
        
    },

    onTransition = {
        -- Transition from init to stage1
        ["drink1"] = function(npc, player)
        end
    },

    onEnter = function(npc, player)
    end,

    onExit = function(npc, player)
    end
}

npc.states["drink1"] =
{
    name = "drink1",
    
    description = "Ventin asks for first drink",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player) 
                npc:say(player, "... fine, be that way.")
                npc:transitionTo(player, "init")
            end
        },
        ["give ale"] = {
            name = "give ale",
            logic = function(npc, player)
                player:give("Ale", npc)
                npc:say(player, "Thanks.")
                npc:emote(npc.getName() .. " drinks the ale.")
                npc:say(player, "Ugh... this is hardly more than tap water!  No, no, this will not do.  I need a stronger drink before I can speak of my glory days.")
                npc:transitionTo(player, "init")
            end
        },
        ["give stout ale"] = {
            name = "give stout ale",
            logic = function(npc, player) 
                player:give("Stout Ale", npc)
                npc:emote(npc.getName() .. " drinks the ale.")
                npc:say(player, "Ahh, now that has got some fire in it!  I will keep my end of the bargain.")
                npc:transitionTo(player, "story1")
            end
        }
    },

    onTransition = {
        -- Transition from stage1 to exit
        ["init"] = function(npc, player)
        end,
        ["story1"] = function(npc, player)
        end

    },

    onEnter = function(npc, player)
        npc:say(player, "I can tell by the look on you that you are in the adventuring business. You cannot tell now, but five years ago, I was the best fencer in Lasphar! Bring this broken man a drink and you might hear something of it. Maybe you will even learn something to keep you safe in your travels. ")
        npc:option(player, "I am not one to spend my good gold on a washed-up fencer.", "exit")

        if player:itemCount("Ale") > 0 then
            npc:option(player, "I have brought you a drink, as you asked (Give Ale).", "give ale")
        end

        if player:itemCount("Stout Ale") > 0 then
            npc:option(player, "I have brought you a drink, as you asked (Give Stout Ale).", "give stout ale")
        end

        npc:option(player, "Exit", "exit")
    end

}

npc.states["story1"] =
{
    name = "story1",
    
    description = "Ventin just got his hard drink and is now telling his story.",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "init2")
            end
        },
        ["continue"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "story2")
            end
        }
    },
    
    onEnter = function(npc, player)
        npc:say(player, "Used to be that I was one of the best swordsmen around, and easily the most eager. The good folks of Lasphar, they hired me to rid them of all sorts of things. Have you heard much of the wolves around here?")

        npc:option(player, "Continue", "continue")
        npc:option(player, "Exit", "exit")
    end
}

npc.states["story2"] =
{
    name = "story2",
    
    description = "Ventin just got his hard drink and is now telling his story.",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "init2")
            end
        },
        ["continue"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "story3")
            end
        }
    },
    
    onEnter = function(npc, player)
        npc:say(player, "That was my first job. The wolves of the Lasphar region, people say there’s something about them… Their lot is bigger and more vicious than I’ve seen anywhere else. Smarter, too!")

        npc:option(player, "Continue", "continue")
        npc:option(player, "Exit", "exit")
    end
}

npc.states["story3"] =
{
    name = "story3",
    
    description = "Ventin just got his hard drink and is now telling his story.",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "init2")
            end
        },
        ["continue"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "story4")
            end
        }
    },
    
    onEnter = function(npc, player)
        npc:say(player, "Not even the most clever of  traps can catch the alpha wolves, so the farmers put their money together every so often and hire someone like us to take care of them.")

        npc:option(player, "Continue", "continue")
        npc:option(player, "Exit", "exit")
    end
}

npc.states["story4"] =
{
    name = "story4",
    
    description = "Ventin just got his hard drink and is now telling his story.",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "init2")
            end
        }
    },
    
    onEnter = function(npc, player)
        npc:say(player, "Keep your ears keen, and you may hear someone looking for a wolfkiller… And when you’re out in the wilderness, remember to bring a healing potion or two – those beasts bite deep!")
        npc:option(player, "Exit", "exit")
    end
}

npc.states["init2"] =
{
    name = "init2",
    
    description = "Ventin is idle",

    input = {
        ["click"] = {
            name = "click",
            logic = function(npc, player)
                npc:transitionTo(player, "drink2")
            end
        }        
    },

    onTransition = {
        -- Transition from init to stage1
        ["drink1"] = function(npc, player)
        end
    },

    onEnter = function(npc, player)
    end,

    onExit = function(npc, player)
    end
}

npc.states["drink2"] =
{
    name = "drink2",
    
    description = "Ventin asks for his second drink",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player) 
                npc:say(player, "... fine, be that way.")
                npc:transitionTo(player, "init2")
            end
        },
        ["give ale"] = {
            name = "give ale",
            logic = function(npc, player)
                player:give("Ale", npc)
                npc:say(player, "Thanks.")
                npc:emote(npc.getName() .. " drinks the ale.")
                npc:say(player, "Look, friend. You cannot expect me to be generous with my words if you are stingy with your coin. Bring something stronger if you want to hear what I have to say.")
                npc:transitionTo(player, "init2")
            end
        },
        ["give stout ale"] = {
            name = "give stout ale",
            logic = function(npc, player) 
                player:give("Stout Ale", npc)
                npc:emote(npc.getName() .. " drinks the ale.")
                npc:say(player, "Thats the stuff!  Well then... (to be continued)")
                npc:transitionTo(player, "init2") -- todo transition to story5
            end
        }
    },
    
    onEnter = function(npc, player)
        npc:say(player, "So you have come back, have you? Buy me another drink and I will tell you more of my hunts. ")
        npc:option(player, "I am not interested in indulging your habit.", "exit")

        if player:itemCount("Ale") > 0 then
            npc:option(player, "I have brought you a drink, as you asked (Give Ale).", "give ale")
        end

        if player:itemCount("Stout Ale") > 0 then
            npc:option(player, "I have brought you a drink, as you asked (Give Stout Ale).", "give stout ale")
        end

        npc:option(player, "Exit", "exit")
    end

}

npc.states["story5"] =
{
    name = "story5",
    
    description = "Ventin just got his hard drink and is now telling his story.",

    input = {
        ["exit"] = {
            name = "exit",
            logic = function(npc, player)
                npc:transitionTo(player, "init2")
            end
        }
    },

    onEnter = function(npc, player)
        npc:say(player, "Keep your ears keen, and you may hear someone looking for a wolfkiller… And when you’re out in the wilderness, remember to bring a healing potion or two – those beasts bite deep!")
        npc:option(player, "Exit", "exit")
    end
}

