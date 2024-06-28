local ball = require "ball"
local platform = require "platform"
local bricks = require "bricks"
local walls = require "walls"
local collisions = require "collisions"
local levels = require "levels"

function love.load()
    bricks.construct_level(levels.sequence[levels.current_level])
    walls.construct_level()
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    ball.update(dt)
    platform.update(dt)
    bricks.update(dt)
    walls.update(dt)

    collisions.resolve_collisions()

    levels.switch_to_next_level()
end

function love.draw()
    ball.draw()
    platform.draw()
    bricks.draw()
    walls.draw()

    if levels.game_finished then
        love.graphics.printf("Congratulations!\nYou have finished the game!",
            300, 250, 200, "center")
    end
end

function love.quit(mode)
    if mode == "restart" then
        print("Try again!")
    else
        print("See ya!")
    end
end
