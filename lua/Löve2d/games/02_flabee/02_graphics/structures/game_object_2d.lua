local ClassTemplate = require "structures.class_template"

local GameObject2d = ClassTemplate:extend()

-- Initialize object's coordinates and colour.
function GameObject2d:init(x, y, color)
    self.x = x or 0
    self.y = y or 0
    if color ~= nil then
        self.color = color
    end
end

-- Function to update an object, usually having a time parameter (dt).
function GameObject2d:update(_)
end

-- Function to draw an object, optionally having mode parameter.
function GameObject2d:draw(_)
end

return GameObject2d
