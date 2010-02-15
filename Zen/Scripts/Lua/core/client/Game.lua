-- core.client.Game
require("core.common.StateMachine")
require("core.client.SplashScreen")

function createGame()
    local game = initStateMachine("game");
    local private = {}

    -- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    -- init
    --
    local init = game:newState("init");

    init.onEnter = function() gameClient:initialize() end;
    
    return game;
end


