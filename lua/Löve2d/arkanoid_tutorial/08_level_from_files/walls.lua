local vector = require "vector"

-- All walls.
local walls = {
    wall_width = 50,
    wall_height = 30,
    wall_thickness = 2,
    current_level_walls = {},
}

function walls.new_wall(x, y, width, height)
    return {
        position = vector(x, y),
        width = width,
        height = height,
    }
end

function walls.construct_level()
    local window_width, window_height = love.window.getMode()

    walls.current_level_walls["left"] = walls.new_wall(0, 0,
        walls.wall_thickness, window_height)
    walls.current_level_walls["right"] = walls.new_wall(
        window_width - walls.wall_thickness, 0,
        walls.wall_thickness, window_height)
    walls.current_level_walls["top"] = walls.new_wall(0, 0,
        window_width, walls.wall_thickness)
    walls.current_level_walls["bottom"] = walls.new_wall(0,
        window_height - walls.wall_thickness,
        window_width, walls.wall_thickness)
end

function walls.update_wall(_)
end

function walls.draw_wall(wall)
    love.graphics.rectangle("line", wall.position.x, wall.position.y,
        wall.width, wall.height)
end

function walls.update(_)
    for _, wall in pairs(walls.current_level_walls) do
        walls.update_wall(wall)
    end
end

function walls.draw()
    for _, wall in pairs(walls.current_level_walls) do
        walls.draw_wall(wall)
    end
end

return walls
