local IInputController = require "input.input_controller_interface"
local KeyboardController = IInputController:extend()

function KeyboardController.key_pressed(key, ...)
    return love.keyboard.isDown(key, ...)
end

function KeyboardController:update()
end

return KeyboardController
