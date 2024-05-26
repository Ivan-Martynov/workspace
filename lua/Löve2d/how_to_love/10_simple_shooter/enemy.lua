local GameObject = require "game_object"

local Enemy = GameObject:extend()

function Enemy:new()
    Enemy.super.new(self, "snake.png", 325, 450, 100)
end

function Enemy:update(dt)
    self.x = self.x + self.speed * dt

    if self.x < 0 then
        self.x = 0
        self.speed = -self.speed
    else
        local border = love.graphics.getWidth() - self.width
        if self.x > border then
            self.x = border
            self.speed = -self.speed
        end
    end
end

return Enemy
