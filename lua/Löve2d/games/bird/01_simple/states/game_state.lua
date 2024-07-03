local IGameState = require "states.state_interface"
local StateController = require "states.state_controller"
local CollisionHelper = require "collision_tools.collision_helper"
local KeyboardController = require "input.keyboard_controller"
local SoundHelper = require "audio_tools.sound_helper"

local background = require "entities.background"
local ground = require "entities.ground"
local pipes = require "entities.pipes"
local bird = require "entities.bird"

local GameState = IGameState:extend()

function GameState:load(do_reset)
    if do_reset then
        bird:reset()
        pipes:reset()
    end
    SoundHelper:play("background")
end

function GameState:focus(is_focused)
    if not is_focused then
        StateController:set_state("pause")
    end
end

function GameState:keypressed()
    if KeyboardController.key_pressed("escape") then
        love.event.quit()
    elseif KeyboardController.key_pressed("p") then
        StateController:set_state("pause")
    end
end

function GameState:update(dt)
    background:update(dt)
    ground:update(dt)
    pipes:update(dt)
    bird:update(dt)

    for _, pipe_pair in ipairs(pipes.pairs) do
        if (bird.y + bird.height >= ground.y) or
            CollisionHelper.AABB_collision(bird, pipe_pair.upper.tube) or
            CollisionHelper.AABB_collision(bird, pipe_pair.upper.collar) or
            CollisionHelper.AABB_collision(bird, pipe_pair.lower.tube) or
            CollisionHelper.AABB_collision(bird, pipe_pair.lower.collar) then
            SoundHelper:play("hit")
            StateController:set_state("gameover")
        elseif bird.y <= 0 then
            SoundHelper:play("bump")
        elseif not pipe_pair.scored
            and (bird.x > pipe_pair.upper.collar.x
                + pipe_pair.upper.collar.width) then
            SoundHelper:play("score")
            bird.score = bird.score + 1
            pipe_pair.scored = true
        end
    end
end

function GameState:draw()
    background:draw()
    ground:draw()
    pipes:draw()
    bird:draw()
end

return GameState
