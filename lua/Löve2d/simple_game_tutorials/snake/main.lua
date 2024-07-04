local timer
local direction_queue

local snake_segments
local snake_alive

local grid_x_count
local grid_y_count
local cell_size

local food_position

local function move_food()
    local empty_cells = {}

    for x = 1, grid_x_count do
        for y = 1, grid_y_count do
            local is_empty = true
            for _, segment in ipairs(snake_segments) do
                if (x == segment.x) and (y == segment.y) then
                    is_empty = false
                end
            end
            if is_empty then
                table.insert(empty_cells, { x = x, y = y })
            end
        end
    end

    food_position = empty_cells[love.math.random(#empty_cells)]
end

local function reset()
    timer = 0
    snake_segments = {
        { x = 3, y = 1, },
        { x = 2, y = 1, },
        { x = 1, y = 1, },
    }
    snake_alive = true

    move_food()

    direction_queue = { "right" }
end

function love.load()
    cell_size = 15
    grid_x_count = 20
    grid_y_count = 15

    local window_width = 800
    local window_height = 200
    local cell_size_x = window_width / grid_x_count
    local cell_size_y = window_height / grid_x_count
    cell_size = (cell_size_x + cell_size_y) / 2
    grid_x_count = window_width / cell_size
    grid_y_count = window_height / cell_size

    love.window.setTitle("Snake")
    love.window.setMode(window_width, window_height)

    reset()
end

local function try_adding(target, opposite)
    if (direction_queue[#direction_queue] ~= target) and
        (direction_queue[#direction_queue] ~= opposite) then
        table.insert(direction_queue, target)
    end
end

function love.keypressed(key)
    if (key == "right") then
        try_adding("right", "left")
    elseif (key == "left") then
        try_adding("left", "right")
    elseif (key == "down") then
        try_adding("down", "up")
    elseif (key == "up") then
        try_adding("up", "down")
    elseif key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    timer = timer + dt
    if not snake_alive then
        if timer >= 2 then
            reset()
        end
        return
    end

    if timer >= 0.15 then
        timer = 0
        local next_x = snake_segments[1].x
        local next_y = snake_segments[1].y

        if #direction_queue > 1 then
            table.remove(direction_queue, 1)
        end

        if direction_queue[1] == "right" then
            next_x = next_x + 1
            if next_x > grid_x_count then
                next_x = 1
            end
        elseif direction_queue[1] == "left" then
            next_x = next_x - 1
            if next_x < 1 then
                next_x = grid_x_count
            end
        elseif direction_queue[1] == "down" then
            next_y = next_y + 1
            if next_y > grid_y_count then
                next_y = 1
            end
        elseif direction_queue[1] == "up" then
            next_y = next_y - 1
            if next_y < 1 then
                next_y = grid_y_count
            end
        end

        for i, segment in ipairs(snake_segments) do
            if (i ~= #snake_segments) and (next_x == segment.x)
            and (next_y == segment.y) then
                snake_alive = false
            end
        end

        if snake_alive  then
            table.insert(snake_segments, 1, { x = next_x, y = next_y })
            if (snake_segments[1].x == food_position.x)
                and (snake_segments[1].y == food_position.y) then
                move_food()
            else
                table.remove(snake_segments)
            end
        end
    end
end

function love.draw()
    local function draw_cell(x, y)
        love.graphics.rectangle("fill", (x - 1) * cell_size,
            (y - 1) * cell_size, cell_size - 1, cell_size - 1)
    end
    love.graphics.setColor(0.28, 0.28, 0.28)
    love.graphics.rectangle("fill", 0, 0, grid_x_count * cell_size,
        grid_y_count * cell_size)

    if snake_alive then
        love.graphics.setColor(0.6, 1, 0.32)
    else
        love.graphics.setColor(0.5, 0.5, 0.5)
    end
    for _, segment in ipairs(snake_segments) do
        draw_cell(segment.x, segment.y)
    end

    love.graphics.setColor(1, 0.3, 0.3)
    draw_cell(food_position.x, food_position.y)
end