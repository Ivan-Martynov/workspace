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

local entities = { background, ground, bee }

local CountdownState = IGameState:extend()

function CountdownState:load()
    SoundHelper:play("background")
end

local count_text = {
    count = 3,
    timer = 0,
}

function count_text:draw()
    local window_width, window_height = love.window.getMode()

    love.graphics.setColor(ColorSchemeHelper.current.orange)
    local font = love.graphics.newFont(48)
    local str = tostring(self.count)
    love.graphics.printf(str, font, 0,
        (window_height / 2 - font:getHeight()) / 2, window_width, "center")
end

function CountdownState:init()
    self.count_text = count_text
    self.entities = entities
    table.insert(self.entities, self.count_text)
end

function CountdownState:load(do_reset)
    if do_reset then
        self.count_text.count = 3
        self.count_text.timer = 0
        bee:reset()
        pipes:reset()
    end
end

function CountdownState:focus(is_focused)
    if not is_focused then
        StateController:set_state("pause", self)
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

    self.count_text.timer = self.count_text.timer + dt
    if self.count_text.timer >= 1 then
        self.count_text.count = self.count_text.count - 1
        self.count_text.timer = self.count_text.timer - 1
    end

    if self.count_text.count == 0 then
        StateController:set_state("game")
    end
end

function CountdownState:draw()
    background:draw()
    ground:draw()
    bee:draw()
    self.count_text:draw()
end

return CountdownState
