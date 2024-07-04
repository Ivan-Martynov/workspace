local CommandInterface = require "command_interface"
local PauseCommand = CommandInterface:extend()
local SoundHelper = require "sound_helper"

local GameState = require "game_state"

function PauseCommand:init(toggle)
    self.parent.init(self)
    self.toggle = toggle
end

function PauseCommand:execute()
    if self.toggle then
        GameState.game_paused = not GameState.game_paused
    else
        GameState.game_paused = true
    end

    if GameState.game_paused then
        SoundHelper.backbround:pause()
    else
        SoundHelper.backbround:play()
    end
end

return PauseCommand
