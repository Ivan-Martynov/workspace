local Entity = require "entity"

local Wall = Entity:extend()

function Wall:new(x, y)
    Wall.super.new(self, x, y, "wall.png")
    self.strength = 100
end

return Wall
