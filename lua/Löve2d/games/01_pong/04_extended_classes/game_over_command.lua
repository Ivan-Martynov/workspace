local CommandInterface = require "command_interface"
local GameOverCommand = CommandInterface:extend()
local GameState = require "game_state"

function GameOverCommand:init(left_paddle)
    self.parent.init(self)
    self.left_paddle = left_paddle
end

function GameOverCommand:execute()
    if self.left_paddle.score == GameState.score_to_win then
        GameState.win_message = "Left paddle won!"
    else
        GameState.win_message = "Right paddle won!"
    end
    GameState.win_message = GameState.win_message .. "\n" ..
        GameState.start_message
    GameState.game_over = true
end

return GameOverCommand
