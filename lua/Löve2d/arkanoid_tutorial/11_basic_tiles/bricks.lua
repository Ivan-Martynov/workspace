local vector = require "vector"

-- All bricks.
local bricks = {
    image = love.graphics.newImage("images/bricks.png"),
    tile_width = 64,
    tile_height = 32,
    tileset_width = 384,
    tileset_height = 160,
    rows = 11,
    columns = 8,
    top_left_position = vector(47, 34),
    horizontal_gap = 0,
    vertical_gap = 0,
    current_level_bricks = {},
}
bricks.brick_width = bricks.tile_width
bricks.brick_height = bricks.tile_height

function bricks.bricktype_to_quad(brick_type)
    if (brick_type == nil) or (brick_type <= 10) then
        return nil
    end

    local row = math.floor(brick_type / 10)
    local col = brick_type % 10
    local x_pos = bricks.tile_width * (col - 1)
    local y_pos = bricks.tile_height * (row - 1)

    return love.graphics.newQuad(x_pos, y_pos,
        bricks.tile_width, bricks.tile_height,
        bricks.tileset_width, bricks.tileset_height)
end

function bricks.new_brick(x, y, width, height, brick_type)
    return {
        position = vector(x, y),
        width = width or bricks.brick_width,
        height = height or bricks.brick_height,
        brick_type = brick_type,
        quad = bricks.bricktype_to_quad(brick_type),
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
                bricks.brick_width, bricks.brick_height, brick_type))

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
    love.graphics.rectangle("line", brick.position.x, brick.position.y,
        brick.width, brick.height)

    if brick.quad then
        love.graphics.draw(bricks.image, brick.quad,
            brick.position.x, brick.position.y)
    end
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
