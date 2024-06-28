local ball = require "ball"
local platform = require "platform"
local bricks = require "bricks"
local walls = require "walls"
local collisions = require "collisions"
local levels = require "levels"

local gamestate = "menu"

function love.load()
    bricks.construct_level(levels.require_current_level())
    walls.construct_level()
end

function love.keyreleased(key)
    if gamestate == "menu" then
        if key == "return" then
            gamestate = "game"
        end
    elseif gamestate == "game" then
        if key == "p" then
            gamestate = "game_paused"
        end
    elseif gamestate == "game_paused" then
        if (key == "p") or (key == "return") then
            gamestate = "game"
        end
    elseif gamestate == "game_finished" then
        if key == "return" then
            levels.current_level = 1
            levels.game_finished = false
            bricks.construct_level(levels.require_current_level())
            ball.reposition()
            gamestate = "menu"
        end
    end

    if key == "c" then
        bricks.clear_current_level_bricks()
    elseif key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    if gamestate == "menu" then
        return
    elseif gamestate == "game" then
    elseif gamestate == "game_paused" then
        return
    elseif gamestate == "game_finished" then
        return
    end

    ball.update(dt)
    platform.update(dt)
    bricks.update(dt)
    walls.update(dt)

    collisions.resolve_collisions()

    levels.switch_to_next_level()
    if levels.game_finished then
    --if levels.current_level > #levels.sequence then
        gamestate = "game_finished"
    end
end

function love.draw()
    if gamestate == "menu" then
        love.graphics.print("Menu gamestate. Press Enter to continue.",
            280, 250)
        return
    elseif gamestate == "game" then
    elseif gamestate == "game_paused" then
        love.graphics.print("Game is paused. Press P or Enter to continue.",
            280, 250)
    elseif gamestate == "game_finished" then
        love.graphics.printf("Congratulations!\nYou have finished the game!\nPress enter to go to the main menu.",
            300, 250, 200, "center")
    end

    ball.draw()
    platform.draw()
    bricks.draw()
    walls.draw()
end

function love.quit(mode)
    if mode == "restart" then
        print("Try again!")
    else
        print("See ya!")
    end
end
