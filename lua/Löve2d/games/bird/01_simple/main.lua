local ColorSchemeHelper = require "color_tools.color_scheme_helper"
ColorSchemeHelper:set_scheme(ColorSchemeHelper.solarized_dark)

local menu_state = require "states.menu_state"

function love.load()
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    menu_state:update(dt)
end

function love.draw()
    menu_state:draw()
end
