local GameState = require "game_state"
local commands = require "commands"
local MouseController = {}

local last_click_time = 0
local double_click_interval = 0.4 -- interval in seconds.

function MouseController.button_pressed(button, ...)
    return love.mouse.isDown(button, ...)
end

function MouseController.get_coordinates()
    return love.mouse.getPosition()
end

function MouseController.update()
    if (not GameState.game_started or GameState.game_over) then
        local t = love.timer.getTime()
        if t <= last_click_time + double_click_interval then
            commands.start_game:execute()
        else
            last_click_time = t
        end
    end
end


return MouseController
