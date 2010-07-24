

serverServ = 
{
    commands = {}    
}

serverServ.defaultHandler = function(connection, words, part2)
	if (serverServ.commands[words[2]] ~= nil) then
		serverServ.commands[words[2]].handler(connection, words, part2);
	else
		print("Unable to handle server command " .. words[2]);
	end
end

serverServ.commands["SERVER"] =
{
        
    -- If hub is identifying itself:
    --  words[1] = "SERVER"
    --  words[2] = server name
    --  words[3] = hop count
    --  words[4] = description
    handler = function(connection, words)
        -- TODO Get configuration from N line
        -- N:services.indiezen.net:Hybrid services
        bot.servicesName = "services.indiezen.net";
        bot.servicesInfo = "Hybrid services"
        --connection.send(":" .. words[2] .. " SERVER " .. bot.servicesName .. " 1 :" .. bot.servicesInfo);
        print(connection.getFullString())
    end
}

local firstPing = true;

serverServ.commands["PING"] =
{
    handler = function(connection, words)
		local debugString = ""
		for i,v in ipairs(words) do
			debugString = debugString .. "/" .. v;
		end
		print("PING " .. debugString);
        connection.send("PONG " .. words[2])
		
		if (firstPing) then
			registerServices();
			firstPing = false;
		end		
    end
}

serverServ.commands["PASS"] =
{
	handler = function(connection, words)
		-- TODO Handle PASS commands
		print(connection.getFullString())
	end
}

serverServ.commands["CAPAB"] =
{
	handler = function(connection, words)
		-- TODO Handle CAPAB commands
		print(connection.getFullString())
	end
}

serverServ.commands["SVINFO"] =
{
	handler = function(connection, words)
		-- TODO Handle SVINFO commands
		print(connection.getFullString())
	end
}

serverServ.commands["NICK"] =
{
	handler = function(connection, words)
		-- TODO Handle NICK commands
		--print("Got Nick " .. words[2]);
		--print(connection.getFullString())
	end
}

local userRegistry = {}
local channelRegistry = {}

serverServ.commands["SJOIN"] =
{
	handler = function(connection, words, part2)
		-- Make sure the channel exists
		local channelName = words[4];
		local channel = channelRegistry[channelName];
		if channel == nil then
			channel = 
			{
				channelKey = words[3],
				channelName = channelName,
				-- list of all users including ops
				users = {}, 
				-- list of ops, used to re-op
				operators = {} 
			}

			channelRegistry[channelName] = channel;
		end

		print("SJOIN part2: " .. part2);
		for user in string.gfind(part2, "%S+") do
			local nick;
			local userMode = string.sub(user, 1, 1)
			if (userMode == "@") then
				nick = string.sub(user, 2);
			elseif (userMode == "%") then
				nick = string.sub(user, 2);
			else
				userMode = "";
				nick = user;
			end
			local user = userRegistry[nick];
			if (user == nil) then 
				user =  
				{
					nick = nick;
				}
				
				userRegistry[nick] = user;
			end
			
			if (userMode == "@") then
				table.insert(channel.operators, user);
			end
			
			table.insert(channel.users, user)         
		end

		table.save(serverRegistry, "channelRegistry.lua");
		--print(connection.getFullString())
	end
}

serverServ.commands["KILL"] =
{
	handler = function(connection, words, part2)
		print(connection.getFullString())
	end
}

local noticeCount = 0;

serverServ.commands["NOTICE"] =
{

	handler = function(connection, words, part2)
		print(connection.getFullString());
		
		-- On the fourth NOTICE, register the server
		noticeCount = noticeCount + 1;
		if (noticeCount == 4) then
			registerServer();
		end
	end
}

ircServices["ServerServ"] = serverServ;
