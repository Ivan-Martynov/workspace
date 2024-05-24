
if arg[2] == "debug" then
    require("lldebugger").start()
end

local circle
local cursor
local angle
local arrow

function love.load()
    circle = { x = 100, y = 100, radius = 25, speed = 200 }
    cursor = {}

    arrow = { x = 200, y = 200, speed = 300, angle = 0 }
    arrow.image = love.graphics.newImage("arrow_right.png")
    arrow.origin = { x = arrow.image:getWidth() / 2,
        y = arrow.image:getHeight() / 2}
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

local function calculate_distance(x1, y1, x2, y2)
    local dx = x2 - x1
    local dy = y2 - y1

    return math.sqrt(dx * dx + dy * dy)
end

local function atan2(y, x)
    if x == 0 then
        if y == 0 then
            return 0
        else
            return y > 0 and math.pi / 2 or -math.pi / 2
        end
    else
        local arctan = math.atan(y / x)
        if x > 0 then
            return arctan
        else
            return y < 0 and -math.pi + arctan or math.pi + arctan
        end
    end
end

function love.update(dt)
    cursor.x, cursor.y = love.mouse.getPosition()

    local dx = cursor.x - circle.x
    local dy = cursor.y - circle.y

    local distance = calculate_distance(circle.x, circle.y, cursor.x, cursor.y)

    if (distance < 400) and (dx ~= 0) and (dy ~= 0) then
        angle = atan2(dy, dx)

        local angle_cos = math.cos(angle)
        local angle_sin = math.sin(angle)
        local ratio     = distance / 100

        circle.x        = circle.x + circle.speed * angle_cos * ratio * dt
        circle.y        = circle.y + circle.speed * angle_sin * ratio * dt
    end

    arrow.angle = atan2(cursor.y - arrow.y, cursor.x - arrow.x)
    local arrow_cos = math.cos(arrow.angle)
    local arrow_sin = math.sin(arrow.angle)
    arrow.x = arrow.x + arrow.speed * arrow_cos * dt
    arrow.y = arrow.y + arrow.speed * arrow_sin * dt
end

function love.draw()
    love.graphics.circle("line", circle.x, circle.y, circle.radius)

    love.graphics.print("Angle: " .. angle, 10, 10)

    love.graphics.line(circle.x, circle.y, cursor.x, circle.y)
    love.graphics.line(circle.x, circle.y, circle.x, cursor.y)

    love.graphics.line(circle.x, circle.y, cursor.x, cursor.y)

    love.graphics.line(cursor.x, cursor.y, cursor.x, circle.y)

    local distance = calculate_distance(circle.x, circle.y, cursor.x, cursor.y)
    love.graphics.circle("line", circle.x, circle.y, distance)

    love.graphics.draw(arrow.image, arrow.x, arrow.y, arrow.angle, 1, 1,
        arrow.origin.x, arrow.origin.y)
    love.graphics.circle("fill", cursor.x, cursor.y, 5)
end
