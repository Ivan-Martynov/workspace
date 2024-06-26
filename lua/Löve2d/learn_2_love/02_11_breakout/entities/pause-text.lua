local state = require("state")

return function()
    local entity = {}
    local window_width, window_height = love.window.getMode()
    local font = love.graphics.newFont(36)

    entity.text = "Paused"

    entity.draw = function(_)
        if state.paused then
            love.graphics.setFont(font)
            love.graphics.printf({ state.palette[5], "PAUSED" },
                0, math.floor((window_height - font:getHeight()) / 2),
                window_width, "center")
        end
    end

    return entity
end
