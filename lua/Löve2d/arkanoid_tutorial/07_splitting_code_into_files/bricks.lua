local vector = require "vector"

-- All bricks.
local bricks = {
    rows = 8,
    columns = 11,
    top_left_position = vector(70, 50),
    brick_width = 50,
    brick_height = 30,
    horizontal_gap = 10,
    vertical_gap = 15,
    current_level_bricks = {},
}

function bricks.new_brick(x, y, width, height)
    return {
        position = vector(x, y),
        width = width or bricks.brick_width,
        height = height or bricks.brick_height,
    }
end

function bricks.brick_hit_by_ball(i)
    table.remove(bricks.current_level_bricks, i)
end

function bricks.add_to_current_level_bricks(brick)
    table.insert(bricks.current_level_bricks, brick)
end

-- Create bricks.
function bricks.construct_level(level_bricks_arrangement)
    bricks.no_more_bricks = false

    for row_index, row in ipairs(level_bricks_arrangement) do
        for col_index, brick_type in ipairs(row) do
            if brick_type == 0 then
                goto continue
            end

            local x = bricks.top_left_position.x +
                (col_index - 1) * (bricks.brick_width + bricks.horizontal_gap)
            local y = bricks.top_left_position.y +
                (row_index - 1) * (bricks.brick_height + bricks.vertical_gap)

            bricks.add_to_current_level_bricks(bricks.new_brick(x, y))

            ::continue::
        end
    end
end

function bricks.update_brick(_)
end

function bricks.draw_brick(brick)
    love.graphics.rectangle("line", brick.position.x, brick.position.y,
        brick.width, brick.height)
end

function bricks.update(_)
    bricks.no_more_bricks = #bricks.current_level_bricks == 0

    for _, brick in ipairs(bricks.current_level_bricks) do
        bricks.update_brick(brick)
    end
end

function bricks.draw()
    for _, brick in ipairs(bricks.current_level_bricks) do
        bricks.draw_brick(brick)
    end
end

return bricks
