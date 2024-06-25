--local InputController = require "input_controller"

local game_state = {
    game_started = false,
    game_paused = false,
    game_over = false,
    focused = true,
    score_to_win = 3,
    win_message = nil,
}

function game_state.update(_)
end

function game_state.draw()
    local font = love.graphics.newFont(36)
    local text = love.graphics.newText(font)
    local window_width, window_height = love.window.getMode()

    local function text_y()
        return (window_height - text:getHeight()) / 2
    end

    if not game_state.game_started then
        text:setf("Press enter/return to start", window_width, "center")
        love.graphics.draw(text, 0, text_y())
    elseif game_state.game_over then
        text:setf(game_state.win_message, window_width, "center")
        love.graphics.draw(text, 0, text_y())
    elseif game_state.game_paused then
        text:setf("PAUSED", window_width, "center")
        love.graphics.draw(text, 0, text_y())
    end
end

return game_state
