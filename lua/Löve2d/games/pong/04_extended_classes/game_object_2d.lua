local ClassTemplate = require "class_template"

local GameObject2d = ClassTemplate:extend()

-- Initialize object's coordinates and colour.
function GameObject2d:init(x, y, color)
    self.x = x or 0
    self.y = y or 0
    self.color = color or { 1, 1, 1, 1 }
end

-- Function to update an object, usually having a time parameter (dt).
function GameObject2d:update(_)
end

-- Function to draw an object, optionally having mode parameter.
function GameObject2d:draw(_)
end

return GameObject2d
