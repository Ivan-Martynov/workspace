
local function make_polygon(center, radius, point_count)
    assert(point_count > 2, "Requires at least 3 points.")
    local vertices = {}

    local step = math.pi * 2 / point_count

    for i = 0, point_count - 1 do
        local x = center.x + radius * math.cos(i * step)
        local y = center.y + radius * math.sin(i * step)

        vertices[i * 2 + 1] = x
        vertices[i * 2 + 2] = y
    end

    return vertices
end

love.draw = function ()
    love.graphics.polygon("line", 50, 0, 0, 100, 100, 10)
    love.graphics.polygon("fill", { 300, 300, 300, 400, 400, 400, 400, 300 })
    love.graphics.polygon("line", { 100, 100, 100, 200, 200, 100, 200, 200 })

    local n = 5
    local polygon = make_polygon({ x = 500, y = 200 }, 75, n)
    love.graphics.polygon("line", polygon)
    love.graphics.polygon("line", make_polygon({ x = 300, y = 100 }, 35, 7))
end

love.keypressed = function (key)
    if key == "escape" then
        love.event.quit()
    end
end
