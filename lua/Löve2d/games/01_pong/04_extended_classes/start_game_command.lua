local CommandInterface = require "command_interface"
local StartGameCommand = CommandInterface:extend()
local GameState = require "game_state"

function StartGameCommand:init(list_of_paddles)
    self.parent.init(self)
    self.paddles = list_of_paddles
end

function StartGameCommand:execute()
    GameState.game_started = true
    GameState.game_over = false
    GameState.win_message = nil

    for _, paddle in ipairs(self.paddles) do
        paddle:reset()
    end
end

return StartGameCommand
