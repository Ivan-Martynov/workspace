Rectangle = Object.extend(Object)

function Rectangle.new(self, x, y)
    self.x = x
    self.y = y
    self.width = 200
    self.height = 150
    self.speed = 100
end

function Rectangle:update(dt)
    self.x = self.x + self.speed * dt
end

function Rectangle.draw(self)
    love.graphics.rectangle("line", self.x, self.y, self.width, self.height)
end
