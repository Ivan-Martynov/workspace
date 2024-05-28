local world = require("world")

return function(pos_x, pos_y)
    local entity_max_speed = 888

    local entity = {}
    entity.body = love.physics.newBody(world, pos_x, pos_y, "dynamic")
    entity.body:setMass(32)
    entity.body:setLinearVelocity(400, 400)

    entity.shape = love.physics.newCircleShape(0, 0, 10)

    entity.fixture = love.physics.newFixture(entity.body, entity.shape)
    entity.fixture:setFriction(0)
    entity.fixture:setRestitution(1)
    entity.fixture:setUserData(entity)

    local font = love.graphics.newFont(12)
    entity.draw = function(self)
        local x, y = self.body:getWorldCenter()
        love.graphics.circle("fill", x, y, self.shape:getRadius())

        local vel_x, vel_y = self.body:getLinearVelocity()
        local speed = math.abs(vel_x) + math.abs(vel_y)
        local window_width = love.window.getMode()
        love.graphics.setFont(font)
        love.graphics.printf("Speed = " .. speed, 10, 10,
            window_width, "left")
    end

    entity.update = function(self, _)
        local vel_x, vel_y = self.body:getLinearVelocity()
        local speed = math.abs(vel_x) + math.abs(vel_y)

        local vel_x_is_critical = math.abs(vel_x) > entity_max_speed * 2
        local vel_y_is_critical = math.abs(vel_y) > entity_max_speed * 2

        if vel_x_is_critical or vel_y_is_critical then
            self.body:setLinearVelocity(vel_x * 0.75, vel_y * 0.75)
        end

        self.body:setLinearDamping(speed > entity_max_speed and 0.1 or 0)
    end

    return entity
end
