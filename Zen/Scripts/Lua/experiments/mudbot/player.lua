
module(..., package.seeall)

function new(_connection, _nick)
    local connection = _connection
    local nick = _nick

    local coins = 10
    local inventory = {}

    local sendMessage = function(self, message)
        connection.sendMessage(self.name, message)
    end

    local getName = function(self)
        return self.name
    end

    local buy = function(self, object, price)
        if price <= coins then
            self:sendMessage("You purchased " .. object .. " for " .. price .. " coins.")
            coins = coins - price
            if inventory[object] == nil then
                inventory[object] = 1
            else
                inventory[object] = inventory[object] + 1
            end

            return true
        else
            return false
        end
    end
    
    local give = function(self, object, npc)
        if inventory[object] ~= nil and inventory[object] > 0 then
            self:sendMessage("You gave " .. object .. " to " .. npc.getName() .. ".")
            inventory[object] = inventory[object] - 1
            return true
        end
    end
    
    local getCoins = function(self)
        return coins
    end
    
    local itemCount = function(self, object)
        if inventory[object] ~= nil then
            return inventory[object]
        else
            return 0
        end
    end

    newPlayer = { 
        name = nick, 
        getName = getName, 
        sendMessage = sendMessage, 
        buy = buy,
        give = give,
        sayOptions = {},
        getCoins = getCoins,
        itemCount = itemCount
    }

    return newPlayer
end
