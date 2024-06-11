-- Ball object.
local ball = {
    position_x = 300,
    position_y = 300,
    speed_x = 300,
    speed_y = 300,
    radius = 10,
}

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

-- Platform object.
local platform = {
    position_x = 500,
    position_y = 500,
    speed_x = 300,
    width = 70,
    height = 20,
}

-- Update the platform.
function platform.update(dt)
    if love.keyboard.isDown("left") then
        platform.position_x = platform.position_x - platform.speed_x * dt
    elseif love.keyboard.isDown("right") then
        platform.position_x = platform.position_x + platform.speed_x * dt
    end
end

-- Draw the platform.
function platform.draw()
    love.graphics.rectangle("line", platform.position_x, platform.position_y,
        platform.width, platform.height)
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

function bricks.add_to_current_level_bricks(brick)
    table.insert(bricks.current_level_bricks, brick)
end

-- Create bricks.
function bricks.construct_level()
    for row = 1, bricks.rows do
        for col = 1, bricks.columns do
            local x = bricks.top_left_position_x
                + (col - 1) * (bricks.brick_width + bricks.horizontal_gap)
            local y = bricks.top_left_position_y
                + (row - 1) * (bricks.brick_height + bricks.vertical_gap)
            bricks.add_to_current_level_bricks(bricks.new_brick(x, y))
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
function collisions.check_rectangles_overlap(first, second)
    return (first.position_x + first.width >= second.position_x)
        and (first.position_x <= second.position_x + second.width)
        and (first.position_y + first.height >= second.position_y)
        and (first.position_y <= second.position_y + second.height)
end

-- Treat a ball as a rect.
function collisions.ball_platform_collision(b, p)
    local ball_rect = {
        position_x = b.position_x - b.radius,
        position_y = b.position_y - b.radius,
        width = b.radius * 2,
        height = b.radius * 2,
    }

    if collisions.check_rectangles_overlap(ball_rect, p) then
        love.graphics.print("Ball-platform collision", 10, 10)
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
        if collisions.check_rectangles_overlap(ball_rect, wall) then
            love.graphics.print("Ball-wall collision", 10, 10)
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

    for _, brick in ipairs(bs.current_level_bricks) do
        if collisions.check_rectangles_overlap(ball_rect, brick) then
            love.graphics.print("Ball-brick collision", 10, 10)
        end
    end
end

function collisions.platform_walls_collision(p, w)
    for _, wall in pairs(w.current_level_walls) do
        if collisions.check_rectangles_overlap(p, wall) then
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
end

function love.draw()
    ball.draw()
    platform.draw()
    bricks.draw()
    walls.draw()
end

function love.quit(mode)
    if mode == "restart" then
        print("Try again!")
    else
        print("See ya!")
    end
end
