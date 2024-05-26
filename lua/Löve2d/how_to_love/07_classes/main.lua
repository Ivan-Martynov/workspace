Object = require "classic"

local r1
local r2

function love.load()
    local Rectangle = require "rectangle"
    local Circle = require "circle"

    r1 = Rectangle(100, 100, 70, 90)
    r2 = Circle(50, 50, 30)
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    r1:update(dt)
    r2:update(dt)
end

function love.draw()
    r1:draw()
    r2:draw()
end
