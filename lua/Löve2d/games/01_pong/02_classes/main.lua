--[[
    --! @file
    --! @brief Simple version of Pong.

    Next version of pong, introducing simple idea of classes.
    Features:
    - Ball and paddles are now instances of respective classes.
    - They have either own update/draw methods or derived from the base class.

    - Paddle class has keys representing up/down keys.
    - Paddles can be moved up and down using keys set in their instances.
    - Alternatively, touch on a screen or left mouse button click allow to move
    - paddles if the touch/click is more or less aligned with the paddle.

    - Now printf function gets a font as an argument.

    - Ball collisions resolutions are moved into the ball's class.
]]

-- Classes for paddles and a ball.
local PaddleClass = require "paddle"
local BallClass = require "ball"

--! Flag to track whether the game is paused.
local game_paused

--! Paddles.
local left_paddle
local right_paddle

--! Ball.
local ball

function love.load()
    local window_width, window_height = love.window.getMode()

    -- Using ball class to instantiate a ball.
--    ball = BallClass.new(window_width / 2, window_height / 2, 5)
    ball = BallClass.new(window_width / 2, window_height / 2, 5)

    -- Using paddle class to instantiate paddles.
    local paddle_width = 10
    local paddle_height = 75
    local paddle_y = (window_height - paddle_height) / 2
    left_paddle = PaddleClass.new(0, paddle_y, paddle_width, paddle_height,
        { up = "w", down = "s" })
    right_paddle = PaddleClass.new(window_width - paddle_width, paddle_y,
        paddle_width, paddle_height,
        { up = "up", down = "down" })

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

--! @brief Main game loop function.
--! @param dt Delta time.
function love.update(dt)
    if game_paused then
        return
    end

    ball:update(dt)
    left_paddle:update(dt)
    right_paddle:update(dt)

    ball:resolve_collisions(left_paddle, right_paddle)
end

-- Main rendering function.
function love.draw()
    local window_width, window_height = love.window.getMode()

    -- Draw vertical centre line.
    local centre_x = window_width / 2
    love.graphics.line(centre_x, 0, centre_x, window_height)

    ball:draw()
    left_paddle:draw()
    right_paddle:draw()

    if game_paused then
        local font = love.graphics.newFont(36)
        love.graphics.printf("PAUSED", font, 0,
            (window_height - font:getHeight()) / 2, window_width, "center")
    end
end
