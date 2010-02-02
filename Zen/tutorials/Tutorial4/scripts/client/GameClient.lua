

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
GameClient =
{
	-- Put member variables for the game client here
}

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
-- new
-- allows you to overload the Zen:GameClient implementation with
-- additional methods and member variables
function GameClient:new(type)
    print("create")
	o = Zen:createGameClient(type)
	print("setmetatable")
	setmetatable(getmetatable(o), self)
	print("self.__index = self")
	self.__index = self
	print("return o")
	return o
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:initialize()
	rootGroup = self:getRootGroup()

	self:createActions()
	self:createDefaultMappings()
	self:createScene()

	self:run()
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:getRootGroup()
    -- This is in Game now, not GameClient
    return self:getGame():getRootGroup()
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:getActionMap(actionMapName)
    -- This is in Game now, not GameClient
    return self:getActionMap(actionMapName)
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createActions()
    print("=============\nCreate Actions...\n============")
    self:getActionMap():createAction("Test", GameClient.testAction, self)
    print("-------------\n")
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createDefaultMappings()
    print("=============\nCreate Default Key Mappings...\n============")
    self:getKeyMap():mapKeyInput("t", self:getActionMap():getAction("Test"))
    self:getKeyMap():mapKeyInput("a", self:getActionMap():getAction("Move Left"))
    self:getKeyMap():mapKeyInput("d", self:getActionMap():getAction("Move Right"))
    self:getKeyMap():mapKeyInput("w", self:getActionMap():getAction("Move Forward"))
    self:getKeyMap():mapKeyInput("s", self:getActionMap():getAction("Move Backward"))
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createScene()
    print("=============\nCreate Scene...\n============")
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:testAction(action)
    if action:getPressedState() then
        print("Yay, the T button was pressed!!!")
    else
        print("Yay, the T button was released!!!")
    end

end

