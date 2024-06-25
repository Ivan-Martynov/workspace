--[[
    --! @file
    --! @brief Simple version of Pong.

    Next version of pong, using extended version of classes.
    Features:
    - Objects can be created in the constructor style.

    - Audio files are now in a separate folder.

    - Added commands to use command design pattern to react on user's input.

    - Setting game state to control when the game starts and ends.
]]

-- Classes for paddles and a ball.
local PaddleClass = require "paddle"
local BallClass = require "ball"

local InputController = require "input_controller"
-- Sound helper.
local SoundHelper = require "sound_helper"

-- Color scheme helper.
local ColorSchemeHelper = require "color_scheme_helper"

--! Flag to track whether the game is paused.
local GameState = require "game_state"
local StartGameCommand = require "start_game_command"
local GameOverCommand = require "game_over_command"
local QuitGameCommand = require "quit_game_command"
local PauseCommand = require "pause_command"
local pause_game = PauseCommand(false)
local pause_toggle_game = PauseCommand(true)

local commands = {}

local start_game_command

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

    -- start_game_command = StartGameCommand({ left_paddle, right_paddle })
    commands["start_game"] = StartGameCommand({ left_paddle, right_paddle })
    --table.insert(commands, "start_game",
    --  StartGameCommand({ left_paddle, right_paddle }))

    -- Initialize background sound.
    SoundHelper.backbround:play()
end

--! @brief Callback to check whether the window is focused.
--! @param focused Flag, informing if the window is focused.
function love.focus(focused)
    if not focused and GameState.game_started then
        pause_game:execute()
    end
end

--! @brief Callback to observe if a certain key was pressed.
--! @param key Name of the pressed key.
function love.keypressed(key)
    if InputController.key_pressed("return", "enter") and
        (not GameState.game_started or GameState.game_over) then
        commands["start_game"]:execute()
    elseif key == "p" and GameState.game_started then
        pause_toggle_game:execute()
    elseif key == "escape" then
        QuitGameCommand():execute()
    end
end

--! @brief Main game loop function.
--! @param dt Delta time.
function love.update(dt)
    if not GameState.game_started or GameState.game_paused or
        GameState.game_over then
        return
    end

    ball:update(dt)
    left_paddle:update(dt)
    right_paddle:update(dt)

    ball:resolve_collisions(left_paddle, right_paddle)

    if (left_paddle.score == GameState.score_to_win) or
        (right_paddle.score == GameState.score_to_win) then
        GameOverCommand(left_paddle.score == GameState.score_to_win):execute()
    end
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

    GameState:draw()

    love.graphics.setColor(ColorSchemeHelper.current["white"])
end
