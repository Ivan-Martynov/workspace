local state = require("state")
local world = require("world")

return function(pos_x, pos_y)
    local window_width = love.window.getMode()

    local entity_width = 180
    local entity_height = 20
    local entity_speed = 600

    local left_boundary = entity_width / 2 + 2
    local right_boundary = window_width - entity_width / 2 - 2

    local entity = {}
    entity.body = love.physics.newBody(world, pos_x, pos_y, "kinematic")
    entity.shape = love.physics.newRectangleShape(entity_width, entity_height)

    entity.fixture = love.physics.newFixture(entity.body, entity.shape)
    entity.fixture:setUserData(entity)

    entity.update = function(self, _)
        if state.button_left and state.button_right then
            return
        end

        --local self_x, self_y = self.body:getPosition()
        local self_x = self.body:getX()

        if state.button_left and (self_x > left_boundary) then
            self.body:setLinearVelocity(-entity_speed, 0)
        elseif state.button_right and (self_x < right_boundary) then
            self.body:setLinearVelocity(entity_speed, 0)
        else
            self.body:setLinearVelocity(0, 0)
        end
    end

    entity.draw = function(self)
        love.graphics.polygon("line",
            self.body:getWorldPoints(self.shape:getPoints()))
    end

    return entity
end
