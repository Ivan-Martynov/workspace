local CommandInterface = require "command_interface"
local GameOverCommand = CommandInterface:extend()
local GameState = require "game_state"

function GameOverCommand:init(left_paddle_won)
    self.parent.init(self)
    self.left_paddle_won = left_paddle_won
end

function GameOverCommand:execute()
    if self.left_paddle_won then
        GameState.win_message = "Left paddle won!"
    else
        GameState.win_message = "Right paddle won!"
    end
    GameState.win_message = GameState.win_message ..
        "\nPress enter/return to play again."
    GameState.game_over = true
end

return GameOverCommand
