local Entity = require "entity"

local Box = Entity:extend()

function Box:new(x, y)
    Box.super.new(self, x, y, "box.png")
end

return Box
