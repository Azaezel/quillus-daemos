

function reload(packageName)
    package.loaded[packageName] = nil;
    require(packageName);
    
    if package.loaded[packageName] ~= nil then
        print("Reloaded " .. packageName);
    else
        print("Error reloading " .. packageName);
    end
end

local channels = {}

local function logHandler()
    local text = bot.connection.getText();

    -- Parse the message 
    local first, last, nick = string.find(bot.connection.getCmd(1), "(.-)!")
    --local first, last, part1, part2 = string.find(bot.connection.getText(), "^(.-):(.-)$")
    local channel = bot.connection.getChannel();
    local nick;

    if first ~= nil then
        if part1 == bot.nickname then
            first, last, nick = string.find(bot.connection.getCmd(1), "(.-)!")
            if nick == nil then 
                print("Error parsing nick")
            else
                if channel == nickname then
                    channel = nick;
                end
            end
        end
    end
    
    --print("first: " .. (first or ""));
    --print("last: " .. (last or ""));
    --print("nick: " .. (nick or ""));
    
    -- Skip non-channel messages
    if (channel == nil) then
        return;
    end
    
    function getLogFileName()
        local fixedChannel = channel;
        
        -- If on Windows, chomp the "#" character
        if (os.getenv("OS") == "Windows_NT") then
            fixedChannel = string.sub(channel, 2);
        else
            fixedChannel = channel .. ".";
        end
        return bot.logDirectory .. channel .. "." .. os.date("%Y%m%d") .. ".log";
    end

    local logChannelData = channels[channel]

    -- Check to see if this is the first time we've seen the channel.
    if (logChannelData == nil) then
        logChannelData = 
        {
            channel = channel;
        }
        channels[channel] = logChannelData;
    end

    -- Always recompute the file name
    logChannelData.fileName = getLogFileName();

    -- Open the file
    local file = io.open(logChannelData.fileName, "a");

    if (file ~= nil) then
    
        local message = os.date("%Y-%m-%dT%X  ");
        
        first, last, action = string.find(text, "^\001ACTION (.-)\001$");
        
        if (action ~= nil) then
            message = message .. "* " .. nick .. " " .. action;
        else
            message = message .. "<" .. nick .. "> " .. text;
        end
        
        -- Write the data
        file:write(message .. "\n");

        -- Close the file.
        -- Opening and closing the file each time is less efficient, but 
        -- that keeps us from having too many files open, and it
        -- prevents the file from being locked all of the time.
        file:close();
    else
        print("Error opening file " .. logChannelData.fileName);
    end
end

bot.connection.setLogHandler(logHandler);
