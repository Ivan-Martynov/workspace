local world = require("world")
local triangle = require("triangle")
local bar = require("bar")

local paused = false

local key_map = {
    space = function ()
        paused = not paused
    end,
    escape = function()
        love.event.quit()
    end
}

love.keypressed = function (key)
    print("Key pressed = ", key)

    if key_map[key] then
        key_map[key]()
    end
end

love.focus = function (f)
    if not f then
        paused = true
    end
end

love.update = function (dt)
    if paused then
        return
    end

    world:update(dt)
end

love.draw = function ()
    love.graphics.polygon("line", triangle.body:getWorldPoints(
        triangle.shape:getPoints()))
    love.graphics.polygon("line", bar.body:getWorldPoints(
        bar.shape:getPoints()))

    local center_x = love.graphics.getWidth() / 2
    local center_y = love.graphics.getHeight() / 2

    if paused then
        local font_size = 36
        local font = love.graphics.newFont(font_size)
        love.graphics.setFont(font)
        love.graphics.setColor(1, 1, 1, 0.9)
        local text = "Paused"
        love.graphics.print("Paused", center_x, center_y, 0, 1, 1,
            font:getWidth(text) / 2, font:getHeight() / 2)
        love.graphics.setColor(1, 1, 1, 1)
    end
end
