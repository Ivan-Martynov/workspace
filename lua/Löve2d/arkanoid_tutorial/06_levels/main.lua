-- Levels.
local levels = {}

levels.sequence = {}

levels.sequence[1] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1 },
    { 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 },
    { 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0 },
    { 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0 },
    { 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
}

levels.sequence[2] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1 },
    { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0 },
    { 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0 },
    { 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },
    { 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
}

levels.current_level = 1

-- Platform object.
local platform = {
    position_x = 500,
    position_y = 500,
    speed_x = 300,
    width = 70,
    height = 20,
    moving_to_the_left = false,
    moving_to_the_right = false,
}

function platform.rebound(shift)
    local min_shift = math.min(math.abs(shift.x), math.abs(shift.y))

    if math.abs(shift.x) == min_shift then
        shift.y = 0
    else
        shift.x = 0
    end

    platform.position_x = platform.position_x + shift.x
    platform.position_y = platform.position_y + shift.y
end

-- Update the platform.
function platform.update(dt)
    if love.keyboard.isDown("left") then
        platform.moving_to_the_left = true
        platform.moving_to_the_right = false

        platform.position_x = platform.position_x - platform.speed_x * dt
    elseif love.keyboard.isDown("right") then
        platform.moving_to_the_left = false
        platform.moving_to_the_right = true

        platform.position_x = platform.position_x + platform.speed_x * dt
    else
        platform.moving_to_the_left = false
        platform.moving_to_the_right = false
    end
end

-- Draw the platform.
function platform.draw()
    love.graphics.rectangle("line", platform.position_x, platform.position_y,
        platform.width, platform.height)
end

-- Ball object.
local ball = {
    position_x = platform.position_x + platform.width / 2,
    position_y = platform.position_y,
    speed_x = 300,
    speed_y = -300,
    radius = 10,
}
ball.position_y = ball.position_y - ball.radius * 2

function ball.reposition()
    ball.position_x = platform.position_x + platform.width / 2
    ball.position_y = platform.position_y
end

function ball.rebound(shift, toggle_speed_x)
    if toggle_speed_x == nil then
        toggle_speed_x = true
    end

    local min_shift = math.min(math.abs(shift.x), math.abs(shift.y))

    if math.abs(shift.x) == min_shift then
        shift.y = 0
    else
        shift.x = 0
    end

    ball.position_x = ball.position_x + shift.x * 2
    ball.position_y = ball.position_y + shift.y * 2

    if (shift.x ~= 0) and toggle_speed_x then
        ball.speed_x = -ball.speed_x
    end
    if shift.y ~= 0 then
        ball.speed_y = -ball.speed_y
    end
end

-- Update the ball.
function ball.update(dt)
    ball.position_x = ball.position_x + ball.speed_x * dt
    ball.position_y = ball.position_y + ball.speed_y * dt 
end

-- Draw the ball.
function ball.draw()
    local circle_segment_count = 16
    love.graphics.circle("line", ball.position_x, ball.position_y, ball.radius,
        circle_segment_count)
end

-- All bricks.
local bricks = {
    rows = 8,
    columns = 11,
    top_left_position_x = 70,
    top_left_position_y = 50,
    brick_width = 50,
    brick_height = 30,
    horizontal_gap = 10,
    vertical_gap = 15,
    current_level_bricks = {},
}

function bricks.new_brick(x, y, width, height)
    return {
        position_x = x,
        position_y = y,
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

            local x = bricks.top_left_position_x +
                (col_index - 1) * (bricks.brick_width + bricks.horizontal_gap)
            local y = bricks.top_left_position_y +
                (row_index - 1) * (bricks.brick_height + bricks.vertical_gap)

            bricks.add_to_current_level_bricks(bricks.new_brick(x, y))

            ::continue::
        end
    end
end

function bricks.update_brick(_)
end

function bricks.draw_brick(brick)
    love.graphics.rectangle("line", brick.position_x, brick.position_y,
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

-- All walls.
local walls = {
    wall_width = 50,
    wall_height = 30,
    wall_thickness = 2,
    current_level_walls = {},
}

function walls.new_wall(x, y, width, height)
    return {
        position_x = x,
        position_y = y,
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
    love.graphics.rectangle("line", wall.position_x, wall.position_y,
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

local collisions = {}

-- Simple AABB collision detection.
local function overlap_along_axis(a_pos, b_pos, a_size, b_size)
    local shift = (a_size + b_size) / 2 - math.abs(a_pos - b_pos)
    return shift, a_pos > b_pos and shift or -shift
end

function collisions.check_rectangles_overlap(first, second)
    local overlap_x, shift_x = overlap_along_axis(
        first.position_x + first.width / 2,
        second.position_x + second.width / 2, first.width, second.width)
    local overlap_y, shift_y = overlap_along_axis(
        first.position_y + first.height / 2,
        second.position_y + second.height / 2, first.height, second.height)
    return (overlap_x > 0 and overlap_y > 0), shift_x, shift_y
end

-- Treat a ball as a rect.
function collisions.ball_platform_collision(b, p)
    local ball_rect = {
        position_x = b.position_x - b.radius,
        position_y = b.position_y - b.radius,
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
        position_x = b.position_x - b.radius,
        position_y = b.position_y - b.radius,
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
        position_x = b.position_x - b.radius,
        position_y = b.position_y - b.radius,
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

function levels.switch_to_next_level()
    if bricks.no_more_bricks then
        if levels.current_level < #levels.sequence then
            levels.current_level = levels.current_level + 1
            bricks.construct_level(levels.sequence[levels.current_level])
            ball.reposition()
        else
            levels.game_finished = true
        end
    end
end

function love.load()
    bricks.construct_level()
    walls.construct_level()
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    ball.update(dt)
    platform.update(dt)
    bricks.update(dt)
    walls.update(dt)

    collisions.resolve_collisions()

    levels.switch_to_next_level()
end

function love.draw()
    ball.draw()
    platform.draw()
    bricks.draw()
    walls.draw()

    if levels.game_finished then
        love.graphics.printf("Congratulations!\nYou have finished the game!",
            300, 250, 200, "center")
    end
end

function love.quit(mode)
    if mode == "restart" then
        print("Try again!")
    else
        print("See ya!")
    end
end
