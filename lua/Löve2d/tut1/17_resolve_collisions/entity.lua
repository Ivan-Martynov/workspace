local Object = require "classic"

local Entity = Object:extend()

function Entity:new(x, y, image_path)
    self.x = x
    self.y = y
    self.image = love.graphics.newImage(image_path)
    self.width = self.image:getWidth()
    self.height = self.image:getHeight()

    self.last = { x = self.x, y = self.y }

    self.strength = 0
    self.temp_strength = self.strength
end

function Entity:update(dt)
    self.last.x = self.x
    self.last.y = self.y

    self.temp_strength = self.strength
end

function Entity:draw()
    love.graphics.draw(self.image, self.x, self.y)
end

function Entity:check_collision(other)
    return (self.x + self.width > other.x)
        and (self.x < other.x + other.width)
        and (self.y + self.height > other.y)
        and (self.y < other.y + other.height)
end

function Entity:was_vertically_aligned(other)
    return (self.last.y + self.height > other.last.y)
        and (self.last.y < other.last.y + other.height)
end

function Entity:was_horizontally_aligned(other)
    return (self.last.x + self.width > other.last.x)
        and (self.last.x < other.last.x + other.width)
end

function Entity:resolve_collision(other)
    if self.temp_strength > other.temp_strength then
        return other:resolve_collision(self)
        --return
    end
    if self:check_collision(other) then
        self.temp_strength = other.temp_strength

        if self:was_vertically_aligned(other) then
            if self.x + self.width / 2 < other.x + other.width / 2 then
                local pushback = self.x + self.width - other.x
                self.x = self.x - pushback
            else
                local pushback = other.x + other.width - self.x
                self.x = self.x + pushback
            end
        elseif self:was_horizontally_aligned(other) then
            if self.y + self.height / 2 < other.y + other.height / 2 then
                local pushback = self.y + self.height - other.y
                self.y = self.y - pushback
            else
                local pushback = other.y + other.height - self.y
                self.y = self.y + pushback
            end
        end

        return true
    end

    return false
end

return Entity
