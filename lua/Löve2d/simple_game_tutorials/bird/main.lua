
local playing_area_width
local playing_area_height

local bird_x
local bird_y
local bird_width
local bird_height
local bird_speed_y

local pipe_width
local pipe_space_height
local pipe1_space_y
local pipe1_x
local pipe2_space_y
local pipe2_x
local upcoming_pipe

local reset
local new_pipe_space_y

local score

function love.load()
    playing_area_width, playing_area_height = love.window.getMode()

    bird_x = 62
    bird_width = 30
    bird_height = 25

    pipe_width = 54
    pipe_space_height = 100
    new_pipe_space_y = function()
        local pipe_space_y_min = 54
        return love.math.random(pipe_space_y_min,
            playing_area_height - pipe_space_height)
    end

    reset = function()
        bird_y = 200
        bird_speed_y = 0

        pipe1_x = playing_area_width
        pipe1_space_y = new_pipe_space_y()

        pipe2_x = playing_area_width + (playing_area_width + pipe_width) / 2
        pipe2_space_y = new_pipe_space_y()

        upcoming_pipe = 1

        score = 0
    end

    reset()
end

function love.keypressed(key)
    if key == "space" and bird_y > 0 then
        bird_speed_y = -165 elseif key == "escape" then love.event.quit()
    end
end

function love.update(dt)
    bird_speed_y = bird_speed_y + 516 * dt
    bird_y = bird_y + bird_speed_y * dt

    local function move_pipe(pipe_x, pipe_space_y)
        pipe_x = pipe_x - 60 * dt

        if (pipe_x + pipe_width < 0) then
            pipe_x = playing_area_width
            pipe_space_y = new_pipe_space_y()
        end

        return pipe_x, pipe_space_y
    end

    pipe1_x, pipe1_space_y = move_pipe(pipe1_x, pipe1_space_y)
    pipe2_x, pipe2_space_y = move_pipe(pipe2_x, pipe2_space_y)

    local function is_bird_colliding_with_pipe(pipe_x, pipe_space_y)
        return (bird_x < pipe_x + pipe_width)
            and (bird_x + bird_width > pipe_x) and ((bird_y < pipe_space_y) or
                (bird_y + bird_height > pipe_space_height + pipe_space_y))
    end

    if is_bird_colliding_with_pipe(pipe1_x, pipe1_space_y) or
        is_bird_colliding_with_pipe(pipe2_x, pipe2_space_y) or
        bird_y > playing_area_height then
        reset()
    end

    local function update_score(this_pipe, pipe_x, other_pipe)
        if (upcoming_pipe == this_pipe) and
            (bird_x > pipe_x + pipe_width) then
            score = score + 1
            upcoming_pipe = other_pipe
        end
    end

    update_score(1, pipe1_x, 2)
    update_score(2, pipe2_x, 1)
end

function love.draw()
    love.graphics.setColor(0.14, 0.36, 0.46)
    love.graphics.rectangle("fill", 0, 0,
        playing_area_width, playing_area_height)

    love.graphics.setColor(0.87, 0.84, 0.27)
    love.graphics.rectangle("fill", bird_x, bird_y, bird_width, bird_height)

    love.graphics.setColor(0.37, 0.82, 0.28)

    local function draw_pipe(x, y)
        love.graphics.rectangle("fill", x, 0, pipe_width, y)
        love.graphics.rectangle("fill", x, y + pipe_space_height, pipe_width,
            playing_area_height - pipe_space_height - y)
    end

    draw_pipe(pipe1_x, pipe1_space_y)
    draw_pipe(pipe2_x, pipe2_space_y)

    love.graphics.setColor(1, 1, 1)
    love.graphics.print(score, 15, 15)
end