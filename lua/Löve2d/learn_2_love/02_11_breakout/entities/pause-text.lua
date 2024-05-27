local input = require("input")

return function()
    local entity = {}
    local window_width, window_height = love.window.getMode()
    local font = love.graphics.newFont(48)

    entity.text = "Paused"

    entity.draw = function(self)
        if input.paused then
            love.graphics.setFont(font)
            love.graphics.print({ { 0.9, 0.9, 1, 0.9 }, self.text },
                math.floor((window_width - font:getWidth(self.text)) / 2),
                math.floor((window_height - font:getHeight()) / 2))
        end
    end

    return entity
end
