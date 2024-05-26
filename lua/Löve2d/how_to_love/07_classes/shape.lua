local Object = require "classic"

local Shape = Object:extend()

function Shape:new(x, y)
    self.x = x
    self.y = y
    self.dx = 0
    self.dy = 0
end

function Shape:update(dt)
    self.x = self.x + self.dx * dt
    self.y = self.y + self.dy * dt
end

return Shape
