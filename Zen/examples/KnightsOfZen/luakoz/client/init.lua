package.path=package.path..";\\luakoz\\?.lua"

-- varius tables
require "core.inventory"
require "core.metamap"
require "core.playerstats"
require "core.skills"

-- echo command line arguments

if (tonumber(arg[1]) == 1) then
player:test(arg[2])
end

if (tonumber(arg[1]) == 2) then
player.inventory:descriptiontest(tonumber(arg[2]))
end

if (tonumber(arg[1]) == 3) then
player.inventory:equip_test(tonumber(arg[2]),tonumber(arg[3]))
end

if (tonumber(arg[1]) == 4) then
player.inventory:test()
end
