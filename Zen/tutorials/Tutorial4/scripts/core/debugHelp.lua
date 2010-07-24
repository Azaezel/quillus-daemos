
print("=============\nLoading Debug scripts...\n============")

DebugInfo = 
{
    nextIndex = 1;
    trace = false;
}

function trace(event, line)
    local info = debug.getinfo(2);
    
    if (info ~= nil) then
        DebugInfo[DebugInfo.nextIndex] = {}
        DebugInfo[DebugInfo.nextIndex].lastSourceFile = info.source;
        DebugInfo[DebugInfo.nextIndex].lastLine = line;
        DebugInfo[DebugInfo.nextIndex].functionName = info.name;
        DebugInfo[DebugInfo.nextIndex].traceback = debug.traceback();
        
        -- Only keep the last 5 lines.
        if (DebugInfo.nextIndex > 5) then
            DebugInfo[DebugInfo.nextIndex - 5] = nil;
        end
        DebugInfo.nextIndex = DebugInfo.nextIndex + 1;

        if (DebugInfo.trace) then
            local s = info.short_src
            print(s .. ":" .. line)
        end
    end

end

debug.sethook(trace, "l")

function errorHandler(message)
    debug.sethook();
    local info = DebugInfo[DebugInfo.nextIndex - 2]
    print("In errorHandler");
    print("Got an error in file" .. info.lastSourceFile .. " on line " .. info.lastLine);
    print("    in function " .. info.functionName);
    print(message);
    print(info.traceback);
    debug.sethook(trace, "l");
end
