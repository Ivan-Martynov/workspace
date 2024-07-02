local MenuState = require "states.menu_state"

local StateController = {
    menu = function()
        return MenuState()
    end,
}

function StateController:set_state(name)
    StateController.current = StateController[name]
end

return StateController
