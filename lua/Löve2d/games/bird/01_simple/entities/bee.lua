local MovableObject2d = require "structures.movable_object_2d"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"
local KeyboardController = require "input.keyboard_controller"
local MouseController = require "input.mouse_controller"
local TouchScreenController = require "input.touch_screen_controller"

local ground = require "entities.ground"

local BeeClass = MovableObject2d:extend()

function BeeClass:reset()
    local window_width, window_height = love.window.getMode()
    self.x = window_width / 5
    self.y = (window_height - self.height) / 2
    self.angle = 0
    self.score = 0
end

function BeeClass:init(color)
    self.parent.init(self, 100, 100, 0, 0, color)
    self.width = 40
    self.height = 20
    self:reset()
end

function BeeClass:update(dt)
    self.parent.update(self, dt)
    if KeyboardController.key_pressed("space")
        or MouseController.button_pressed(1)
        or (#TouchScreenController.get_all_touches() > 0) then
        self.angle = -math.pi / 20
        self.dy = -200
    else
        self.angle = math.pi / 20
        self.dy = 200
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

    local wing_color = ColorSchemeHelper.current.cyan
    wing_color[4] = 0.7
    love.graphics.setColor(wing_color)

    love.graphics.push()
    love.graphics.translate(self.width / 4, -self.height / 4)
    love.graphics.rotate(-math.pi / 4)
    love.graphics.ellipse("fill", 0, 0, self.height / 3, self.width / 2, 16)
    love.graphics.pop()

    love.graphics.setColor(self.color)
    love.graphics.rectangle("fill", 0, 0, self.width, self.height,
        self.width / 3, self.height / 3, 8)

    love.graphics.setColor(ColorSchemeHelper.current.black)
    local head_radius = self.height / 2
    local eye_x = self.width + head_radius / 2
    local eye_y = head_radius / 4
    love.graphics.line(eye_x, eye_y,
        eye_x + head_radius * 1.5 * math.cos(math.pi / 4),
        eye_y - head_radius * 1.5 * math.sin(math.pi / 4))
    love.graphics.line(eye_x, eye_y,
        eye_x + head_radius * 1.5 * math.cos(math.pi / 6),
        eye_y - head_radius * 1.5 * math.sin(math.pi / 6))

    love.graphics.setColor(self.color)
    love.graphics.circle("fill", self.width,
        head_radius / 2, head_radius)

    love.graphics.setColor(ColorSchemeHelper.current.black)
    local step_x = self.width / 4
    local x = step_x / 2
    for i = 0, 2 do
        love.graphics.rectangle("fill", x + i * step_x, 0,
            self.width / 8, self.height)
    end
    love.graphics.circle("fill", eye_x, eye_y, head_radius / 8)

    love.graphics.setColor(wing_color)

    love.graphics.push()
    love.graphics.translate(self.width / 4 + self.height / 4,
        -self.height / 4)
    love.graphics.rotate(-math.pi / 10)
    love.graphics.ellipse("fill", 0, 0, self.height / 3, self.width / 2, 16)
    love.graphics.pop()

    love.graphics.pop()

    local font = love.graphics.newFont(32)
    love.graphics.setFont(font)
    love.graphics.setColor(ColorSchemeHelper.current.red)
    love.graphics.print(tostring(self.score), 20, 20)

    love.graphics.setColor(r, g, b, a)
end

local bee = BeeClass(ColorSchemeHelper.current.yellow)

return bee
