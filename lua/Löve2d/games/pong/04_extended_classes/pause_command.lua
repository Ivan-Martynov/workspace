local CommandInterface = require "command_interface"
local PauseCommand = CommandInterface:extend()

function PauseCommand:init()
    self.parent.init(self)
end

function PauseCommand:execute()
end

return PauseCommand
