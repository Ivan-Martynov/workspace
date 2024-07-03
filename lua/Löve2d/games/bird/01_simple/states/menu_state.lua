local IGameState = require "states.state_interface"
local StateController = require "states.state_controller"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"
local SoundHelper = require "audio_tools.sound_helper"

local background = require "entities.background"
local ground = require "entities.ground"

local MenuState = IGameState:extend()

function MenuState:init()
    self.text = love.graphics.newText(love.graphics.newFont(32))
    self.text:add("Press return to start")
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
        love.event.quit()
    elseif KeyboardController.key_pressed("return") then
        StateController:set_state("game", true)
    end
end

function MenuState:update(dt)
    --SoundHelper:play("background")

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

    love.graphics.draw(self.text,
        (window_width - self.text:getWidth()) / 2,
        (window_height - self.text:getHeight()) / 2)
end

return MenuState
