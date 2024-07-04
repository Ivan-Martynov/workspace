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

local PauseState = IGameState:extend()
PauseState.keys = { "p" }

function PauseState:init()
    self.keys = PauseState.keys
    self.text = love.graphics.newText(love.graphics.newFont(32))
    local window_width = love.window.getMode()
    self.text:addf("PAUSED", window_width, "center")
end

function PauseState:keypressed()
    if KeyboardController.key_pressed("escape") then
        QuitGameCommand():execute()
    elseif KeyboardController.one_of_keys_pressed(self.keys) then
        StateController:set_state("game")
    end
end

function PauseState:touchpressed()
    StateController:set_state("game")
end

function PauseState:mousepressed()
    StateController:set_state("game")
end


function PauseState:update(_)
    SoundHelper:pause("background")
end

function PauseState:draw()
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

return PauseState
