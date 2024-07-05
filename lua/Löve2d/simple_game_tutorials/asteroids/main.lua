local arena_width, arena_height

local ship_x, ship_y
local ship_angle
local ship_radius
local ship_speed_x, ship_speed_y

local bullets
local bullet_timer_limit
local bullet_timer
local bullet_radius

local asteroids
local asteroid_stages

local function reset()
    ship_x = arena_width / 2
    ship_y = arena_height / 2
    ship_angle = 0
    ship_speed_x = 0
    ship_speed_y = 0

    bullets = {}
    bullet_timer = bullet_timer_limit

    asteroids = {
        {
            x = 100,
            y = 100,
        },
        {
            x = arena_width - 100,
            y = 100,
        },
        {
            x = arena_width / 2,
            y = arena_height - 100,
        },
    }

    for _, asteroid in ipairs(asteroids) do
        asteroid.angle = love.math.random() * math.pi * 2
        asteroid.stage = #asteroid_stages
    end
end

function love.load()
    love.window.setTitle("Asteroids")

    arena_width, arena_height = love.window.getMode()

    ship_radius = 30

    bullet_timer_limit = 0.5
    bullet_radius = 5

    asteroid_stages = {
        {
            speed = 120,
            radius = 15,
        },
        {
            speed = 70,
            radius = 30,
        },
        {
            speed = 50,
            radius = 50,
        },
        {
            speed = 20,
            radius = 80,
        },
    }

    reset()
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    local turn_speed = 10
    if love.keyboard.isDown("left") then
        ship_angle = ship_angle - turn_speed * dt
    elseif love.keyboard.isDown("right") then
        ship_angle = ship_angle + turn_speed * dt
    end
    ship_angle = ship_angle % (math.pi * 2)

    if love.keyboard.isDown("up") then
        local sheep_speed = 100
        ship_speed_x = ship_speed_x + math.cos(ship_angle) * sheep_speed * dt
        ship_speed_y = ship_speed_y + math.sin(ship_angle) * sheep_speed * dt
    end

    ship_x = (ship_x + ship_speed_x * dt) % arena_width
    ship_y = (ship_y + ship_speed_y * dt) % arena_height

    local function circles_collide(a_x, a_y, a_radius, b_x, b_y, b_radius)
        return (a_x - b_x) ^ 2 + (a_y - b_y) ^ 2 <= (a_radius + b_radius) ^ 2
    end

    for i = #bullets, 1, -1 do
        local bullet = bullets[i]
        bullet.live_time = bullet.live_time - dt

        if bullet.live_time <= 0 then
            table.remove(bullets, i)
        else
            local bullet_speed = 500
            bullet.x = (bullet.x + math.cos(bullet.angle) * bullet_speed * dt)
                % arena_width
            bullet.y = (bullet.y + math.sin(bullet.angle) * bullet_speed * dt)
                % arena_height
        end

        for j = #asteroids, 1, -1 do
            local asteroid = asteroids[j]
            if circles_collide(bullet.x, bullet.y, bullet_radius, asteroid.x,
                    asteroid.y, asteroid_stages[asteroid.stage].radius) then
                table.remove(bullets, i)

                if asteroid.stage > 1 then
                    local first_angle = love.math.random() * math.pi * 2
                    local second_angle = (first_angle - math.pi) % (math.pi * 2)
                    table.insert(asteroids, {
                        x = asteroid.x,
                        y = asteroid.y,
                        angle = first_angle,
                        stage = asteroid.stage - 1,
                    })
                    table.insert(asteroids, {
                        x = asteroid.x,
                        y = asteroid.y,
                        angle = second_angle,
                        stage = asteroid.stage - 1,
                    })
                end

                table.remove(asteroids, j)
                break
            end
        end
    end

    bullet_timer = bullet_timer + dt
    if love.keyboard.isDown("s") and (bullet_timer >= bullet_timer_limit) then
        bullet_timer = 0
        table.insert(bullets, {
            x = ship_x + math.cos(ship_angle) * ship_radius,
            y = ship_y + math.sin(ship_angle) * ship_radius,
            angle = ship_angle,
            live_time = 4,
        })
    end

    if #asteroids == 0 then
        reset()
        return
    end

    for _, asteroid in ipairs(asteroids) do
        local asteroid_speed = asteroid_stages[asteroid.stage].speed * dt
        asteroid.x = (asteroid.x + math.cos(asteroid.angle) * asteroid_speed)
            % arena_width
        asteroid.y = (asteroid.y + math.sin(asteroid.angle) * asteroid_speed)
            % arena_height

        if circles_collide(ship_x, ship_y, ship_radius, asteroid.x, asteroid.y,
                asteroid_stages[asteroid.stage].radius) then
            reset()
            break
        end
    end
end

function love.draw()
    for y = -1, 1 do
        for x = -1, 1 do
            love.graphics.origin()
            love.graphics.translate(x * arena_width, y * arena_height)
            love.graphics.setColor(0, 0, 1)
            love.graphics.circle("fill", ship_x, ship_y, ship_radius)

            local ship_circle_distance = 20
            love.graphics.setColor(0, 1, 1)
            love.graphics.circle("fill",
                ship_x + math.cos(ship_angle) * ship_circle_distance,
                ship_y + math.sin(ship_angle) * ship_circle_distance,
                bullet_radius)

            love.graphics.setColor(0, 1, 0)
            for _, bullet in ipairs(bullets) do
                love.graphics.circle("fill", bullet.x, bullet.y, 5)
            end

            love.graphics.setColor(1, 1, 0)
            for _, asteroid in ipairs(asteroids) do
                love.graphics.circle("fill", asteroid.x, asteroid.y,
                    asteroid_stages[asteroid.stage].radius)
            end
        end
    end

    --love.graphics.origin()
    --love.graphics.setColor(1, 1, 1)
    --love.graphics.print(
    --    table.concat({
    --        "Ship angle: " .. ship_angle * 180 / math.pi,
    --        "X: " .. ship_x, "Y: " .. ship_y,
    --        "dx: " .. ship_speed_x, "dy:" .. ship_speed_y }, "\n"))
end