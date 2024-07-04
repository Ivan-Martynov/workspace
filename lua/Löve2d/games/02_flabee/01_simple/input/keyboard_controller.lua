local IInputController = require "input.input_controller_interface"
local KeyboardController = IInputController:extend()

function KeyboardController.key_pressed(key, ...)
    return love.keyboard.isDown(key, ...)
end

function KeyboardController.one_of_keys_pressed(keys)
    for _, key in ipairs(keys) do
        if KeyboardController.key_pressed(key) then
            return true
        end
    end
    return false
end

function KeyboardController:update()
end

return KeyboardController
