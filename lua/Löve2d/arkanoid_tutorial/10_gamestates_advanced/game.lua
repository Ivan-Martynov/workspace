local gamestates = require "gamestates"

local game = {}

local ball = require "ball"
local platform = require "platform"
local bricks = require "bricks"
local walls = require "walls"
local collisions = require "collisions"
local levels = require "levels"

function game.keyreleased(key)
    if key == "c" then
        bricks.clear_current_level_bricks()
    elseif key == "p" then
        gamestates.set_state("game_paused")
    elseif key == "escape" then
        love.event.quit()
    end
end

function game.load()
    walls.construct_level()
end

function game.enter(_, params)
    if params then
        levels.current_level = params.level
        bricks.construct_level(levels.require_current_level())
        ball.reposition()
    end
end

function game.update(dt)
    ball.update(dt)
    platform.update(dt)
    bricks.update(dt)
    walls.update(dt)

    collisions.resolve_collisions()

    game.switch_to_next_level()
end

function game.switch_to_next_level()
    if bricks.no_more_bricks then
        if levels.current_level < #levels.sequence then
            gamestates.set_state("game", { level = levels.current_level + 1 })
        else
            gamestates.set_state("game_finished")
        end
    end
end

function game.draw()
    ball.draw()
    platform.draw()
    bricks.draw()
    walls.draw()
end

function game.exit()
end

return game
