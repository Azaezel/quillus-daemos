
function create(baseObject, newObject)
	if type(baseObject) ~= "table" then
		return newObject or baseObject
	end
	
	newObject = newObject or {}
	newObject.__index = baseObject
	return setmetatable(newObject, newObject)
end
