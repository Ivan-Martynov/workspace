local ColorSchemeHelper = require "color_tools.color_scheme_helper"
ColorSchemeHelper:set_scheme(ColorSchemeHelper.solarized_dark)

local StateController = require "states.state_controller"

local MenuState = require "states.menu_state"
local CountdownState = require "states.countdown_state"
local GameState = require "states.game_state"
local PauseState = require "states.pause_state"
local GameoverState = require "states.gameover_state"

local window_width, window_height
function love.load()
    window_width, window_height = love.window.getMode()

    StateController:init({
        menu = function() return MenuState() end,
        countdown = function() return CountdownState() end,
        game = function() return GameState() end,
        pause = function() return PauseState() end,
        gameover = function() return GameoverState() end,
    })
    StateController:set_state("menu")
end

function love.focus(is_focused)
    StateController.current:focus(is_focused)
end

function love.keypressed(key, ...)
    StateController.current:keypressed(key, ...)
end

function love.touchpressed()
    StateController.current:touchpressed()
end

function love.mousepressed()
    StateController.current:mousepressed()
end

function love.update(dt)
    StateController.current:update(dt)
end

function love.draw()
    StateController.current:draw()
end

function love.resize(w, h)
    print(window_width .. ", " .. window_height .. ", " .. w .. "," .. h)
    print(window_width / w .. ", " .. window_height / h)

    --local scale = math.min(window_width / w, window_height / h)

    window_width = w
    window_height = h
end
