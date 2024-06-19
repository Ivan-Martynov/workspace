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

local Paddle_class = require "paddle"
local Ball_class = require "ball"

--! Flag to track whether the game is paused.
local game_paused

local function reset_ball_position(b)
    local window_width, window_height = love.window.getMode()
    b.x = window_width / 2
    b.y = window_height / 2
end

--! Paddles.
local left_paddle
local right_paddle

--! Ball.
local ball

function love.load()
    local window_width, window_height = love.window.getMode()

    ball = Ball_class.new(window_width / 2, window_height / 2, 5)

    local paddle_width = 10
    local paddle_height = 75
    local paddle_y = (window_height - paddle_height) / 2
    left_paddle = Paddle_class.new(0, paddle_y, paddle_width, paddle_height)
    right_paddle = Paddle_class.new(window_width - paddle_width, paddle_y,
        paddle_width, paddle_height)

    game_paused = false
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    ball:update(dt)
    left_paddle:update(dt)
    right_paddle:update(dt)
end

function love.draw()
    ball:draw()
    left_paddle:draw()
    right_paddle:draw()
end
