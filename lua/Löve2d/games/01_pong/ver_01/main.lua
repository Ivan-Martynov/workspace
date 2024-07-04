local entities = require "entities"
local input = require "input"
local control = require "control"

local window_width, window_height = 800, 600

-- Load the window.
function love.load()
    love.window.setMode(window_width, window_height,
        {
            resizable = true
        })
end

-- Keypressed callback.
function love.keypressed(key)
    input.press(key)
end

function love.touchpressed(id, x, y, dx, dy, pressure)
    input.touch_press(id, x, y, dx, dy, pressure)
end

function love.touchreleased(id, x, y, dx, dy, pressure)
    input.touch_release(id, x, y, dx, dy, pressure)
end

-- Keyreleased callback.
function love.keyreleased(key)
    input.release(key)
end

-- Focus callback.
function love.focus(focused)
    input.toggle_focus(focused)
end

-- Game loop.
function love.update(dt)
    if control.game_paused then
        return
    end

    entities.update(dt)
end

-- Main rendering function.
function love.draw()
    entities.draw()
end
