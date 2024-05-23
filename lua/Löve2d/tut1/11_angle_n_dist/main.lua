
if arg[2] == "debug" then
    require("lldebugger").start()
end

local circle
local cursor
local angle

function love.load()
    circle = { x = 100, y = 100, radius = 25, speed = 200 }
    cursor = {}
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    cursor.x, cursor.y = love.mouse.getPosition()
    angle = math.atan2(cursor.y - circle.y, cursor.x - circle.x)
end

function love.draw()
    love.graphics.circle("line", circle.x, circle.y, circle.radius)

    love.graphics.print("Angle: " .. angle, 10, 10)

    love.graphics.line(circle.x, circle.y, cursor.x, circle.y)
    love.graphics.line(circle.x, circle.y, circle.x, cursor.y)

    love.graphics.line(circle.x, circle.y, cursor.x, cursor.y)
end
