local MovableObject2d = require "structures.movable_object_2d"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"

local dx = -100

local BoxClass = MovableObject2d:extend()
function BoxClass:init(x, y, width, height, color)
    self.parent.init(self, x, y, dx, 0, color)
    self.width = width
    self.height = height
end

function BoxClass:draw()
    love.graphics.setColor(self.color)
    love.graphics.rectangle("fill", self.x, self.y, self.width, self.height)
end

local GroundClass = MovableObject2d:extend()

function GroundClass:add_box(x, width, height, color)
    local _, window_height = love.window.getMode()
    table.insert(self.boxes, BoxClass(x, window_height - height, width, height, color))
end

function GroundClass:init(height)
    local window_width, window_height = love.window.getMode()
    self.parent.init(self, 0, window_height - height, dx, 0)
    self.width = window_width
    self.height = height
    self.adding_first_box = true
    self.box_x = 0
    self.box_width = 40
    self.first_box_color = ColorSchemeHelper.current.red
    self.second_box_color = ColorSchemeHelper.current.violet
    self.boxes = {}
end

function GroundClass:add_box()
    local window_width, window_height = love.window.getMode()
    if self.box_x <= window_width then

        local color
        if self.adding_first_box then
            self.adding_first_box = false
            color = self.first_box_color
        else
            self.adding_first_box = true
            color = self.second_box_color
        end

        table.insert(self.boxes, BoxClass(self.box_x,
            window_height - self.height, self.box_width, self.height, color))
        self.box_x = self.box_x + self.box_width
    end
end

function GroundClass:update(dt)
    self.parent.update(self, dt)

    self:add_box()

    for _, box in ipairs(self.boxes) do
        box:update(dt)
    end

    if self.boxes[1].x < -self.boxes[1].width then
        table.remove(self.boxes, 1)
    end

    self.box_x = self.box_x + dx * dt
end

function GroundClass:draw()
    for _, box in ipairs(self.boxes) do
        box:draw()
    end
end

local ground = GroundClass(10)

return ground
