local TouchScreenController = require "touch_screen_controller"
local MouseController = require "mouse_controller"
local KeyboardController = require "keyboard_controller"

local InputController = {
    KeyboardController = KeyboardController,
    MouseController = MouseController,
    TouchScreenController = TouchScreenController,
}

return InputController
