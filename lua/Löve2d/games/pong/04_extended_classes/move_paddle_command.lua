local CommandInterface = require "command_interface"
local MovePaddleCommand = CommandInterface:extend()

function MovePaddleCommand:init(paddle, paddle_speed)
    self.parent.init(self)
    self.paddle = paddle
    self.paddle_speed = paddle_speed
end

function MovePaddleCommand:execute()
end

return MovePaddleCommand
