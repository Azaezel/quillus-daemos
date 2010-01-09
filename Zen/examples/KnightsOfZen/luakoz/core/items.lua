require "core.playerstats"
require "core.skills"
require "core.util"

-- slot defs
headSlot = {1}
bodySlot = {2}
legsSlot = {3}
ArmsSlot = {4,5}
NeckSlot = {6}
RingSlot = {7,8,9,10}

-- ==================================== ITEM DEFS
ItemDef = polymorphTable:new()

function ItemDef:Initialise(vAllowedslot,vReq,vAutoeffect,vTriggeredEffect,vName,vDesc)
	self.allowedSlots = vAllowedslot									-- for placement
	self.requirement = vReq											-- double value list for verrifying player.stat[odd]>=even
	self.autoeffect = vAutoeffect										-- skill table key, skill multiplier (skills being passive, or active
	self.triggeredeffect = vTriggeredEffect
	self.name = vName												-- simple name
	self.description = vDesc										-- simple desc
end


Helmet = ItemDef:new()
Helmet:Initialise(
	headSlot,										
	{str = 10, lvl = 1},
	{1,1},
	nil,
	'Helmet',
	'This rusted, dented, hunk of metals saved your life more than once.'
)

Thong = ItemDef:new()
Thong:Initialise(
	legsSlot,
	{char = 10},
	{1,1},
	nil,
	'Thong',
	'You honestly didn\'t want to keep this, but it \*was\* a present...'
)

Shotgun = ItemDef:new()
Shotgun:Initialise(
	ArmsSlot,
	{wis = 0},
	nil,
	{3,1},
	'Shotgun',
	'The old medicine man said you shake it at things, and they die. Not real sure how it works though...'
)

BFS9000 = ItemDef:new()
BFS9000:Initialise(
	ArmsSlot,
	{str = 15},
	nil,
	{2,10},
	'BFS9000',
	'This massive boradsword\'s seen better days. Truthfully, It more resembles a bar of steel than a weapon.'
)


sparklyGem = ItemDef:new()
sparklyGem:Initialise(
	NeckSlot,
	{str = 15},
	{4,1},
	nil,
	'sparklyGem',
	'You\'re not quite sure just what this necklace does, really...'
)

decoderRing = ItemDef:new()
decoderRing:Initialise(
	RingSlot,
	{str = 15},
	{6,2},
	nil,
	'Decoder Ring',
	'Decoder Ring'
)

-- ==================================== ITEM LIST - this is a singular lookup structure, no instancing needed
ItemList = {
	[1] = Helmet, 													-- key, item
	[2] = Thong,
	[3] = Shotgun,
	[4] = BFS9000,
	[5] = sparklyGem
}

-- ==================================== ITEM FUNCS
function ItemList:getItemDescription(vID)
	if (self[vID] ~= nil) then
		return self[vID].description
	end
end

function ItemList:getItemName(vID)
	if (self[vID] ~= nil) then
		return self[vID].name
	end
end

function ItemList:getItemRequirements(vID)
	if (self[vID] ~= nil) then
		return self[vID].requirement
	end
end

function ItemList:getItemAllowedSlots(vID)
	if (self[vID] ~= nil) then
		return self[vID].allowedSlots
	end
end

-- ====================================== ITEM INSTANCES
itemInst = polymorphTable:new()