local vector = require "vector"

local ball = require "ball"
local platform = require "platform"
local bricks = require "bricks"
local walls = require "walls"

local collisions = {}

-- Simple AABB collision detection.
local function overlap_along_axis(a_pos, b_pos, a_size, b_size)
    local shift = (a_size + b_size) / 2 - math.abs(a_pos - b_pos)
    return shift, a_pos > b_pos and shift or -shift
end

function collisions.check_rectangles_overlap(first, second)
    local overlap_x, shift_x = overlap_along_axis(
        first.position.x + first.width / 2,
        second.position.x + second.width / 2, first.width, second.width)
    local overlap_y, shift_y = overlap_along_axis(
        first.position.y + first.height / 2,
        second.position.y + second.height / 2, first.height, second.height)
    return (overlap_x > 0 and overlap_y > 0), shift_x, shift_y
end

-- Treat a ball as a rect.
function collisions.ball_platform_collision(b, p)
    local ball_rect = {
        position = vector(b.position.x - b.radius, b.position.y - b.radius),
        width = b.radius * 2,
        height = b.radius * 2,
    }

    local overlap, shift_x, shift_y =
        collisions.check_rectangles_overlap(ball_rect, p)
    if overlap then
        if (platform.moving_to_the_left and (ball.speed_x < 0)) or
            (platform.moving_to_the_right and (ball.speed_x > 0)) then
            ball.rebound({ x = shift_x, y = shift_y }, false)
        else
            ball.rebound({ x = shift_x, y = shift_y })
        end
    end
end

function collisions.ball_walls_collision(b, w)
    local ball_rect = {
        position = vector(b.position.x - b.radius, b.position.y - b.radius),
        width = b.radius * 2,
        height = b.radius * 2,
    }

    for _, wall in pairs(w.current_level_walls) do
        local overlap, shift_x, shift_y =
            collisions.check_rectangles_overlap(ball_rect, wall)

        if overlap then
            ball.rebound({ x = shift_x, y = shift_y })
        end
    end
end

function collisions.ball_bricks_collision(b, bs)
    local ball_rect = {
        position = vector(b.position.x - b.radius, b.position.y - b.radius),
        width = b.radius * 2,
        height = b.radius * 2,
    }

    for i, brick in ipairs(bs.current_level_bricks) do
        local overlap, shift_x, shift_y =
            collisions.check_rectangles_overlap(ball_rect, brick)

        if overlap then
            ball.rebound({ x = shift_x, y = shift_y })
            bricks.brick_hit_by_ball(i)
        end
    end
end

function collisions.platform_walls_collision(p, w)
    for _, wall in pairs(w.current_level_walls) do
        local overlap, shift_x, shift_y =
            collisions.check_rectangles_overlap(p, wall)
        if overlap then
            platform.rebound({ x = shift_x, y = shift_y })
            love.graphics.print("Platform-wall collision", 10, 10)
        end
    end
end

function collisions.resolve_collisions()
    collisions.ball_platform_collision(ball, platform)
    collisions.ball_walls_collision(ball, walls)
    collisions.ball_bricks_collision(ball, bricks)
    collisions.platform_walls_collision(platform, walls)
end

return collisions
