local cell_size
local grid_x_count
local grid_y_count
local grid

local function piece_value(x, y)
    return ((y - 1) * grid_x_count) + x
end

local function is_complete()
    for y = 1, grid_y_count do
        for x = 1, grid_x_count do
            if grid[y][x] ~= piece_value(x, y) then
                return false
            end
        end
    end

    return true
end

local function move(direction)
    local empty_x
    local empty_y
    for y = 1, grid_y_count do
        for x = 1, grid_x_count do
            if grid[y][x] == grid_x_count * grid_y_count then
                empty_x = x
                empty_y = y
                goto continue
            end
        end
    end
    ::continue::

    local next_x = empty_x
    local next_y = empty_y

    if direction == "right" then
        next_x = next_x - 1
    elseif direction == "left" then
        next_x = next_x + 1
    elseif direction == "down" then
        next_y = next_y - 1
    elseif direction == "up" then
        next_y = next_y + 1
    end

    if grid[next_y] and grid[next_y][next_x] then
        grid[next_y][next_x], grid[empty_y][empty_x] =
            grid[empty_y][empty_x], grid[next_y][next_x]
    end
end

local function reset()
    grid = {}

    for y = 1, grid_y_count do
        grid[y] = {}
        for x = 1, grid_x_count do
            grid[y][x] = piece_value(x, y)
        end
    end

    repeat
        for _ = 1, 1000 do
            local roll = love.math.random(4)
            if roll == 1 then
                move("down")
            elseif roll == 2 then
                move("up")
            elseif roll == 3 then
                move("right")
            else
                move("left")
            end
        end

        for _ = 1, grid_x_count - 1 do
            move("left")
        end

        for _ = 1, grid_y_count - 1 do
            move("up")
        end
    until not is_complete()
end

function love.load()
    love.window.setTitle("Fifteen")
    love.graphics.setNewFont(30)

    cell_size = 100
    grid_x_count = 4
    grid_y_count = 4

    love.window.setMode(grid_x_count * cell_size, grid_y_count * cell_size)

    reset()
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    elseif (key == "down") or (key == "up")
        or (key == "left") or (key == "right") then
        move(key)

        if is_complete() then
            reset()
        end
    end
end

function love.draw()
    for y = 1, grid_y_count do
        for x = 1, grid_x_count do
            if grid[y][x] ~= grid_x_count * grid_y_count then
                local draw_size = cell_size - 1
                love.graphics.setColor(0.4, 0.1, 0.6)
                love.graphics.rectangle("fill",
                    (x - 1) * cell_size, (y - 1) * cell_size,
                    draw_size, draw_size)
                love.graphics.setColor(1, 1, 1)
                love.graphics.print(grid[y][x],
                    (x - 1) * cell_size, (y - 1) * cell_size)
            end
        end
    end
end
