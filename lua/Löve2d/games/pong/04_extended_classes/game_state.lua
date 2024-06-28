local GameState = {
    game_started = false,
    game_paused = false,
    game_over = false,
    focused = true,
    score_to_win = 3,
    start_message = "Press return or double tap/click to start",
    win_message = nil,
}

function GameState.draw()
    local font = love.graphics.newFont(36)
    local text = love.graphics.newText(font)
    local window_width, window_height = love.window.getMode()

    local function text_y()
        return (window_height - text:getHeight()) / 2
    end

    if not GameState.game_started then
        text:setf(GameState.start_message, window_width, "center")
        love.graphics.draw(text, 0, text_y())
    elseif GameState.game_over then
        text:setf(GameState.win_message, window_width, "center")
        love.graphics.draw(text, 0, text_y())
    elseif GameState.game_paused then
        text:setf("PAUSED", window_width, "center")
        love.graphics.draw(text, 0, text_y())
    end
end

return GameState
