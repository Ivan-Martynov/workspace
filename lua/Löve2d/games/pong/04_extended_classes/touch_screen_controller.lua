local TouchScreenController = {}
local GameState = require "game_state"
local commands = require "commands"

local last_touch_time = 0
local double_touch_interval = 0.5 -- interval in seconds.

function TouchScreenController.get_touch_coordinates(touch_id)
    return love.touch.getPosition(touch_id)
end

function TouchScreenController.get_all_touches()
    return love.touch.getTouches()
end

function TouchScreenController.update()
    if not GameState.game_started or GameState.game_over then
        local t = love.timer.getTime()
        if t <= last_touch_time + double_touch_interval then
            commands.start_game:execute()
        else
            last_touch_time = t
        end
    end
end

return TouchScreenController
