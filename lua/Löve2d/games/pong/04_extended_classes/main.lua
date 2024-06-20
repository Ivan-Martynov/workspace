--[[
    --! @file
    --! @brief Simple version of Pong.

    Next version of pong, using extended version of classes.
    Features:
    - Objects can be created in the constructor style.

    - Audio files are now in a separate folder.
]]

-- Classes for paddles and a ball.
local PaddleClass = require "paddle"
local BallClass = require "ball"

-- Sound helper.
local SoundHelper = require "sound_helper"

-- Color scheme helper.
local ColorSchemeHelper = require "color_scheme_helper"

--! Flag to track whether the game is paused.
local game_paused

--! Paddles.
local left_paddle
local right_paddle

--! Ball.
local ball

function love.load()
    -- Set title.
    love.window.setTitle("Pong")

    local window_width, window_height = love.window.getMode()

    -- Initialize color scheme and background color.
    ColorSchemeHelper:set_scheme(ColorSchemeHelper.solarized_light)
    love.graphics.setBackgroundColor(ColorSchemeHelper.current[1])

    -- Using ball class to instantiate a ball.
    ball = BallClass(window_width / 2, window_height / 2, 5,
        ColorSchemeHelper.current[5])

    -- Using paddle class to instantiate paddles.
    local paddle_width = 10
    local paddle_height = 75
    local paddle_y = (window_height - paddle_height) / 2
    left_paddle = PaddleClass(0, paddle_y, paddle_width, paddle_height,
        { up = "w", down = "s" }, ColorSchemeHelper.current[2])
    right_paddle = PaddleClass(window_width - paddle_width, paddle_y,
        paddle_width, paddle_height,
        { up = "up", down = "down" }, ColorSchemeHelper.current[2])

    -- Initialize background sound.
    SoundHelper.backbround:play()

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
        SoundHelper.backbround:pause()
        return
    end
    SoundHelper.backbround:play()

    ball:update(dt)
    left_paddle:update(dt)
    right_paddle:update(dt)

    ball:resolve_collisions(left_paddle, right_paddle)
end

-- Main rendering function.
function love.draw()
    love.graphics.setColor(ColorSchemeHelper.current["white"])

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

    love.graphics.setColor(ColorSchemeHelper.current["white"])
end
