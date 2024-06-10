-- Use a function to initialize paddle at the specified position.
return function(pos_x, pos_y)
    local speed = 200
    local entity = { x = pos_x, y = pos_y, dy = 0 }

    entity.width = 10
    entity.height = 50

    function entity.move(self, up, down)
        if up then
            self.dy = -speed
        elseif down then
            self.dy = speed
        else
            self.dy = 0
        end
    end

    -- Update paddle's state.
    function entity:update(dt)
        self.y = self.y + self.dy * dt;
    end

    function entity:draw()
        love.graphics.rectangle("fill", self.x, self.y, self.width, self.height)
    end

    return entity
end

