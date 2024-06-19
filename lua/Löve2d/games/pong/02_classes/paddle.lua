local Movable_object_2d = require "movable_object_2d"

local Paddle = setmetatable({}, Movable_object_2d)
Paddle.__index = Paddle

function Paddle.new(x, y, width, height)
    local instance = setmetatable(Movable_object_2d.new(x, y, 0, 0), Paddle)
    instance.width = width
    instance.height = height
    instance.score = 0
    return instance
end

function Paddle:draw(mode)
    love.graphics.rectangle(mode or "fill", self.x, self.y,
        self.width, self.height)
end

return Paddle
