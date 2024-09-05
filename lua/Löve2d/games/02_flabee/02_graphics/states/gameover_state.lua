local IGameState = require "states.state_interface"
local StateController = require "states.state_controller"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"
local SoundHelper = require "audio_tools.sound_helper"
local QuitGameCommand = require "commands.quit_game_command"

local background = require "entities.background"
local ground = require "entities.ground"
local pipes = require "entities.pipes"
local bee = require "entities.bee"

local GameoverState = IGameState:extend()

function GameoverState:init()
    self.text = love.graphics.newText(love.graphics.newFont(32))
    local window_width = love.window.getMode()
    self.text:addf("Final score: " .. bee.score, window_width, "center")
    self.text:addf("\nPress return or click/tap to go to the main menu",
        window_width, "center")
end

function GameoverState:focus(is_focused)
    if not is_focused then
        SoundHelper:pause("background")
    else
        SoundHelper:play("background")
    end
end

function GameoverState:keypressed()
    if KeyboardController.key_pressed("escape") then
        QuitGameCommand():execute()
    elseif KeyboardController.key_pressed("return") then
        StateController:set_state("menu")
    end
end

function GameoverState:touchpressed()
    StateController:set_state("menu")
end

function GameoverState:mousepressed()
    StateController:set_state("menu")
end

function GameoverState:update(dt)
    background:update(dt)
end

function GameoverState:draw()
    background:draw()
    ground:draw()
    pipes:draw()
    bee:draw()

    local window_width, window_height = love.window.getMode()
    love.graphics.setColor(0, 0, 0, 0.5)
    love.graphics.rectangle("fill", 0, 0, window_width, window_height)

    love.graphics.setColor(ColorSchemeHelper.current.foreground)

    love.graphics.draw(self.text, 0,
        (window_height - self.text:getHeight()) / 2)
end

return GameoverState
