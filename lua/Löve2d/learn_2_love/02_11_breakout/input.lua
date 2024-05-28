local state = require("state")

local press_functions = {
    left = function()
        state.button_left = true
    end,

    right = function()
        state.button_right = true
    end,

    escape = function()
        love.event.quit()
    end,

    space = function()
        if state.game_over or state.stage_cleared then
            return
        else
            state.paused = not state.paused
        end
    end,

    enter = function()
        if state.game_over or state.stage_cleared then
            love.event.quit("restart")
        end
    end
}

press_functions["return"] = press_functions.enter

local release_functions = {
    left = function()
        state.button_left = false
    end,

    right = function()
        state.button_right = false
    end
}

return {
    press = function(key)
        if press_functions[key] then
            press_functions[key]()
        end
    end,

    release = function(key)
        if release_functions[key] then
            release_functions[key]()
        end
    end,

    toggle_focus = function(focused)
        if not focused then
            state.paused = true
        end
    end
}
