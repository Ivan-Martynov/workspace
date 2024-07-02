local BirdClass = require "entities.bird"
local BackgroundClass = require "entities.background"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"

local _, window_height = love.window.getMode()

local entities = {
    --background = BackgroundClass(0, 0, ColorSchemeHelper.current.blue),
    --bird = BirdClass(100, window_height / 2, ColorSchemeHelper.current.yellow),
}

function entities.draw()
    --entities.background:draw()
    --entities.bird:draw()
end

return entities
