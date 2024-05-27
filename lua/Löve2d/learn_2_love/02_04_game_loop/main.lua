local current_color = { 1, 1, 1, 1 }
local seconds = 0
local clock_display = "Seconds: " .. seconds

local key_map = {
    b = function()
        current_color = { 0, 0, 1, 1 }
    end,
    c = function()
        current_color = { 0, 1, 1, 1 }
    end,
    g = function()
        current_color = { 0, 1, 0, 1 }
    end,
    r = function()
        current_color = { 1, 0, 0, 1 }
    end,
    y = function()
        current_color = { 1, 1, 0, 1 }
    end,
    o = function()
        current_color = { 1, 0.5, 0, 1 }
    end,
    m = function()
        current_color = { 1, 0, 1, 1 }
    end,
    p = function()
        current_color = { 1, 0.5, 1, 1 }
    end,
    v = function()
        current_color = { 0.5, 0, 0.5, 1 }
    end,
    n = function()
        current_color = { 0.5, 0, 0, 1 }
    end,
    k = function()
        current_color = { 0, 0, 0, 1 }
    end,
    w = function()
        current_color = { 1, 1, 1, 1 }
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

love.update = function (dt)
    seconds = seconds + dt
    clock_display = "Seconds: " .. math.floor(seconds) .. "\ndt = " .. dt
end

love.draw = function ()
    --local first_square = { 100, 100, 100, 200, 200, 200, 200, 100 }
    local first_square = { seconds * 10, 100, seconds * 10,
        200, 200, 200, 200, 100 }
    local second_square = { 500, 500, 500, 400, 400, 400, 400, 500 }

    love.graphics.setColor(current_color)
    love.graphics.polygon("fill", first_square)
    love.graphics.polygon("line", second_square)

    local opposite_color = { 1 - current_color[1], 1 - current_color[2],
        1 - current_color[3], 1 }

    love.graphics.setColor(opposite_color)
    love.graphics.polygon("line", first_square)
    love.graphics.polygon("fill", second_square)

    love.graphics.setColor(1, 1, 1, 1)
    love.graphics.print(clock_display, 0, 0, 0, 2, 2)
end
