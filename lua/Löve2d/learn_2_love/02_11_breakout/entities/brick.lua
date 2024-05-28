local state = require("state")
local world = require("world")

return function(x_pos, y_pos)
    local entity = {}
    entity.body = love.physics.newBody(world, x_pos, y_pos, "static")

    entity.shape = love.physics.newRectangleShape(50, 20)

    entity.fixture = love.physics.newFixture(entity.body, entity.shape)
    entity.fixture:setUserData(entity)

    entity.type = "brick"
    entity.health = 2

    entity.draw = function(self)
        love.graphics.setColor(state.palette[entity.health] or state.palette[1])

        love.graphics.polygon("fill",
            self.body:getWorldPoints(self.shape:getPoints()))

        love.graphics.setColor(state.palette[5])
    end

    entity.end_contact = function(self)
        self.health = self.health - 1
    end

    return entity
end
