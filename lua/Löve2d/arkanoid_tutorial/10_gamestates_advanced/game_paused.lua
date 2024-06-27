local gamestates = require "gamestates"
local game_paused = {}

local ball = require "ball"
local platform = require "platform"
local bricks = require "bricks"
local walls = require "walls"

function game_paused.load()
end

function game_paused.enter()
end

function game_paused.keyreleased(key)
    if (key == "p") or (key == "return") then
        gamestates.set_state("game")
    elseif key == "escape" then
        love.event.quit()
    end
end

function game_paused.update(_)
end

function game_paused.draw()
    ball.draw()
    platform.draw()
    bricks.draw()
    walls.draw()

    -- Add a tint.
    local r, g, b, a = love.graphics.getColor()
    local window_width, window_height = love.window.getMode()
    love.graphics.setColor(r, g, b, 0.2)
    love.graphics.rectangle("fill", 0, 0, window_width, window_height)
    love.graphics.setColor(r, g, b, a)

    love.graphics.print("Game is paused. Press P or Enter to continue.",
        280, 250)
end

function game_paused.exit()
end

return game_paused
