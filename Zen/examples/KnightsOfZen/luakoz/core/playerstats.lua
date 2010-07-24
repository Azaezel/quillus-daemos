require "core.util"

-- baseline stats: permanent, modifier stack where modstack[scourceid] = value so that when scourceid is removed/times out, value can be popped off

basestats = polymorphTable:new()
basestats.str = {10, modstack = {}}
basestats.con = {10, modstack = {}}
basestats.dex = {10, modstack = {}}
basestats.int = {10, modstack = {}}
basestats.wis = {10, modstack = {}}
basestats.char = {10, modstack = {}}
basestats.lvl = {1, modstack = {}}
basestats.def = {0, modstack = {}}
basestats.hp = {0, modstack = {}}
basestats.mp = {0, modstack = {}}

-- baseline player, holds functionality, and recieves hooks for substructures like inventory and stats

baseplayer = polymorphTable:new()

function baseplayer:getStatTotal(vStat)
	if (self.stats[vStat] ~= nil) then
		stat = self.stats[vStat]
		total = stat[1]
		-- language specific :/ itterates through entries reguarless of key num
		for key,val in pairs(stat.modstack) do 
			total = total + val
		end
		return total
	else
		print(vStat, "is not a valid player stat")
	end
end


-- oneoff implementation. Global atm.
player = baseplayer:new()
player.stats = basestats:new()
player.stats.str.modstack = {0,1, 1,5, 2,-10}

function player:test(stat)
	print(player:getStatTotal(tostring(stat)))
	print("end stat test")
end