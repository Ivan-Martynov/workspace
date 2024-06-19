local Game_object_2d = require "game_object_2d"

local Movable_object_2d = setmetatable({}, Game_object_2d)
Movable_object_2d.__index = Movable_object_2d

function Movable_object_2d:set_velocity(dx, dy)
    self.dx = dx
    self.dy = dy
end

function Movable_object_2d.new(x, y, dx, dy)
    local instance = setmetatable(Game_object_2d.new(x, y), Movable_object_2d)
    instance:set_velocity(dx or 0, dy or 0)
    return instance
end

function Movable_object_2d:update(dt)
    self.x = self.x + self.dx * dt
    self.y = self.y + self.dy * dt
end

return Movable_object_2d
