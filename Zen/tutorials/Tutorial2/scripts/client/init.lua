package.path=package.path..";scripts\\?.lua"

require "core.Environment"
require "client.GameClient"

                                     
-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
-- Create a game client of the specified type
-- Note!  Most script types are not available until after this point.
print("=============\nCreating Game Client...\n============")
gameClient = GameClient:new(environment["arg2"])

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
-- Initialize the main taBBall Game
print("=============\nInitializing taBBall Client...\n============")
gameClient:initialize()
