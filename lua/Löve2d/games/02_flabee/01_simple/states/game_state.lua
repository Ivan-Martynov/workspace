local IGameState = require "states.state_interface"
local StateController = require "states.state_controller"
local CollisionHelper = require "collision_tools.collision_helper"
local KeyboardController = require "input.keyboard_controller"
local SoundHelper = require "audio_tools.sound_helper"

local QuitGameCommand = require "commands.quit_game_command"

local background = require "entities.background"
local ground = require "entities.ground"
local pipes = require "entities.pipes"
local bee = require "entities.bee"

local entities = { background, ground, pipes, bee }

local PauseState = require "states.pause_state"
local GameState = IGameState:extend()

function GameState:load()
    self.entities = entities
    SoundHelper:play("background")
end

function GameState:focus(is_focused)
    if not is_focused then
        StateController:set_state("pause", self)
    end
end

function GameState:keypressed()
    if KeyboardController.key_pressed("escape") then
        QuitGameCommand():execute()
    elseif KeyboardController.one_of_keys_pressed(PauseState.keys) then
        StateController:set_state("pause", self)
    end
end

function GameState:update(dt)
    background:update(dt)
    ground:update(dt)
    pipes:update(dt)
    bee:update(dt)

    for _, pipe_pair in ipairs(pipes.pairs) do
        if (bee.y + bee.height >= ground.y) or
            CollisionHelper.AABB_collision(bee, pipe_pair.upper.tube) or
            CollisionHelper.AABB_collision(bee, pipe_pair.upper.collar) or
            CollisionHelper.AABB_collision(bee, pipe_pair.lower.tube) or
            CollisionHelper.AABB_collision(bee, pipe_pair.lower.collar) then
            SoundHelper:play("hit")
            StateController:set_state("gameover")
        elseif bee.y <= 0 then
            SoundHelper:play("bump")
        elseif not pipe_pair.scored
            and (bee.x > pipe_pair.upper.collar.x
                + pipe_pair.upper.collar.width) then
            SoundHelper:play("score")
            bee.score = bee.score + 1
            pipe_pair.scored = true
        end
    end
end

function GameState:draw()
    background:draw()
    ground:draw()
    pipes:draw()
    bee:draw()
end

return GameState
