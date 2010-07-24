package.path=package.path..";scripts/?.lua"

print("=============\nLoading Tutorial4 initialization scripts...\n============");

print("Loading core.Environment");
require("core.Environment");

print("Loading client.GameClient");
require("client.GameClient");

-- Debug
require "core.debugHelp";

-- Turn on tracing
DebugInfo.trace = true;

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
-- Create a game client of the specified type
-- Note!  Most script types are not available until after this point.
print("=============\nCreating Game Client...\n============")
gameClient = GameClient:new(environment["arg2"])

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
-- Initialize the main taBBall Game
print("=============\nInitializing Tutorial 4 Client...\n============")
gameClient:initialize()
