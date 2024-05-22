Object = require "classic"

local r1
local r2

function love.load()
    require "rectangle"

    r1 = Rectangle(100, 100)
    r2 = Rectangle(50, 50)
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    r1.update(r1, dt)
    r2:update(dt)
end

function love.draw()
    r1.draw(r1)
    r2:draw()
end
