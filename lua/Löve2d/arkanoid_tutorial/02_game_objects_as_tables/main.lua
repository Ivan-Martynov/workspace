local ball = {
    position_x = 300,
    position_y = 300,
    speed_x = 300,
    speed_y = 300,
    radius = 10,
}

function ball.update(dt)
    ball.position_x = ball.position_x + ball.speed_x * dt
    ball.position_y = ball.position_y + ball.speed_y * dt 
end

function ball.draw()
    local circle_segment_count = 16
    love.graphics.circle("line", ball.position_x, ball.position_y, ball.radius,
        circle_segment_count)
end

local platform = {
    position_x = 500,
    position_y = 500,
    speed_x = 300,
    width = 70,
    height = 20,
}

function platform.update(dt)
    if love.keyboard.isDown("left") then
        platform.position_x = platform.position_x - platform.speed_x * dt
    elseif love.keyboard.isDown("right") then
        platform.position_x = platform.position_x + platform.speed_x * dt
    end
end

function platform.draw()
    love.graphics.rectangle("line", platform.position_x, platform.position_y,
        platform.width, platform.height)
end

local bricks = {
    current_level_bricks = {},
    brick_width = 50,
    brick_height = 30,
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

function love.load()
    bricks.add_to_current_level_bricks(bricks.new_brick(100, 100))
    bricks.add_to_current_level_bricks(bricks.new_brick(160, 100))
    bricks.add_to_current_level_bricks(bricks.new_brick(220, 100))
    bricks.add_to_current_level_bricks(bricks.new_brick(280, 140))
    bricks.add_to_current_level_bricks(bricks.new_brick(340, 140))
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
end

function love.draw()
    ball.draw()
    platform.draw()
    bricks.draw()
end

function love.quit(mode)
    if mode == "restart" then
        print("Try again!")
    else
        print("See ya!")
    end
end
