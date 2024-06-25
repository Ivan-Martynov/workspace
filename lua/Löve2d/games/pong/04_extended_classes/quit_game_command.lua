local CommandInterface = require "command_interface"
local QuitGameCommand = CommandInterface:extend()

function QuitGameCommand:init(exit_status)
    self.parent.init(self)
    self.exit_status = exit_status
end

function QuitGameCommand:execute()
    love.event.quit(self.exit_status)
end

return QuitGameCommand
