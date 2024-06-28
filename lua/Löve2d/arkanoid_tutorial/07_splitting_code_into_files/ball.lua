local vector = require "vector"

-- Ball object.
local ball = {
    position = vector(200, 500),
    speed = vector(300, -300),
    radius = 10,
}
ball.position.y = ball.position.y - ball.radius * 2

function ball.reposition()
    ball.position = vector(200, 500)
end

function ball.rebound(shift, toggle_speed_x)
    if toggle_speed_x == nil then
        toggle_speed_x = true
    end

    local min_shift = math.min(math.abs(shift.x), math.abs(shift.y))

    if math.abs(shift.x) == min_shift then
        shift.y = 0
    else
        shift.x = 0
    end

    ball.position.x = ball.position.x + shift.x * 2
    ball.position.y = ball.position.y + shift.y * 2

    if (shift.x ~= 0) and toggle_speed_x then
        ball.speed.x = -ball.speed.x
    end
    if shift.y ~= 0 then
        ball.speed.y = -ball.speed.y
    end
end

-- Update the ball.
function ball.update(dt)
    ball.position = ball.position + ball.speed * dt
end

-- Draw the ball.
function ball.draw()
    local circle_segment_count = 16
    love.graphics.circle("line", ball.position.x, ball.position.y, ball.radius,
        circle_segment_count)
end

return ball
