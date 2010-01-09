local base = _G

module(...) --, package.seeall

local metat = { __index = {} }

function new(_name)
    local private = { name = _name }
    local getName = function()
        return private.name
    end
    newAvatar = { getName = getName }
    base.setmetatable(newAvatar, metat)
    --base.avatar.__index = base.avatar
    return newAvatar
end

function metat.__index:say(_text)
    base.print(self:getName() .. " says: " .. _text)
end

--[[
disgruntledAvatar = 
{
}

function disgruntledAvatar:rebase(_avatar)
    setmetatable(_avatar, self)
    self.__index = self
end

function disgruntledAvatar:say(_text)
    print(self.getName() .. " mutters: " .. _text)
end

p1 = avatar:new("Rhapsody")
--p1:say("Hello!")

disgruntledAvatar:rebase(p1)
--p1:say("Hello!")
--]]

