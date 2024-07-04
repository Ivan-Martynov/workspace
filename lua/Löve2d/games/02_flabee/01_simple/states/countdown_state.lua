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

local CountdownState = IGameState:extend()

function CountdownState:init()
    self.count = 3
    self.timer = 0
end

function CountdownState:load()
    bee:reset()
    pipes:reset()
end

function CountdownState:focus(is_focused)
    if not is_focused then
        SoundHelper:pause("background")
    else
        SoundHelper:play("background")
    end
end

function CountdownState:keypressed()
    if KeyboardController.key_pressed("escape") then
        QuitGameCommand():execute()
    end
end

function CountdownState:update(dt)
    background:update(dt)
    ground:update(dt)

    self.timer = self.timer + dt
    if self.timer >= 1 then
        self.count = self.count - 1
        self.timer = self.timer - 1
    end

    if self.count == 0 then
        StateController:set_state("game")
    end
end

function CountdownState:draw()
    background:draw()
    ground:draw()
    bee:draw()

    local window_width, window_height = love.window.getMode()
    love.graphics.setColor(0, 0, 0, 0.5)
    love.graphics.rectangle("fill", 0, 0, window_width, window_height)

    love.graphics.setColor(ColorSchemeHelper.current.foreground)

    local font = love.graphics.newFont(48)
    local str = tostring(self.count)
    love.graphics.printf(str, font, 0,
        (window_height - font:getHeight()) / 2, window_width, "center")
end

return CountdownState
