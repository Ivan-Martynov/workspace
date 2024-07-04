local GameObject2d = require "structures.game_object_2d"

-- Make a class for movable objects from GameObject2d class.
local MovableObject2d = GameObject2d:extend()

-- Set object's velocity.
function MovableObject2d:set_velocity(dx, dy)
    self.dx = dx
    self.dy = dy
end

-- Initialize the object with the help of the parent class.
function MovableObject2d:init(x, y, dx, dy, color)
    GameObject2d.init(self, x, y, color)
    self:set_velocity(dx or 0, dy or 0)
end

-- Default function to update an object.
function MovableObject2d:update(dt)
    self.x = self.x + self.dx * dt
    self.y = self.y + self.dy * dt
end

return MovableObject2d
