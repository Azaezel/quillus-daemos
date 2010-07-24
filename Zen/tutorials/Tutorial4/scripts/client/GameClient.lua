print("=============\nLoading GameClient scripts...\n============")


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
    self:init();
    
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
    return eventService:getActionMap(actionMapName or "default")
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
function GameClient:createTerrain()
    local terrainService = self:getGame():getTerrainService(); 
    terrainService:setPhysicsZone(self.zone);
    
    local matXfm = Math.Matrix4();
    matXfm:setXYZRotation(0, 0, 0);
    matXfm:setPosition(0, 0, 0);
    
    local physicsConfig = Utility.Config( 
    {
        ["type"]          = "terrain",
        ["terrainType"]   = "heightfield",
		["fileName"]      = "terrain.raw",
		["width"]         = 2048,
		["widthSamples"]  = 512,
		["depth"]         = 2048,
		["depthSamples"]  = 512,
		["scale"]         = 1.0,
		["thickness"]     = 1.0,
		["wrap"]          = 1,
    });
    
    local renderingConfig = Utility.Config(
    {
        ["type"]          = "terrain",
        ["fileName"]      = "terrain.cfg",
    });
    
    self.terrain = terrainService:createTerrain(physicsConfig, renderingConfig);
    
    -- TODO set physics material and collision group?
    
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createSky()
    local skyService = self:getGame():getSkyService();
    
    local skyConfig = Utility.Config(
    {
        ["type"]         = "skybox",
        ["resourceName"] = "SteveCube",
    });
    
    self.sky = skyService:createSky(skyConfig);
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createSceneLighting()
    local sceneService = self:base():getSceneService();

    if (sceneService == nil) then
        print("Error getting scene service.");
    end
    
    sceneService:setAmbientLight(0.8, 0.8, 0.8, 1.0);
    
    self.lights = {};
    self.lights[1] = sceneService:createLight("default", "Light");
    self.lights[1]:setPosition(5000, 5000, 5000);
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:createScene()
    print("=============\nCreate Scene...\n============")
    local minSize = Math.Vector3(-2000.0, -4000.0, -2000.0);
    local maxSize = Math.Vector3( 2000.0,  2000.0,  2000.0);

    self.zone = self:getGame():getPhysicsService():createZone();
    self.zone:setBoundary(minSize, maxSize);

    self:getGame():setCurrentPhysicsZone(self.zone);

    -- Load the terrain
    self:createTerrain();

    -- Load the sky
    self:createSky();

    -- Create the sceen lighting    
    self:createSceneLighting();
    
    local player = self:getAvatarGameObject();
    
    player:loadRenderable("ninja.mesh", "");
    player:base():setScale(0.005, 0.005, 0.005);
    player:base():setPosition(146.0, 101.0, 221.0, true);
    
    local canvas = self:base():getRenderingCanvas();
    
    canvas:createCamera("chase");
    
    local camera = canvas:selectCamera("chase");
    
    camera:setNearClipDistance(0.01);
    camera:setFarClipDistance(99999*6);
    --camera:setAspectRatio(canvas:getWidth() / canvas:getHeight());
    camera:setAspectRatio(800/600);
    
    -- local fov = Math.Degree(60.0);
    local fov = Math.Radian(1.0471975512);
    camera:setHorizontalFOV(fov);

    local offset = Math.Vector3(0.0, 80.0, 0.0);
    local rotation = Math.Quaternion4(0.0, 0.0, 0.0, 1.0);   -- identity quaternion
    
    --player:base():getSceneNode():attachObject("camera", camera, offset, rotation);
    --camera:setPosition(0.0, 0.5, 2.0);
    camera:setPosition(145.0, 102, 220.0);
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function GameClient:testAction(action)
    if action:getPressedState() then
        print("Yay, the T button was pressed!!!")
    else
        print("Yay, the T button was released!!!")
    end

end
