local MovableObject2d = require "structures.movable_object_2d"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"
local MouseController = require "input.mouse_controller"
local TouchScreenController = require "input.touch_screen_controller"

local ground = require "entities.ground"

local BirdClass = MovableObject2d:extend()

function BirdClass:reset()
    local window_width, window_height = love.window.getMode()
    self.x = window_width / 5
    self.y = (window_height - self.height) / 2
    self.score = 0
end

function BirdClass:init(color)
    self.parent.init(self, 100, 100, 0, 0, color)
    self.width = 40
    self.height = 20
    self:reset()
end

function BirdClass:update(dt)
    self.parent.update(self, dt)
    if KeyboardController.key_pressed("space")
        or MouseController.button_pressed(1)
        or (#TouchScreenController.get_all_touches() > 0) then
        self.dy = -200
    else
        self.dy = 200
    end

    if self.y < 0 then
        self.y = 0
    elseif self.y + self.height > ground.y then
        self.y = ground.y - self.height
    end
end

function BirdClass:draw()
    local r, g, b, a = love.graphics.getColor()

    love.graphics.setColor(self.color)
    love.graphics.rectangle("fill", self.x, self.y, self.width, self.height)

    local font = love.graphics.newFont(32)
    love.graphics.setFont(font)
    love.graphics.setColor(ColorSchemeHelper.current.red)
    love.graphics.print(tostring(self.score), 20, 20)

    love.graphics.setColor(r, g, b, a)
end

local bird = BirdClass(ColorSchemeHelper.current.orange)

return bird
