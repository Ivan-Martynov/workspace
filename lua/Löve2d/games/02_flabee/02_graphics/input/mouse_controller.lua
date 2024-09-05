local IInputController = require "input.input_controller_interface"
local MouseController = IInputController:extend()

function MouseController.button_pressed(button, ...)
    return love.mouse.isDown(button, ...)
end

function MouseController.get_coordinates()
    return love.mouse.getPosition()
end

return MouseController
