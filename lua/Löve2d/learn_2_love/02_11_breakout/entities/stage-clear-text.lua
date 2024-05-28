local state = require("state")

return function()
    local window_width, window_height = love.window.getMode()
    local entity = {}
    local font = love.graphics.newFont(48)

    entity.draw = function (_)
        if state.stage_cleared then
            love.graphics.setFont(font)
            love.graphics.printf({ state.palette[4], "STAGE CLEARED" },
                0, math.floor((window_height - font:getHeight()) / 2),
                window_width, "center")
        end
    end

    return entity
end
