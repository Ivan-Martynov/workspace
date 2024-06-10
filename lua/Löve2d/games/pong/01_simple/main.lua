
local left_paddle
local right_paddle

local ball

local function get_paddle(x)
    local paddle_height = 75
    local _, window_height = love.window.getMode()

    return {
        x = x,
        y = window_height / 2 - paddle_height,
        width = 10,
        height = paddle_height,
        dy = 0,
        score = 0,
    }
end
local function reset_ball()
    local window_width, window_height = love.window.getMode()
    ball = {
        x = window_width / 2,
        y = window_height / 2,
        radius = 5,
        dx = love.math.random(150, 200),
        dy = love.math.random(150, 200),
    }

    if love.math.random(2) == 1 then
        ball.dx = -ball.dx
    end

    if love.math.random(2) == 1 then
        ball.dy = -ball.dy
    end
end

function love.load()
    local window_width, _ = love.window.getMode()
    left_paddle = get_paddle(0)
    right_paddle = get_paddle(window_width - left_paddle.width)

    reset_ball()
end


function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

local function update_paddle(paddle, up_dir, dt)
    local paddle_speed = 280
    if up_dir then
        paddle.y = paddle.y - paddle_speed * dt
    else
        paddle.y = paddle.y + paddle_speed * dt
    end

    local _, window_height = love.window.getMode()
    paddle.y = math.min(math.max(paddle.y, 0), window_height - paddle.height)
end

local function clamp(x, a, b)
    if x < a then
        return a
    elseif x > b then
        return b
    else
        return x
    end
end

local function ball_collides(rect)
    local closest_x = clamp(ball.x, rect.x, rect.x + rect.width)
    local closest_y = clamp(ball.y, rect.y, rect.y + rect.height)

    return (ball.x - closest_x) ^ 2 + (ball.y - closest_y) ^ 2
        <= ball.radius ^ 2
end

function love.update(dt)
    -- Update the paddles.
    if love.keyboard.isDown("w") then
        update_paddle(left_paddle, true, dt)
    elseif love.keyboard.isDown("s") then
        update_paddle(left_paddle, false, dt)
    end

    if love.keyboard.isDown("up") then
        update_paddle(right_paddle, true, dt)
    elseif love.keyboard.isDown("down") then
        update_paddle(right_paddle, false, dt)
    end

    -- Update the ball.
    ball.x = ball.x + ball.dx * dt
    ball.y = ball.y + ball.dy * dt

    local window_width, window_height = love.window.getMode()
    if ball_collides(left_paddle) or ball_collides(right_paddle) then
        ball.dx = -ball.dx * 1.1
        ball.dy = ball.dy * 1.1
        ball.x = ball.x + ball.dx * dt
    elseif (ball.y <= ball.radius) or
        (ball.y + ball.radius >= window_height) then
        ball.dy = -ball.dy
    elseif (ball.x <= ball.radius) then
        right_paddle.score = right_paddle.score + 1
        reset_ball()
    elseif (ball.x + ball.radius >= window_width) then
        left_paddle.score = left_paddle.score + 1
        reset_ball()
    end
end

function love.draw()
    local window_width, window_height = love.window.getMode()

    -- Draw vertical centre line.
    local centre_x = window_width / 2
    love.graphics.line(centre_x, 0, centre_x, window_height)

    -- Draw ball.
    love.graphics.circle("fill", ball.x, ball.y, ball.radius)

    -- Draw paddles.
    local current_font = love.graphics.newFont(32)
    love.graphics.setFont(current_font)
    love.graphics.rectangle("fill", left_paddle.x, left_paddle.y,
        left_paddle.width, left_paddle.height)
    love.graphics.print(left_paddle.score,
        window_width / 4 - current_font:getWidth(left_paddle.score) / 2,
        current_font:getHeight())

    love.graphics.rectangle("fill", right_paddle.x, right_paddle.y,
        right_paddle.width, right_paddle.height)
    love.graphics.print(right_paddle.score,
        window_width / 4 * 3 - current_font:getWidth(right_paddle.score) / 2,
        current_font:getHeight())
end
