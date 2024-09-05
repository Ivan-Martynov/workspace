local vector = require "vector"

-- Ball object.
local ball = {
    position = vector(200, 500),
    speed = vector(300, -300),
    image = love.graphics.newImage("images/ball.png"),
    x_tile_pos = 0,
    y_tile_pos = 0,
}
ball.tile_width = ball.image:getWidth()
ball.tile_height = ball.image:getHeight()
ball.tileset_width = ball.tile_width
ball.tileset_height = ball.tile_height

ball.quad = love.graphics.newQuad(ball.x_tile_pos, ball.y_tile_pos,
    ball.tile_width, ball.tile_height, ball.tileset_width, ball.tileset_height)

ball.radius = ball.tile_width / 2
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
    love.graphics.draw(ball.image, ball.quad,
        ball.position.x - ball.radius, ball.position.y - ball.radius)
    local circle_segment_count = 16
    love.graphics.circle("line", ball.position.x, ball.position.y, ball.radius,
        circle_segment_count)
end

return ball
