local Game_object_2d = {}
Game_object_2d.__index = Game_object_2d

function Game_object_2d:init(x, y)
    self.x = x or 0
    self.y = y or 0
end

function Game_object_2d.new(x, y)
    local instance = setmetatable({}, Game_object_2d)
    instance:init(x, y)
    return instance
end

function Game_object_2d:update(_)
end

function Game_object_2d:draw(_)
end

return Game_object_2d
