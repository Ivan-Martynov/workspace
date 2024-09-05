local MovableObject2d = require "structures.movable_object_2d"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"
local MouseController = require "input.mouse_controller"
local TouchScreenController = require "input.touch_screen_controller"

local FlapCommand = require "commands.flap_command"

local ground = require "entities.ground"

local BeeClass = MovableObject2d:extend()

local flap_speed = -200
local gravity_speed = 200

function BeeClass:reset()
    local window_width, window_height = love.window.getMode()
    self.x = window_width / 5
    self.y = (window_height - self.height) / 2
    self.angle = 0
    self.score = 0
end

function BeeClass:init(image_path)
    self.parent.init(self, 100, 100, 0, gravity_speed)
    self.image = love.graphics.newImage(image_path)
    self.width = self.image:getWidth()
    self.height = self.image:getHeight()
    self:reset()
end

function BeeClass:update(dt)
    if KeyboardController.key_pressed("space")
        or MouseController.button_pressed(1)
        or (#TouchScreenController.get_all_touches() > 0) then
        local flap_command = FlapCommand(self, flap_speed, dt)
        flap_command:execute()
    else
        local flap_command = FlapCommand(self, gravity_speed, dt)
        flap_command:execute()
    end

    if self.y < 0 then
        self.y = 0
    elseif self.y + self.height > ground.y then
        self.y = ground.y - self.height
    end
end

function BeeClass:draw()
    local r, g, b, a = love.graphics.getColor()

    love.graphics.push()

    love.graphics.translate(self.x, self.y)
    love.graphics.rotate(self.angle)

    love.graphics.setColor(1, 1, 1)
    love.graphics.draw(self.image)

    love.graphics.pop()

    local font = love.graphics.newFont(32)
    love.graphics.setFont(font)
    love.graphics.setColor(ColorSchemeHelper.current.red)
    love.graphics.print("Score: " .. tostring(self.score), 20, 20)

    love.graphics.setColor(r, g, b, a)
end

local bee = BeeClass("assets/graphics/bee.png")

return bee
