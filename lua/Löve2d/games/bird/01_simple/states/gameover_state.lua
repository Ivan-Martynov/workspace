local IGameState = require "states.state_interface"
local StateController = require "states.state_controller"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"
local SoundHelper = require "audio_tools.sound_helper"

local background = require "entities.background"
local ground = require "entities.ground"
local pipes = require "entities.pipes"
local bird = require "entities.bird"

local GameoverState = IGameState:extend()

function GameoverState:init()
    self.text = love.graphics.newText(love.graphics.newFont(32))
    local window_width = love.window.getMode()
    self.text:addf("Final score: " .. bird.score, window_width, "center")
    self.text:addf("\nPress return to go to the main menu", window_width,
        "center")
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
        love.event.quit()
    elseif KeyboardController.key_pressed("return") then
        StateController:set_state("menu")
    end
end

function GameoverState:update(dt)
    background:update(dt)
end

function GameoverState:draw()
    background:draw()
    ground:draw()
    pipes:draw()
    bird:draw()

    local window_width, window_height = love.window.getMode()
    love.graphics.setColor(0, 0, 0, 0.5)
    love.graphics.rectangle("fill", 0, 0, window_width, window_height)

    love.graphics.setColor(ColorSchemeHelper.current.foreground)

    love.graphics.draw(self.text, 0,
        (window_height - self.text:getHeight()) / 2)
end

return GameoverState
