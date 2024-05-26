local r1
local r2

local function is_AABB_collision(a, b)
    return (a.x + a.width > b.x)
        and (a.x < b.x + b.width)
        and (a.y + a.height > b.y)
        and (a.y < b.y + b.height)
end

function love.load()
    r1 = { x = 10, y = 100, width = 100, height = 100 }
    r2 = { x = 250, y = 120, width = 150, height = 120 }
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    r1.x = r1.x + 100 * dt
end

function love.draw()
    local mode = is_AABB_collision(r1, r2) and "fill" or "line"

    love.graphics.rectangle(mode, r1.x, r1.y, r1.width, r1.height)
    love.graphics.rectangle(mode, r2.x, r2.y, r2.width, r2.height)
end