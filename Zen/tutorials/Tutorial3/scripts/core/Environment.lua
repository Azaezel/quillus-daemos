--[[

environment.lua

Create a global variable called "environment" and add some array
accessor type methods.

]]
                        
-- Get the environment
environment = Zen:getEnvironment()

-- Set up environment with new methods for array type access
-- TODO Eventually this should be done in C++ somehow
local metaarray = getmetatable(environment)
metaarray.__index = environment.get
metaarray.__newindex = environment.set
