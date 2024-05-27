
local entities = require("entities")
local input = require("input")
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
    if input.paused then
        return
    end

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
            index = index + 1
        end
    end

    world:update(dt)
end

love.draw = function()
    for _, entity in ipairs(entities) do
        if entity.draw then
            entity:draw()
        end
    end
end
