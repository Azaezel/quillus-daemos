-- taken from example 2 of http://lua-users.org/wiki/InheritanceTutorial for further refference

function new( base_object, clone_object )
  if type( base_object ) ~= "table" then
    return clone_object or base_object 
  end
  clone_object = clone_object or {}
  clone_object.__index = base_object
  return setmetatable(clone_object, clone_object)
end

function isa( clone_object, base_object )
  local clone_object_type = type(clone_object)
  local base_object_type = type(base_object)
  if clone_object_type ~= "table" and base_object_type ~= table then
    return clone_object_type == base_object_type
  end
  local index = clone_object.__index
  local _isa = index == base_object
  while not _isa and index ~= nil do
    index = index.__index
    _isa = index == base_object
  end
  return _isa
end

polymorphTable = {}
polymorphTable = new( table, { new = new, isa = isa } )

function polymorphTable:instantiate(vOwner,vDescriptor)
	key = self
	val = vDescriptor
	if (self[vOwner]~= nil) then
		self[vOwner][key] = val
	else
		self[vOwner] = {}
		self[vOwner][key] = val
	end
	self.descriptor = vDescriptor
end


function polymorphTable:dump()
--todo: add dump-to-file as well
	for key,val in pairs(o) do
		print(key,val)
	end
end