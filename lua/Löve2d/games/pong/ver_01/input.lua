local control = require "control"

local press_functions = {
    w = function ()
        control.move_left_paddle_up = true
    end,
    s = function()
        control.move_left_paddle_down = true
    end,
    up = function()
        control.move_right_paddle_up = true
    end,
    down = function()
        control.move_right_paddle_down = true
    end,

    space = function()
        control.game_paused = not control.game_paused
    end,

    escape = function()
        love.event.quit()
    end
}

local release_functions = {
    w = function ()
        control.move_left_paddle_up = false
    end,
    s = function()
        control.move_left_paddle_down = false
    end,
    up = function()
        control.move_right_paddle_up = false
    end,
    down = function()
        control.move_right_paddle_down = false
    end,
}

return {
    press = function(key)
        if press_functions[key] then
            press_functions[key]()
        end
    end,

    touch_press = function(_, x, y, _, _, _)
        control.touchpad_x = x
        control.touchpad_y = y
    end,

    touch_release = function(_, _, _, _, _, _)
        control.touchpad_x = nil
        control.touchpad_y = nil
    end,

    release = function(key)
        if release_functions[key] then
            release_functions[key]()
        end
    end,

    toggle_focus = function(focused)
        if not focused then
            control.game_paused = true
        end
    end,
}
