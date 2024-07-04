local CommandInterface = require "commands.command_interface"
local MovableObject2d = require "structures.movable_object_2d"

local FlapCommand = CommandInterface:extend()

function FlapCommand:init(bee, speed, dt)
    self.parent.init(self)
    self.bee = bee
    self.speed = speed
    self.dt = dt
end

function FlapCommand:execute()
    self.bee:set_velocity(0, self.speed)
    self.bee.angle = self.speed < 0 and -math.pi / 20 or math.pi / 20
    MovableObject2d.update(self.bee, self.dt)
end

return FlapCommand
