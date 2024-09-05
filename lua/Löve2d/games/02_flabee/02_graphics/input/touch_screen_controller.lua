local IInputController = require "input.input_controller_interface"
local TouchScreenController = IInputController:extend()

function TouchScreenController.get_touch_coordinates(touch_id)
    return love.touch.getPosition(touch_id)
end

function TouchScreenController.get_all_touches()
    return love.touch.getTouches()
end

return TouchScreenController
