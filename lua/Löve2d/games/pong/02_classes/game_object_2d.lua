-- Simple 2d object class, having x, y coordinates.
local GameObject2d = {}
GameObject2d.__index = GameObject2d

-- Initialize the coordinates.
function GameObject2d:init(x, y)
    self.x = x or 0
    self.y = y or 0
end

-- Create new object.
function GameObject2d.new(x, y)
    local instance = setmetatable({}, GameObject2d)
    instance:init(x, y)
    return instance
end

-- Function to update an object, usually having a time parameter (dt).
function GameObject2d:update(_)
end

-- Function to draw an object, optionally having mode parameter.
function GameObject2d:draw(_)
end

return GameObject2d
