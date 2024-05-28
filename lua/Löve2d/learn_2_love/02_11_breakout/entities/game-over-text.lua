local state = require("state")

return function()
    local window_width, window_height = love.window.getMode()
    local entity = {}
    local font = love.graphics.newFont(64)

    entity.draw = function (_)
        if state.game_over then
            love.graphics.setFont(font)
            love.graphics.printf({ state.palette[3], "GAME OVER" },
                0, math.floor((window_height - font:getHeight()) / 2),
                window_width, "center")
        end
    end

    return entity
end