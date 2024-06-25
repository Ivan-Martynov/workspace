-- Try using Command Pattern for paddle movement.
local CommandInterface = require "command_interface"
local MovePaddleCommand = CommandInterface:extend()

-- Need this to update the paddle.
local MovableObject2d = require "movable_object_2d"

-- Command object has paddle reference, its speed and dt in order to move the
-- paddle properly.
function MovePaddleCommand:init(paddle, paddle_speed, dt)
    self.parent.init(self)
    self.paddle = paddle
    self.paddle_speed = paddle_speed
    self.dt = dt
end

-- Move the paddle by setting its velocity and reseting it to zero after the
-- movement is complete.
function MovePaddleCommand:execute()
    self.paddle:set_velocity(0, self.paddle_speed)
    MovableObject2d.update(self.paddle, self.dt)
    self.paddle:set_velocity(0, 0)
end

return MovePaddleCommand
