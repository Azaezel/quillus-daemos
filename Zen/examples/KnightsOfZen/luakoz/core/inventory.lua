require "core.items"
require "core.util"

-- ==================================== INVENTORY
baselineInventory = polymorphTable:new()
baselineInventory.wearsize = 10
baselineInventory.bagsize = 20

function baselineInventory:getInventoryItem(vID)
	if (self[vID] ~= nil) then 
		return self[vID]
	end
end

-- ======================================= DATA ACCESS FUNCS ==== (the stuff you over-ride if using a different input setup, like a db)
function baselineInventory:swap(vSlotFrom,vSlotTo) -- no native swap func. something to look into...
	t = self[vSlotFrom]
	self[vSlotFrom] = self[vSlotTo]
	self[vSlotTo] = t
end

-- simple minimum stat lookup for now. shift to a proper string holding actual code evaluation later
function baselineInventory:evalRequirement(vPlayer,vID)
	req = ItemList:getItemRequirements(vID)
	passed = true
	if (req ~= nil) then
		for key, val in pairs(req) do 
			if (vPlayer:getStatTotal(key) < val) then
				passed = false
			end
		end
	end
	return passed
end

function baselineInventory:evalSlot(vID,vSlotTo,vItter)
	req = ItemList:getItemAllowedSlots(vID)
	if (req ~= nil) then
		if (req[vItter] == vSlotTo) then
			passed = true
			return passed
		else
			passed = false
			if (req[vItter+1] ~= nil) then 
				passed = self:evalSlot(vID,vSlotTo,vItter+1)
				return passed
			end
		end
	end
end

-- this version's got quite a bit of errorchecking... refactor for simplicty?
function baselineInventory:validswap(vPlayer,vSlotFrom,vSlotTo)
	-- if all were looking for is moving things arroud in our bag, we really could care less what the reqs are...
	if ((vSlotFrom>self.wearsize) and (vSlotFrom<self.bagsize)) then -- are we removing it from our bag?
		if ((vSlotTo>self.wearsize) and (vSlotTo<self.bagsize)) then	-- how about to it?
			self:swap(vSlotFrom,vSlotTo)
			return true
		else	
			-- on the other hand, if we're equiping it, we need to check
			itemID = self:getInventoryItem(vSlotFrom)
			if (self:evalSlot(itemID,vSlotTo,1) == true) then -- slot first, since that's less lookup
				if (self:evalRequirement(vPlayer,itemID) == true) then
					self:swap(vSlotFrom,vSlotTo)
					return true
				end
			end
		end
	else
		-- could be we're rearanging our offhand item/jewlery, hence the checking 
		itemID = self:getInventoryItem(vSlotFrom)
		if (self:evalSlot(itemID,vSlotTo,1) == true) then 
			if (self:evalRequirement(vPlayer,itemID) == true) then
				self:swap(vSlotFrom,vSlotTo)
				return true
			end
		end
	end
	return false
end


-- ======================================= OUTPUT FUNCS ===== (likely replace these with guis ect later)
function baselineInventory:displayItemDescription(vID)
	if (self:getInventoryItem(vID) ~= nil) then
		itemID = self:getInventoryItem(vID)
		print(ItemList:getItemDescription(itemID))
	else
		print("Empty") -- todo: hunt down why ya can't get here
	end
end

function baselineInventory:displayItemName(vID)
	if (self:getInventoryItem(vID) ~= nil) then
		itemID = self:getInventoryItem(vID)
		print(ItemList:getItemName(itemID))
	else
		print("Empty") -- todo: hunt down why ya can't get here
	end
end




function baselineInventory:addItem(vPlayer,vID)
	count = 0
	for key, val in pairs(self) do 
		count = count + 1
	end
	newslot = count + self.wearsize
	if (newslot <= self.bagsize) then
		neoinst = itemInst:new()
		neoinst:instantiate(vPlayer,vID)
		self[newslot] = neoinst

		print("adding a new", ItemList:getItemName(self[newslot].descriptor), "to slot", newslot)

	end
end




-- ==================================== INVENTORY TESTS

-- oneoff implementation for hooking an instance into player. Global atm.
player.inventory = baselineInventory:new()
-- inventory reserved slots. All inventory entries are item IDs
player.inventory[1] = 1 --head
player.inventory[2] = 0 -- body
player.inventory[3] = 2 -- legs
player.inventory[4] = 3 -- leftArm
player.inventory[5] = 0 -- rightArm
-- inventory bag slots.
player.inventory[11] = 4



function baselineInventory:test()
	player.inventory:addItem(player,1)
end



function baselineInventory:descriptiontest(vInventorySlot)
	self:displayItemDescription(vInventorySlot)
	print("end inventory test")
end

function baselineInventory:equip_test(vInventorySlotFrom,vInventorySlotTo)
	if (self:validswap(player,vInventorySlotFrom,vInventorySlotTo) == true) then
		print("equipped:", ItemList:getItemName(self:getInventoryItem(vInventorySlotTo)))
	else
		print("you don't meet the requirements to equip that")
	end
	print("end inventory test")
end