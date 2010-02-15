package.path=package.path..";scripts\\?.lua"

require "core.Environment"
require "client.GameClient"
                                     
-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
-- Create a game client of the specified type
-- Note!  Most script types are not available until after this point.
print("=============\nCreating Game Client...\n============")
gameClient = GameClient:new(environment["arg2"])

if (environment["arg5"] ==  "hosting") then 
	gameClient.setNetworkingRole("hosting", "0.0.0.0")
else
	if (environment["arg5"] ==  "connect_to") then
		gameClient.setNetworkingRole("hosting", environment["arg6"])
	end
end
-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
-- Initialize the main taBBall Game
 print("=============\nInitializing Client...\n============")
-- gameClient:initialize()
