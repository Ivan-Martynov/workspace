local IGameState = require "states.state_interface"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"

local background = require "entities.background"
local ground = require "entities.ground"

local MenuState = IGameState:extend()

function MenuState:init()
    self.text = love.graphics.newText(love.graphics.newFont(32))
    self.text:add("Press return to start")
end

function MenuState:update(dt)
    background:update(dt)
    ground:update(dt)
    if KeyboardController.key_pressed("return") then
    end
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

return MenuState()
