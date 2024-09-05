local IGameState = require "states.state_interface"
local StateController = require "states.state_controller"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"
local SoundHelper = require "audio_tools.sound_helper"
local QuitGameCommand = require "commands.quit_game_command"

local background = require "entities.background"
local ground = require "entities.ground"

local MenuState = IGameState:extend()

function MenuState:init()
    local window_width = love.window.getMode()
    self.text = love.graphics.newText(love.graphics.newFont(32))
    self.text:addf("Press return or click/tap to start", window_width, "center")
end

function MenuState:focus(is_focused)
    if not is_focused then
        SoundHelper:pause("background")
    else
        SoundHelper:play("background")
    end
end

function MenuState:keypressed()
    if KeyboardController.key_pressed("escape") then
        QuitGameCommand():execute()
    elseif KeyboardController.key_pressed("return") then
        StateController:set_state("countdown", true)
    end
end

function MenuState:touchpressed()
    StateController:set_state("countdown", true)
end

function MenuState:mousepressed()
    StateController:set_state("countdown", true)
end


function MenuState:update(dt)
    background:update(dt)
    ground:update(dt)
end

function MenuState:draw()
    background:draw()
    ground:draw()

    local window_width, window_height = love.window.getMode()
    love.graphics.setColor(0, 0, 0, 0.5)
    love.graphics.rectangle("fill", 0, 0, window_width, window_height)

    love.graphics.setColor(ColorSchemeHelper.current.foreground)

    love.graphics.draw(self.text, 0,
        (window_height - self.text:getHeight()) / 2)
end

return MenuState
