local Object = require "classic"

local GameObject = Object:extend()

function GameObject:new(image_name, x, y, speed)
    self.image = love.graphics.newImage(image_name)
    self.x = x
    self.y = y
    self.speed = speed
    self.width = self.image:getWidth()
    self.height = self.image:getWidth()
end

function GameObject:update(dt)
    --if love.keyboard.isDown("left") then
    --    self.x = self.x - self.speed * dt
    --elseif love.keyboard.isDown("right") then
    --    self.x = self.x + self.speed * dt
    --end

    --self.x = math.max(0, self.x)
    --self.x = math.min(self.x, love.graphics.getWidth() - self.width)
end

function GameObject:draw()
    love.graphics.draw(self.image, self.x, self.y)
end

return GameObject
