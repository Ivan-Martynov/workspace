local ball = {
    position_x = 300,
    position_y = 300,
    speed_x = 300,
    speed_y = 300,
    radius = 10,
}

local platform = {
    position_x = 500,
    position_y = 500,
    speed_x = 300,
    width = 70,
    height = 20,
}

local brick = {
    position_x = 100,
    position_y = 100,
    width = 50,
    height = 30,
}

function love.load()
end

function love.update(dt)
    ball.position_x = ball.position_x + ball.speed_x * dt
    ball.position_y = ball.position_y + ball.speed_y * dt

    if love.keyboard.isDown("left") then
        platform.position_x = platform.position_x - platform.speed_x * dt
    elseif love.keyboard.isDown("right") then
        platform.position_x = platform.position_x + platform.speed_x * dt
    end
end

function love.draw()
    local circle_segment_count = 16
    love.graphics.circle("line", ball.position_x, ball.position_y, ball.radius,
        circle_segment_count)

    love.graphics.rectangle("line", platform.position_x, platform.position_y,
        platform.width, platform.height)

    love.graphics.rectangle("line", brick.position_x, brick.position_y,
        brick.width, brick.height)
end

function love.quit(mode)
    if mode == "restart" then
        print("Try again!")
    else
        print("See ya!")
    end
end
