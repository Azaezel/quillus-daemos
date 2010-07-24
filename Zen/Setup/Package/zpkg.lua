require("lfs") -- Install Lua for Windows or liblus5.1-filesystem0

--[[
t = svn.status(".")
for k, v in pairs (t) do
    print (k, v)
end

]]

subversionProjects =
{
    ["build"] = { repo="svn://svn.indiezen.org/zoss/build" },
    ["Community"] = { repo="svn://svn.indiezen.org/community" },
    ["Core"] = { repo="svn://svn.indiezen.org/zoss/Core" },
    ["Engine"] = { repo="svn://svn.indiezen.org/zoss/Engine" },
    ["Enterprise"] = { repo="svn://svn.indiezen.org/zoss/Enterprise" },
    ["EnterprisePlugins"] = { repo="svn://svn.indiezen.org/zoss/EnterprisePlugins" },
    ["examples"] = { repo="svn://svn.indiezen.org/zoss/examples" },
    ["plugins"] = { repo="svn://svn.indiezen.org/zoss/plugins" },
    ["Scripts"] = { repo="svn://svn.indiezen.org/zoss/Scripts" },
    --["Shaders"] = { repo="svn://svn.indiezen.org/zoss/Shaders" },
	["Spaces"] = { repo="svn://svn.indiezen.org/zoss/Spaces" },
	["Starter"] = { repo="svn://svn.indiezen.org/zoss/Starter" },
    ["Studio"] = { repo="svn://svn.indiezen.org/zoss/Studio" },
    ["StudioPlugins"] = { repo="svn://svn.indiezen.org/zoss/StudioPlugins" },
    --["tests"] = { repo="svn://svn.indiezen.org/zoss/tests" },
    ["tutorials"] = { repo="svn://svn.indiezen.org/zoss/tutorials" },
    ["Worlds"] = { repo="svn://svn.indiezen.org/worlds" },
}

function checkoutHandler(startArg)
    require("svn") -- If you don't have this, you need to download it.
    for project, info in pairs(subversionProjects) do
        print("Checking out " .. info.repo .. "/trunk")
        svn.checkout(info.repo .. "/trunk", project)
    end
    return 0
end

function switchHandler(startArg)
    local argsUsed = 1
    local repoSetup = arg[startArg + argsUsed]
    require("Setup.Package.zoss_" .. repoSetup) -- For branch info.

    for project, info in pairs(subversionProjects) do
        local branch = nil
        if subversionBranches[project] == "" then
            branch = "/trunk"
        else
            if (subversionBranches[project] == nil) then
                print("Error attempting to get branch for " .. project)
            else
                branch = "/branches/" .. subversionBranches[project]
            end
        end
        if (branch ~= nil) then
            os.execute("svn switch " .. info.repo .. branch .. " " .. project)
        end
    end
    return argsUsed
end


function exportHandler(startArg)
    local argsUsed = 2
    local repoSetup = arg[startArg + 1]
    local destination = arg[startArg + 2]

    if (repoSetup == nil) then
        print("Usage: zpkg.lua svn export <branch> <destination>");
        print("    <branch> is a zoss_<branch>.lua file");
        print("    <destination> is a destination directory to export the files");
        return argsUsed
    end

    require("Setup.Package.zoss_" .. repoSetup) -- For branch info.

    for project, info in pairs(subversionProjects) do
        local branch = nil
        if subversionBranches[project] == "" then
            branch = "/trunk"
        else
            if (subversionBranches[project] == nil) then
                print("Error attempting to get branch for " .. project)
            else
                branch = "/branches/" .. subversionBranches[project]
            end
        end
        if (branch ~= nil) then
            os.execute("svn export " .. info.repo .. branch .. " " .. destination .. "/" .. project)
        end
    end
    return argsUsed
end

function svnInfoHandler(startArg)
    for project, info in pairs(subversionProjects) do
        print("Giving info about " .. info.repo .. "/trunk")
        t = svn.status(project)
        for k, v in pairs (t) do
            print (k, v)
        end
    end
    return 0
end

svnHandlers =
{
    ["co"] = { func = checkoutHandler, help = "Check out IndieZen source code from SVN" },
    ["switch"] = { func = switchHandler, help = "Switch to a branch" },
    ["export"] = { func = exportHandler, help = "Export a branch" },
    ["info"] = { func = svnInfoHandler, help = "Give SVN information for each project" }
}

-- @param startArg is the index into arg[] of the "svn" command
function svnHandler(startArg)
    require("svn") -- If you don't have this, you need to download it.
    local argsUsed = 1

    if svnHandlers[arg[startArg + argsUsed]] ~= nil then
        argsUsed = argsUsed + svnHandlers[arg[startArg + argsUsed]].func(startArg + argsUsed)
    else
        print("Error, svn " .. arg[startArg + argsUsed] .. " is not a legal command.")
    end

    return argsUsed
end

function loadTemplate(fileName)
	local file = io.open(fileName,"r")
	if file == nil then
		print("Error opening " .. fileName)
		return nil;
	end
	template = file:read("*all")
	file:close()
	return template
end

function replaceAppFolder(fileName, templatePath, rootPath, dependenciesPath, libPath, relativePath)
	local template = loadTemplate(rootPath .. "\\" .. templatePath .. "\\" .. fileName)
    if template == nil then
        return nil;
    end
	local result = string.gsub(template, "%%AppFolder%%", rootPath)
	result = string.gsub(result, "%%Dependencies%%", dependenciesPath)
	result = string.gsub(result, "%%DependenciesLib%%", libPath)
	local file = io.open(rootPath .. "\\" .. relativePath .. "\\" .. fileName,"w")
	file:write(result)
	file:close()
end

function generateConfigHandler(startArg)
	local argsUsed = 1

	if arg[startArg + argsUsed] ~= nil then
		local rootPath = arg[startArg + argsUsed]

		replaceAppFolder("main_env_vars.vsprops", "Zen\\Setup\\Package\\config", rootPath, "c:\\dev\\dependencies", "c:\\dev\\lib", "Zen\\config");
		replaceAppFolder("zen_debug_dll.vsprops", "Zen\\Setup\\Package\\config", rootPath, "c:\\dev\\dependencies", "c:\\dev\\lib", "Zen\\config");
		replaceAppFolder("zen_debug_exe.vsprops", "Zen\\Setup\\Package\\config", rootPath, "c:\\dev\\dependencies", "c:\\dev\lib", "Zen\\config");
		replaceAppFolder("zen_release_dll.vsprops", "Zen\\Setup\\Package\\config", rootPath, "c:\\dev\\dependencies", "c:\\dev\\lib", "Zen\\config");
		replaceAppFolder("zen_release_exe.vsprops", "Zen\\Setup\\Package\\config", rootPath, "c:\\dev\\dependencies", "c:\\dev\\lib", "Zen\\config");
		replaceAppFolder("ogre_prefs_debug.vsprops", "Zen\\Setup\\Package\\config", rootPath, "c:\\dev\\dependencies", "c:\\dev\\lib", "Zen\\config");
		replaceAppFolder("ogre_prefs_release.vsprops", "Zen\\Setup\\Package\\config", rootPath, "c:\\dev\\dependencies", "c:\\dev\\lib", "Zen\\config");
	else
		print("Error in usage:  zpkg.lua config generate rootPath")
	end
	
	return argsUsed
end

configHandlers =
{
	["generate"] = { func = generateConfigHandler, help = "Generate configuration files" }
}

function configHandler(startArg)
	local argsUsed = 1

	if configHandlers[arg[startArg + argsUsed]] ~= nil then
		argsUsed = argsUsed + configHandlers[arg[startArg + argsUsed]].func(startArg + argsUsed)
	else
		print("Error, config " .. arg[startArg + argsUsed] .. " is not a legal command.")
	end
	
	return argsUsed
end

function generateSourceGroup(subdir)
	local groupName = string.gsub(subdir, "/", "\\\\")
	
	if subdir == "" then
		return ""
	end
	
	return [[
source_group(
    ]] .. groupName .. [[

	REGULAR_EXPRESSION 
	]] .. subdir .. [[*.*
	)

]]
end

-- returns the content to be written in the CMakeLists.txt
function installWriter(content)
   text = "project"
   -- Finds the Line Containing the project name 
   init_line = string.find(content, text)   
   init = string.find(content, "%(", init_line)
   eol = string.find(content, "%)", init_line)
   -- returns the substring refering to the project
   target = string.sub(content, init + 1,  eol - 1)
   -- Defines the Install Target CMake Directive
   install = "\ninstall ( TARGETS " .. target .. " RUNTIME DESTINATION bin LIBRARY DESTINATION lib64 ) "
   -- search if the install target wasn't added before (as if someone type cmake twice)

   return install

end

function headersDiggers(myStrRep, prjDir)
   -- this will slow down the script, but will organize the headers in a better way
   -- I swear the code was a lot more beautiful than this but everything was going to /usr/local/include only...
   print ("Digging the headers of: " .. prjDir)
   slice_list = "\n"
   install_files = "\n"
    
   -- separates the files which are in the base directory
   for str in string.gmatch(myStrRep, "%S*%.hpp") do
      if (string.match(str, "/") ~= "/") then
	 slice_list = slice_list .. str .. "\n" 
      end
   end
   
   if (slice_list ~= "\n") then
      install_files = install_files .. "\ninstall ( FILES " .. slice_list .. " DESTINATION include/Zen/" .. prjDir .. ") \n"
   end

   slice_list = "\n"
   -- populates an array of subdirs
   t= {}
   i = 1
   for str in string.gmatch(myStrRep, "%w*%/") do
      if  (str ~= t[i - 1]) then
	 t[i] = str
	 i = i + 1
      end
   end

   -- runs the array of subdirs searching for headers to be installed... this is slow...
   i = 1
   len = 0
   while (i <= #t) do
      for str in string.gmatch(myStrRep, t[i]) do
	 slice_init = string.find(myStrRep, "%W*" .. str , len)
	 slice_end = string.find(myStrRep, "%.%w*%c", slice_init)
	 slice = string.sub(myStrRep, slice_init, slice_end  + 3)
	 slice_list = slice_list .. slice
	 len = len  + string.len(slice)
      end
      install_files = install_files .. "\ninstall ( FILES " .. slice_list .. " DESTINATION include/Zen/" .. prjDir .. t[i] .. " ) \n"
      slice_list = "\n"
      i = i + 1
   end
   
   return install_files

end

function findSources(pathName, subdir)
	local sourceList = ""
	local groupList = ""

	subdir = subdir or ""
	
	groupList = groupList .. generateSourceGroup(subdir)
	
	for file in lfs.dir(pathName .. "/" .. subdir) do
		if file ~= "." and file ~= ".." and file ~= ".svn" then
			local fullFileName = pathName .. subdir .. file
			local attr = lfs.attributes (fullFileName)
			if attr.mode == "directory" then
				local newSourceList , newGroupList = findSources(pathName, subdir .. file .. "/")
				sourceList = sourceList .. newSourceList
				groupList = groupList .. newGroupList
			else
				if string.find(file, "%.hpp$") or string.find(file, "%.h$") or string.find(file, "%.cpp$") or string.find(file, "%.c$") then
					fileName = "    " .. subdir .. file
					sourceList = sourceList .. fileName .. "\n"
				end
			end
		end
	end

	return sourceList, groupList
end

function findHeaders(pathName, subdir)
    local headerList = ""
	local installDirectives = ""
    
    subdir = subdir or ""
    
    for file in lfs.dir(pathName .. "/" .. subdir) do
        if file ~= "." and file ~= ".." and file ~= ".svn" and file ~= "src" then
            local fullFileName = pathName .. subdir .. file;
            local attr = lfs.attributes (fullFileName)
            if attr.mode == "directory" then
                local newInstallDirectives = findHeaders(pathName, subdir .. file .. "/")
                installDirectives = installDirectives .. newInstallDirectives
            else
                if string.find(file, "%.hpp$") or string.find(file, "%.h$") or string.find(file, "%.xml$") or string.find(file, "%.exsd$") then
                    fileName = "    " .. subdir .. file
                    headerList = headerList .. fileName .. "\n"
               end
            end
        end
    end
    
	if headerList ~= "" then
		installDirectives = installDirectives .. "\ninstall ( FILES \n" .. headerList .. "    DESTINATION\n    Zen/${PROJECT_PREFIX}/" .. subdir .. "\n)\n"
	end
	
    return installDirectives
end

function replaceSourceList(pathName)
	local inputFileName = pathName .. "CMakeLists.template"
	local outputFileName = pathName .. "CMakeLists.txt"
	
	local template = loadTemplate(inputFileName)
	
	if (template == nil) then
		print("Error finding " .. inputFileName)
		return
	end
	
	local sourceList, groupList = findSources(pathName)
    
    local headerList = findHeaders(pathName)
	
	local result = string.gsub(template, "<%%SourceList%%>", sourceList)
	result = string.gsub(result, "<%%GroupList%%>", groupList)
    result = string.gsub(result, "<%%HeaderInstallDirectives%%>", headerList)

--[[    
    if (os.execute("uname -s") == "Linux") then    
        -- uses the include list instead of run into the subdirs and preserves the source of being installed as headers
        include_list = string.gsub(sourceList, "%S*src%S*%.%app\n%s*", "")
        install_target = installWriter(result)
        prjDir = string.gsub(pathName, "%S*Zen/", "")       
        install_files = headersDiggers(include_list, prjDir)
        result = result .. "\n" .. install_files .. "\n" .. install_target .. "\n"
    end
--]]

	local file = io.open(outputFileName, "w")
	file:write(result)
	file:close()
end

function generateBuildHandler(startArg)
	-- Assume the working directory contains "./Zen"
	local argsUsed = 1

	local rootPath = ""
	
	if arg[startArg + argsUsed] ~= nil then
		rootPath = arg[startArg + argsUsed]	
	else
		print("Error in usage:  build generate rootpath")
		return argsUsed
	end
	projects = { 
		["Core"] = {},
		["Engine"] = {},
		["Enterprise"] = {},
		
		["Starter/Base/BaseCommon"] = {},
		["Starter/Base/BaseClient"] = {},
		["Starter/Base/ZGameLoader"] = {},
		["Starter/Base/ZenServer"] = {},
		
		["Studio/Workbench"] = {},
		["Studio/WorkbenchCommon"] = {},
		["Studio/WorkbenchModel"] = {},
		["Studio/WorkbenchProtocol"] = {},
		["Studio/WorkbenchServer"] = {},
		["Studio/ZenStudio"] = {},

		["StudioPlugins/Workbench"] = {},
		
		["StudioPlugins/GameBuilder"] = {},
		["StudioPlugins/GameBuilderCommon"] = {},
		["StudioPlugins/GameBuilderModel"] = {},
		["StudioPlugins/GameBuilderProtocol"] = {},
		["StudioPlugins/GameBuilderServer"] = {},

		["StudioPlugins/WorldBuilder"] = {},
		["StudioPlugins/WorldBuilderCommon"] = {},
		["StudioPlugins/WorldBuilderModel"] = {},
		["StudioPlugins/WorldBuilderProtocol"] = {},
		["StudioPlugins/WorldBuilderServer"] = {},
        
        ["StudioPlugins/ArtLibrary"] = {},
        ["StudioPlugins/ArtLibraryCommon"] = {},
        ["StudioPlugins/ArtLibraryModel"] = {},
        ["StudioPlugins/ArtLibraryProtocol"] = {},
        ["StudioPlugins/ArtLibraryServer"] = {},

		["plugins/Framework"] = {},
        ["plugins/ZInput"] = {},
		["plugins/ZOgre"] = {},
		["plugins/ZCrazyEddie"] = {},
		["plugins/ZLua"] = {},
		["plugins/ZSQLite"] = {},
		["plugins/ZBoostNetworking"] = {},
		["plugins/ZNewton"] = {},
        ["plugins/ZTerrain"] = {},
        ["plugins/ZSky"] = {},
        ["plugins/ZODE"] = {},
        ["plugins/ZOpenAL"] = {},
        ["plugins/ZMicroPather"] = {},
        
        ["EnterprisePlugins/Account/AccountClient"] = {},
        ["EnterprisePlugins/Account/AccountServer"] = {},
        ["EnterprisePlugins/Account/AccountService"] = {},
        
        ["EnterprisePlugins/Permission/PermissionClient"] = {},
        ["EnterprisePlugins/Permission/PermissionServer"] = {},
        ["EnterprisePlugins/Permission/PermissionService"] = {},

        ["EnterprisePlugins/Session/SessionClient"] = {},
        ["EnterprisePlugins/Session/SessionServer"] = {},
        ["EnterprisePlugins/Session/SessionService"] = {},

		["Spaces/ObjectModel"] = {},

		["tests/EventTest"] = {},
		["tests/ScriptLoginTest"] = {},
        ["tests/ScriptTest"] = {},
        ["tests/StreamTest"] = {},
        
        ["tutorials/Tutorial1"] = {},
        ["tutorials/Tutorial2"] = {},
        ["tutorials/Tutorial3"] = {},
        ["tutorials/Tutorial4"] = {},
        ["tutorials/Tutorial5"] = {},

        ["Worlds/WorldClient"] = {},
        ["Worlds/WorldCommon"] = {},

        ["Worlds/ZoneCommon"] = {},
        ["Worlds/ZoneServer"] = {},
        ["Worlds/ZWGameClient"] = {},
        
--[[
        ["Worlds/WorldProtocol"] = {},
        ["Worlds/WorldServer"] = {},

        ["Worlds/ZoneClient"] = {},
        ["Worlds/ZoneProtocol"] = {},

        ["Worlds/ZoneDataClient"] = {},
        ["Worlds/ZoneDataCommon"] = {},
        ["Worlds/ZoneDataProtocol"] = {},
        ["Worlds/ZoneDataServer"] = {},
]]
        ["Community/AccountCommon"] = {},
        ["Community/AccountClient"] = {},
        ["Community/AccountProtocol"] = {},
        ["Community/AccountServer"] = {},
        ["Community/AccountModel"] = {},
        ["Community/SessionCommon"] = {},
        ["Community/SessionClient"] = {},
        ["Community/SessionProtocol"] = {},
        ["Community/SessionServer"] = {},
        ["Community/SessionModel"] = {},
        ["Community/ChatCommon"] = {},
		["Community/ChatClient"] = {},
        ["Community/ChatProtocol"] = {},
        ["Community/ChatServer"] = {},
        ["Community/ChatModel"] = {},

        }

	for projectName, project in pairs(projects) do
		pathName = rootPath .. "/Zen/" .. projectName .. "/"
		replaceSourceList(pathName)
	end
	return argsUsed
end

--[[
local function fixEclipseHandler()
function generateBuildHandler(startArg)
    -- First arg is build directory, second arg is source directory

	local argsUsed = 2;
    local buildDirectory = "";
    local sourceDirectory = "";

    -- Get buildDirectory from the first argument
    if arg[startArg + 1] ~= nil then
        buildDirectory =  arg[startArg + 1];
    else
        print("Usage: zpkg build fix <buildDir> <sourceDir>");
        return argsUsed;
    end
    
    -- Get sourceDirectory from the second argument
    if arg[startArg + 2] ~= nil then
        sourceDirectory =  arg[startArg + 2];
    else
        print("Usage: zpkg build fix <buildDir> <sourceDir>");
        return argsUsed;
    end

	local inputProject = buildDirectory .. "/.project";
    local inputCProject = buildDirectory .. "/.cproject";

	local outputProject = sourceDirectory .. "/project";
	local outputCProject = sourceDirectory .. "/cproject";
	
    -- Load the .project file
	local template = loadTemplate(inputProject)

	if (template == nil) then
		print("Error finding " .. inputProject)
		return argsUsed;
	end
	
	local result = string.gsub(template, buildDirectory .. "</name>", sourceDirectory .. "</name>");

	local file = io.open(outputProject, "w")
	file:write(result)
	file:close()

    -- Load the .cproject file
    template = loadTemplate(inputCProject);
	if (template == nil) then
		print("Error finding " .. inputCProject)
		return argsUsed;
	end

    return argsUsed;
end
]]

buildHandlers =
{
	["generate"] = { func = generateBuildHandler, help = "Generate configuration files" }
    --["fix"] = { func = fixEclipseHandler, help = "Fix Eclipse build files to work with version control" } 
}

function buildHandler(startArg)
	local argsUsed = 1
	if buildHandlers[arg[startArg + argsUsed]] ~= nil then
		argsUsed = argsUsed + buildHandlers[arg[startArg + argsUsed]].func(startArg + argsUsed)
	else
		print("Error, build " .. arg[startArg + argsUsed] .. " is not a legal command.")
	end
	
	return argsUsed
end

functionHandlers =
{
    ["svn"] = { func = svnHandler, help = "Misc subversion utilities" },
	["config"] = { func = configHandler, help = "Configuration setup" },
	["build"] = { func = buildHandler, help = "Build System setup" }
}

-- Parse the arguments
local argNum = 1
while arg[argNum] ~= nil do
    if functionHandlers[arg[argNum]] ~= nil then
        argNum = argNum  + functionHandlers[arg[argNum]].func(argNum)
    else
        break
    end
end
