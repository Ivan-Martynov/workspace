
local selected_x
local selected_y

local cell_size

local grid_x_count
local grid_y_count

local grid

function love.load()
    cell_size = 5

    local window_width = love.graphics.getWidth()
    local window_height = love.graphics.getHeight()

    grid_x_count = window_width / cell_size
    grid_y_count = window_height / cell_size

    grid = {}
    for y = 1, grid_y_count do
        grid[y] = {}
        for x = 1, grid_x_count do
            grid[y][x] = false
        end
    end

    -- Temporary.
    grid[1][1] = true
    grid[1][2] = true

    love.graphics.setBackgroundColor(1, 1, 1)

    love.keyboard.setKeyRepeat(true)
end

function love.keypressed(key)
    local next_grid = {}
    for y = 1, grid_y_count do
        next_grid[y] = {}
        for x = 1, grid_x_count do
            local neighbor_count = 0

            for dy = -1, 1 do
                for dx = -1, 1 do
                    if (dy ~= 0 or dx ~= 0) and grid[y + dy]
                        and grid[y + dy][x + dx] then
                        neighbor_count = neighbor_count + 1
                    end
                end
            end

            next_grid[y][x] = (neighbor_count == 3)
                or (grid[y][x] and (neighbor_count == 2))
        end
    end

    grid = next_grid

    if key == "escape" then
        love.event.quit()
    end
end

function love.update()
    selected_x = math.min(math.floor(love.mouse.getX() / cell_size) + 1,
        grid_x_count)
    selected_y = math.min(math.floor(love.mouse.getY() / cell_size) + 1,
        grid_y_count)

    if love.mouse.isDown(1) then
        grid[selected_y][selected_x] = true
    elseif love.mouse.isDown(2) then
        grid[selected_y][selected_x] = false
    end
end

function love.draw()
    local cell_draw_size = cell_size - 1

    for y = 1, grid_y_count do
        for x = 1, grid_x_count do

            if (x == selected_x) and (y == selected_y) then
                love.graphics.setColor(0, 1, 1)
            elseif grid[y][x] then
                love.graphics.setColor(1, 0, 1)
            else
                love.graphics.setColor(0.86, 0.86, 0.86)
            end

            love.graphics.rectangle("fill", (x - 1) * cell_size,
                (y - 1) * cell_size, cell_draw_size, cell_draw_size)
        end
    end

    love.graphics.setColor(0, 0, 0)
    love.graphics.print("Selected x = " .. selected_x ..
        ", selected y = " .. selected_y)
end