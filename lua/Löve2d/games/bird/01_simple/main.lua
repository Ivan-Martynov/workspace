local ColorSchemeHelper = require "color_tools.color_scheme_helper"
ColorSchemeHelper:set_scheme(ColorSchemeHelper.solarized_dark)

local StateController = require "states.state_controller"

local MenuState = require "states.menu_state"
local GameState = require "states.game_state"
local PauseState = require "states.pause_state"
local GameoverState = require "states.gameover_state"

function love.load()
    StateController:init({
        menu = function() return MenuState() end,
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

function love.update(dt)
    StateController.current:update(dt)
end

function love.draw()
    StateController.current:draw()
end
