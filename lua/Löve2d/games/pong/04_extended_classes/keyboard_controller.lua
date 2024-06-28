local GameState = require "game_state"
local commands = require "commands"
local KeyboardController = {}

function KeyboardController.key_pressed(key, ...)
    return love.keyboard.isDown(key, ...)
end

function KeyboardController.update()
    if KeyboardController.key_pressed("return") and
        (not GameState.game_started or GameState.game_over) then
        commands.start_game:execute()
    elseif KeyboardController.key_pressed("p") and GameState.game_started then
        commands.pause_toggle_game:execute()
    elseif KeyboardController.key_pressed("escape") then
        commands.quit_game:execute()
    end
end


return KeyboardController
