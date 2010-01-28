
source="C:\\dev\\stage\\ZOSS_src\\dev\\"
destination="C:\\dev\\stage\\ZOSS_headers\\dev\\"

dirs =  {
    "Starter\\Base\\BaseClient",
    "Starter\\Base\\BaseCommon",
    "Starter\\Base\\BaseServer",
}

for i, dir in ipairs(dirs) do
    os.execute("mkdir " .. destination .. "Zen\\" .. dir)
    os.execute("copy " .. source .. "Zen\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\" .. dir)    
end


studio = {
"Workbench", "WorkbenchCommon", "WorkbenchModel", "WorkbenchProtocol", "WorkbenchServer", "ZenStudio"
}

for i, dir in ipairs(studio) do
    os.execute("mkdir " .. destination .. "Zen\\Studio\\" .. dir)
    os.execute("copy " .. source .. "Zen\\Studio\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\Studio\\" .. dir)    
end

plugins = {
"BasicNetworking", "Framework", "ZBasicCommandLineParser", "ZBasicConsole", "ZBasicGameRules", "ZBoostNetworking", "ZCloud", "ZCrazyEddie", "ZFMOD", "ZInput", "ZLua", "ZMicroPather", "ZMySQL", "ZNewton", "ZOgre", "ZOpenAL", "ZPhysX", "ZPostgres", "ZPython", "ZRakNet", "ZSky", "ZSQLite", "ZTerrain", "ZWebServices" 
}

for i, dir in ipairs(plugins) do
    os.execute("mkdir " .. destination .. "Zen\\plugins\\" .. dir)
    os.execute("copy " .. source .. "Zen\\plugins\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\plugins\\" .. dir)    
end

studioPlugins = {
"ArtLibrary", "ArtLibraryCommon", "ArtLibraryModel", "ArtLibraryServer", "GameBuilder", "GameBuilderCommon", "GameBuilderModel", "GameBuilderProtocol", "GameBuilderServer", "Workbench", "WorldBuilder", "WorldBuilderCommon", "WorldBuilderModel", "WorldBuilderServer"
}

for i, dir in ipairs(studioPlugins) do
    os.execute("mkdir " .. destination .. "Zen\\StudioPlugins\\" .. dir)
    os.execute("copy " .. source .. "Zen\\StudioPlugins\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\StudioPlugins\\" .. dir)    
end

worlds = {
"WorldClient", "WorldCommon", "ZoneCommon", "ZoneDataCommon", "ZoneServer"
}

for i, dir in ipairs(worlds) do
    os.execute("mkdir " .. destination .. "Zen\\Worlds\\" .. dir)
    os.execute("copy " .. source .. "Zen\\Worlds\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\Worlds\\" .. dir)    
end

community = {
"ChatClient", "ChatCommon", "ChatProtocol", "ChatServer", "SessionClient", "SessionCommon", "SessionProtocol", "SessionServer"
}

for i, dir in ipairs(community) do
    os.execute("mkdir " .. destination .. "Zen\\Community\\" .. dir)
    os.execute("copy " .. source .. "Zen\\Community\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\Community\\" .. dir)    
end

core = {
"Event", "Math", "Memory", "Plugins", "Scripting", "Threading", "Utility"
}

for i, dir in ipairs(core) do
    os.execute("mkdir " .. destination .. "Zen\\Core\\" .. dir)
    os.execute("copy " .. source .. "Zen\\Core\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\Core\\" .. dir)    
end

engine = {
"Camera", "Cinematics", "Client", "Core", "Input", "Move", "Navigation", "Physics", "Rendering", "Resource", "Rules", "Server", "Sound", "Widgets", "World"
}

for i, dir in ipairs(engine) do
    os.execute("mkdir " .. destination .. "Zen\\Engine\\" .. dir)
    os.execute("copy " .. source .. "Zen\\Engine\\" .. dir .. "\\*.hpp " .. destination .. "Zen\\Engine\\" .. dir)    
end
