local world = require("world")

return function(pos_x, pos_y)
    local entity = {}

    entity.body = love.physics.newBody(world, pos_x, pos_y, "static")

    entity.shape = love.physics.newRectangleShape(10, 600)
    entity.fixture = love.physics.newFixture(entity.body, entity.shape)
    entity.fixture:setUserData(entity)

    return entity
end
