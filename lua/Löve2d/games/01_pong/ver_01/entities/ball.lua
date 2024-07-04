local function clamp(x, a, b)
    if x < a then
        return a
    elseif x > b then
        return b
    else
        return x
    end
end

-- Use a function to initialize ball at the specified position.
return function(pos_x, pos_y)
    -- Initialize ball's position, velocity and radius.
    local entity = { x = pos_x, y = pos_y, dx = 150, dy = 150, radius = 5 }

    local window_width, window_height = love.window.getMode()

    function entity:reset()
        self.x = window_width / 2
        self.y = window_height / 2

        self.dx = love.math.random(150, 200)
        if love.math.random(2) == 1 then
            self.dx = -self.dx
        end

        self.dy = love.math.random(150, 200)
        if love.math.random(2) == 1 then
            self.dy = -self.dy
        end
    end

    entity:reset()

    --function entity:resolve_collision_with_rect(rect)
    --    local closest_x = clamp(self.x, rect.x, rect.x + rect.width)
    --    local closest_y = clamp(self.y, rect.y, rect.y + rect.height)

    --    if (self.x - closest_x) ^ 2 + (self.y - closest_y) ^ 2
    --        <= self.radius ^ 2 then
    --    end
    --end

    -- Update ball's state.
    function entity:update(dt)
        self.x = self.x + self.dx * dt;
        self.y = self.y + self.dy * dt;
    end

    -- Define how to draw the ball.
    function entity:draw()
        love.graphics.circle("fill", self.x, self.y, self.radius)
    end

    return entity
end
