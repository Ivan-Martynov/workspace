--[[
    --! @file
    --! @brief Simple version of Pong.

    Simple version of pong, having a ball and two paddles.
    Features:
    - Ball bounces back from paddles and top and bottom borders.
    - After each bounce from a paddle the ball's velocity is slightly increased.
    - Left paddle can be moved up and down using 'w' and 's' keys, respectively.
    - Right paddle can be moved up and down using 'up' and 'down' keys,
    respectively.
    - Alternatively, touch on a touchscreen allows to move paddles if the touch
    is more or less aligned with the paddle.
    - Paddles represent players, having scores. A player's score in incremented
    when the ball flies behind opponent's paddle.
    - Pressing 'p' pauses or unpauses the game.
    - If window is not focused (that is, the user switches to another window or
    it is minimized) then the game is paused.
]]

--! Paddles.
local left_paddle
local right_paddle

--! Ball.
local ball

--! Flag to track whether the game is paused.
local game_paused

--! @brief Function to make a paddle.
--! @param x Paddle's x coordinate.
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

--! @brief Function to reset ball to its defaults.
local function reset_ball()
    local window_width, window_height = love.window.getMode()
    ball = {
        x = window_width / 2,
        y = window_height / 2,
        radius = 5,
        dx = love.math.random(150, 200),
        dy = love.math.random(150, 200),
        --dx = love.math.random(550, 560),
        --dy = love.math.random(550, 560),
    }

    if love.math.random(2) == 1 then
        ball.dx = -ball.dx
    end

    if love.math.random(2) == 1 then
        ball.dy = -ball.dy
    end
end

--! @brief Main loading function.
function love.load()
    local window_width, _ = love.window.getMode()
    left_paddle = get_paddle(0)
    right_paddle = get_paddle(window_width - left_paddle.width)

    reset_ball()

    game_paused = false
end

--! @brief Callback to check whether the window is focused.
--! @param focused Flag, informing if the window is focused.
function love.focus(focused)
    if not focused then
        game_paused = true
    end
end

--! @brief Callback to observe if a certain key was pressed.
--! @param key Name of the pressed key.
function love.keypressed(key)
    if key == "p" then
        game_paused = not game_paused
    elseif key == "escape" then
        love.event.quit()
    end
end

local touch_text = ""
function love.touchpressed(_, x, y, dx, dy, pressure)
--    touch_text = "Pressed => [" ..
--        string.format("%.0f", x) .. ", " .. string.format("%.0f", y) .. "], ["
--        .. string.format("%.0f", dx) .. ", " .. string.format("%.0f", dy) ..
--        "] => " .. pressure
end

function love.touchreleased(_, x, y, dx, dy, pressure)
--    touch_text = "Released => [" ..
--        string.format("%.0f", x) .. ", " .. string.format("%.0f", y) .. "], ["
--        .. string.format("%.0f", dx) .. ", " .. string.format("%.0f", dy) ..
--        "] => " .. pressure
end

function love.touchmoved(_, x, y, dx, dy, pressure)
-----    touch_text = "Moved => [" ..
-----        string.format("%.0f", x) .. ", " .. string.format("%.0f", y) .. "], ["
-----        .. string.format("%.0f", dx) .. ", " .. string.format("%.0f", dy) ..
-----        "] => " .. pressure
end

--! @brief Helper function to move a paddle up or down.
--! @param paddle Paddle to move.
--! @param up_dir Flag to move the paddle: true => up, otherwise down.
--! @param dt Delta time.
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

--[[
--! \brief Helper function to trigger paddle movement if the corresponding
button has been pressed.
]]
--! @param paddle Paddle to check.
--! @param up_key Key name for the up movement.
--! @param down_key Key name for the down movement.
--! @param dt Delta time.
local function check_paddle_key(paddle, up_key, down_key, dt)
    local touches = love.touch.getTouches()
    for _, id in ipairs(touches) do
        local x, y = love.touch.getPosition(id)

        -- Make touch area bigger than the paddle.
        if (x >= paddle.x - paddle.width) and
            (x <= paddle.x + paddle.width * 2) then
            if y < paddle.y + paddle.height / 2 then
                update_paddle(paddle, true, dt)
            else
                update_paddle(paddle, false, dt)
            end
        end
    end

    if love.keyboard.isDown(up_key) then
        update_paddle(paddle, true, dt)
    elseif love.keyboard.isDown(down_key) then
        update_paddle(paddle, false, dt)
    end
end

--! @brief Helper function to clamp the value within the given range.
--! @param x Value to clamp.
--! @param a Range's smaller value.
--! @param b Range's larger value.
local function clamp(x, a, b)
    if x < a then
        return a
    elseif x > b then
        return b
    else
        return x
    end
end

--! @brief Helper function to check whether the ball collides with a paddle.
--! @param rect Rectangle representing a paddle.
local function ball_collides(rect)
    local closest_x = clamp(ball.x, rect.x, rect.x + rect.width)
    local closest_y = clamp(ball.y, rect.y, rect.y + rect.height)

    return (ball.x - closest_x) ^ 2 + (ball.y - closest_y) ^ 2
        <= ball.radius ^ 2
end

--! @brief Helper function to move the ball.
--! @param dt Delta time.
local function move_ball(dt)
    ball.x = ball.x + ball.dx * dt
    ball.y = ball.y + ball.dy * dt
end

--! @brief Main game loop function.
--! @param dt Delta time.
function love.update(dt)
    if game_paused then
        return
    end

    -- Update the paddles.
    check_paddle_key(left_paddle, "w", "s", dt)
    check_paddle_key(right_paddle, "up", "down", dt)

    -- Update the ball.
    move_ball(dt)

    local window_width, window_height = love.window.getMode()
    if ball_collides(left_paddle) or ball_collides(right_paddle) then
        ball.dx = -ball.dx * 1.1
        ball.dy = ball.dy * 1.1
        -- Make an additional move to avoid repetitive collisions.
        move_ball(dt)
    elseif (ball.y <= ball.radius) or
        (ball.y + ball.radius >= window_height) then
        ball.dy = -ball.dy
        -- Make an additional move to avoid repetitive collisions.
        move_ball(dt)
    elseif (ball.x <= -ball.radius) then
        right_paddle.score = right_paddle.score + 1
        reset_ball()
    elseif (ball.x - ball.radius >= window_width) then
        left_paddle.score = left_paddle.score + 1
        reset_ball()
    end
end

local function draw_paddle(paddle, x)
    local window_width, window_height = love.window.getMode()
    local current_font = love.graphics.getFont()
    love.graphics.rectangle("fill", paddle.x, paddle.y,
        paddle.width, paddle.height)
    love.graphics.printf(paddle.score, x, current_font:getHeight(paddle.score),
        window_width / 2, "center")
end

--! @brief Main rendering function.
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

    draw_paddle(left_paddle, 0)
    draw_paddle(right_paddle, window_width / 2)

    -- Inform about paused game.
    if game_paused then
        love.graphics.printf("PAUSED", 0,
            (window_height - current_font:getHeight()) / 2,
            window_width, "center")
    end

    love.graphics.print(touch_text, 20, 20)
end
