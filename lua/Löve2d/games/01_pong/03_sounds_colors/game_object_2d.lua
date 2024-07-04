local GameObject2d = {}
GameObject2d.__index = GameObject2d

function GameObject2d:init(x, y, color)
    self.x = x or 0
    self.y = y or 0
    self.color = color or { 1, 1, 1, 1 }
end

function GameObject2d.new(x, y, color)
    local instance = setmetatable({}, GameObject2d)
    instance:init(x, y, color)
    return instance
end

function GameObject2d:update(_)
end

function GameObject2d:draw(_)
end

return GameObject2d
