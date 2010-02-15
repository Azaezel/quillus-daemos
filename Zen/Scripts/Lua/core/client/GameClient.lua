

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
	local o = Zen:createGameClient(type)
	setmetatable(getmetatable(o), self)
	self.__index = self
	return o
end

function GameClient:error(errorString)
    print("Error! " .. errorString);
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:initialize()
    print("=============\nInitializing Game Client...\n============")
    
    -- Validate plugins 
    if (self.plugins == nil) then
        self:error("You must specify GameClient.plugins before calling GameClient:initialize().");
        return;
    end

    if (self.config == nil) then
        self:error("You must specify GameClient.config before calling GameClient:initialize()");
        return;
    end

    if (self.plugins["Rendering"] ~= nil) then
        local windowTitle = self:getGameName();
        if self:isDebug() then
            windowTitle = windowTitle .. " - DEBUG";
        end
        self:initRenderingService(self.plugins["Rendering"], windowTitle, 
                self.config["Resolution"][1], self.config["Resolution"][2]);
    end

    if (self.plugins["Physics"] ~= nil) then
        self:initPhysicsService(self.plugins["Physics"]);
    end

    -- Just default this for now
    self:initInputService("keyboard");

    if (self.plugins["Widgets"] ~= nil) then
        self:initWidgetService(self.plugins["Widgets"]);
    end

    if (self.plugins["Navigation"] ~= nil) then
        self:initNavigationService(self.plugins["Navigation"]);
    end

    local resources = self.config["RenderingResources"];
    if resources ~= nil then
        for index, resource in ipairs(resources) do
            renderingResourceService:addResourceLocation(resource.path, resource.containerType, resource.group, resource.recursive);
        end
    end

	self.getActionMap = function (self) return self:getGame():getActionMap() end
	self.getRootGroup = function (self) return self:getGame():getRootGroup() end

	self:createActions();
	self:createDefaultMappings();
    
    -- Services are all loaded, let the C++ code have a chance to initialize itself
    self:init();
    

	rootGroup = self:getRootGroup();

	self:createScene();

end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createActions()
    print("=============\nCreate Actions...\n============")
	self:getActionMap():createAction("onTick", GameClient.onTick, self)
	self:getActionMap():createAction("onInitDone", GameClient.onInitDone, self)
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createDefaultMappings()
    print("=============\nCreate Default Key Mappings...\n============")
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createScene()
    print("=============\nCreate Scene...\n============")
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:onTick(timeDelta)
	game:fireEvent("clockTick", timeDelta)
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:onInitDone()
	game:fireEvent("onInitDone")
end

