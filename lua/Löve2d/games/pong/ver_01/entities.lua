local control = require "control"

-- Keep all game elements here.
local ball_entity = require "entities.ball"
local bar_entity = require "entities.horizontal_bar"
local paddle_entity = require "entities.paddle"

local window_width, window_height = love.window.getMode()

local ball = ball_entity(window_width / 2, window_height / 2)

local bar_y = 25
local upper_bar = bar_entity(0, bar_y)
local lower_bar = bar_entity(0, window_height - bar_y - upper_bar.height)

local left_paddle = paddle_entity(0, window_height / 2)
left_paddle.y = left_paddle.y - left_paddle.height / 2
local right_paddle = paddle_entity(window_width - left_paddle.width,
    left_paddle.y)

-- Collect all entities.
local entities =
{
    ball,
    upper_bar,
    lower_bar,
    left_paddle,
    right_paddle,
}

-- Update entities.
function entities.update(dt)
    for _, entity in ipairs(entities) do
        if entity.update then
            entity:update(dt);
        end
    end

    left_paddle:move(control.move_left_paddle_up,
        control.move_left_paddle_down)
    right_paddle:move(control.move_right_paddle_up,
        control.move_right_paddle_down)

    left_paddle.y = math.max(left_paddle.y, upper_bar.y + upper_bar.height)
    left_paddle.y = math.min(left_paddle.y - left_paddle.height, lower_bar.y)

    right_paddle.y = math.max(right_paddle.y, upper_bar.y + upper_bar.height)
    right_paddle.y = math.min(right_paddle.y - right_paddle.height, lower_bar.y)

    -- Resolve collisions with borders.
    if ball.y + ball.radius >= lower_bar.y then
        ball.dy = -ball.dy
        ball.y = ball.y + ball.dy * dt
    elseif ball.y - ball.radius <= upper_bar.y + upper_bar.height then
        ball.dy = -ball.dy
        ball.y = upper_bar.y + upper_bar.height + ball.radius + ball.dy * dt
    elseif (ball.x + ball.radius >= window_width)
        or (ball.x <= ball.radius) then
        ball:reset()
    end
end

-- Draw entities.
function entities.draw()
    for k, entity in ipairs(entities) do
        if entity.draw then
            entities[k]:draw();
        end
    end
end

return entities
