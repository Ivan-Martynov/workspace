local state = require("state")
local world = require("world")

return function(pos_x, pos_y, is_bottom)
    local entity = {}

    entity.body = love.physics.newBody(world, pos_x, pos_y, "static")

    entity.shape = love.physics.newRectangleShape(800, 10)
    entity.fixture = love.physics.newFixture(entity.body, entity.shape)
    entity.fixture:setUserData(entity)

    if is_bottom then
        entity.end_contact = function (_)
            state.game_over = true
        end
    end

    return entity
end
