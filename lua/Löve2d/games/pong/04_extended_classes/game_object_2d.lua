local GameObject2d = {}
GameObject2d.__index = GameObject2d

-- Initialize object's coordinates and colour.
function GameObject2d:init(x, y, color)
    self.x = x or 0
    self.y = y or 0
    self.color = color or { 1, 1, 1, 1 }
end

-- Make the class "callable", that is using the class as a constructor.
function GameObject2d:__call(...)
    -- Set metatable to point to the class, giving access to its methods.
    local instance = setmetatable({}, self)
    -- Initialize the instance, using corresponding init function.
    instance:init(...)
    return instance
end

-- Function to update an object, usually having a time parameter (dt).
function GameObject2d:update(_)
end

-- Function to draw an object, optionally having mode parameter.
function GameObject2d:draw(_)
end

return GameObject2d
