local entities = require("entities")
local input = require("input")
local state = require("state")
local world = require("world")

love.keypressed = function(key)
    input.press(key)
end

love.keyreleased = function(key)
    input.release(key)
end

love.focus = function(focused)
    input.toggle_focus(focused)
end

love.update = function(dt)
    if state.game_over or state.stage_cleared or state.paused then
        return
    end

    local no_bricks = true

    local index = 1
    while index <= #entities do
        local entity = entities[index]

        if entity.update then
            entity:update(dt)
        end

        if entity.health == 0 then
            table.remove(entities, index)
            entity.fixture:destroy()
        else
            if entity.type == "brick" then
                no_bricks = false
            end
            index = index + 1
        end
    end

    state.stage_cleared = no_bricks

    world:update(dt)
end

love.draw = function()
    for _, entity in ipairs(entities) do
        if entity.draw then
            entity:draw()
        end
    end
end
