local MovableObject2d = require "structures.movable_object_2d"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"

local bird = MovableObject2d:extend()

function bird:init(x, y, color)
    self.parent.init(self, x, y, 0, 0, color)
    self.width = 40
    self.height = 20
end

function bird:draw()
    love.graphics.setColor(self.color)
    love.graphics.rectangle("fill", self.x, self.y, self.width, self.height)
    love.graphics.setColor(ColorSchemeHelper.current.foreground)
end

return bird
