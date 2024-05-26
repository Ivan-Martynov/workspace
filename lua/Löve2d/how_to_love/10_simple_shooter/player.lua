local GameObject = require "game_object"

local Player = GameObject:extend()

function Player:new()
    Player.super.new(self, "panda.png", 300, 20, 500)
end

function Player:update(dt)
    if love.keyboard.isDown("left") then
        self.x = self.x - self.speed * dt
    elseif love.keyboard.isDown("right") then
        self.x = self.x + self.speed * dt
    end

    self.x = math.max(0, self.x)
    self.x = math.min(self.x, love.graphics.getWidth() - self.width)
end

return Player
