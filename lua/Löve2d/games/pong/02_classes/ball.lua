local Movable_object_2d = require "movable_object_2d"

local Ball = setmetatable({}, Movable_object_2d)
Ball.__index = Ball

function Ball.new(x, y, radius)
    local dx = love.math.random(150, 200)
    local dy = love.math.random(150, 200)

    if love.math.random(2) == 1 then
        dx = dx
    end

    if love.math.random(2) == 1 then
        dy = -dy
    end

    local instance = setmetatable(Movable_object_2d.new(x, y, dx, dy), Ball)
    instance.radius = radius

    return instance
end

function Ball:draw(mode)
    love.graphics.circle(mode or "fill", self.x, self.y, self.radius)
end

return Ball
