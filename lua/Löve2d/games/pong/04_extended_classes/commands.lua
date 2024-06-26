local StartGameCommand = require "start_game_command"
local GameOverCommand = require "game_over_command"
local QuitGameCommand = require "quit_game_command"
local PauseCommand = require "pause_command"

local commands = {}

function commands.add_command(key, command)
    commands[key] = command
end

return commands
