local x
local y
local tick
local drawRectangle

function love.load()
    tick = require "tick"

    x = 30
    y = 50

    drawRectangle = false

    tick.delay(function()
            drawRectangle = true
        end,
        0.7)

    --[[
    tick.delay(function()
            drawRectangle = false
        end,
        1.2)]] --
end

function love.keypressed(key)
    if key == "space" then
        x = math.random(100, 500)
        y = math.random(100, 500)
    end
end

function love.keyreleased(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    tick.update(dt)
end

function love.draw()
    if drawRectangle then
        love.graphics.rectangle("fill", x, y, 100, 200)
    end
end
