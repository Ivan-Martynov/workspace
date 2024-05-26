local GameObject = require "game_object"

local Bullet = GameObject:extend()

function Bullet:new(x, y)
    Bullet.super.new(self, "bullet.png", x, y, 700)
    self.used = false
end

function Bullet:update(dt)
    self.y = self.y + self.speed * dt
end

local function is_AABB_collision(a, b)
    return (a.x + a.width > b.x)
        and (a.x < b.x + b.width)
        and (a.y + a.height > b.y)
        and (a.y < b.y + b.height)
end

function Bullet:check_aabb_collision(object)
    return is_AABB_collision(self, object)
end

return Bullet

 