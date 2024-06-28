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

function bricks.new_brick(x, y, width, height, color_index)
    return {
        position = vector(x, y),
        width = width or bricks.brick_width,
        height = height or bricks.brick_height,
        color_index = color_index,
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
            bricks.add_to_current_level_bricks(bricks.new_brick(x, y,
                bricks.brick_width, bricks.brick_height, tonumber(brick_type)))

            ::continue::
        end
    end
end

function bricks.clear_current_level_bricks()
    for i in ipairs(bricks.current_level_bricks) do
        bricks.current_level_bricks[i] = nil
    end
end

function bricks.update_brick(_)
end

function bricks.draw_brick(brick)
    local r, g, b, a = love.graphics.getColor(1, 0, 0, 1)
    local alpha = 0.5
    if brick.color_index == 1 then
        love.graphics.setColor(1, 0, 0, alpha)
    elseif brick.color_index == 2 then
        love.graphics.setColor(0, 1, 0, alpha)
    elseif brick.color_index == 3 then
        love.graphics.setColor(0, 0, 1, alpha)
    end

    love.graphics.rectangle("fill", brick.position.x, brick.position.y,
        brick.width, brick.height)

    love.graphics.setColor(r, g, b, a)

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
