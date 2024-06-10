-- Use a function to initialize bar at the specified position.
return function(pos_x, pos_y)
    local entity = { x = pos_x, y = pos_y }

    entity.width = love.window.getMode()
    entity.height = 3

    function entity:draw()
        love.graphics.rectangle("fill", self.x, self.y, self.width, self.height)
    end

    return entity
end
