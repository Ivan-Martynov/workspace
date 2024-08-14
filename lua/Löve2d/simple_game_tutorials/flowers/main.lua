local images
local cell_size
local grid_x_count
local grid_y_count
local selected_x
local selected_y
local grid
local game_over
local first_click

local function count_flowers(x, y)
    local flower_count = 0
    for dy = -1, 1 do
        for dx = -1, 1 do
            if ((dy ~= 0) or (dx ~= 0))
                and grid[y + dy] and grid[y + dy][x + dx]
                and grid[y + dy][x + dx].flower then
                flower_count = flower_count + 1
            end
        end
    end

    return flower_count
end

local function reset()
    game_over = false
    first_click = true

    grid = {}
    for y = 1, grid_y_count do
        grid[y] = {}
        for x = 1, grid_x_count do
            grid[y][x] = {
                flower = false,
                state = "covered",
            }
        end
    end
end

function love.load()
    cell_size = 18
    grid_x_count = 19
    grid_y_count = 14

    love.window.setTitle("Flowers")
    love.window.setMode(grid_x_count * cell_size, grid_y_count * cell_size)

    images = {}
    for _, image in ipairs({
        1, 2, 3, 4, 5, 6, 7, 8,
        "uncovered", "covered_highlighted", "covered",
        "flower", "flag", "question"
    }) do
        images[image] = love.graphics.newImage("images/" .. image .. ".png")
    end

    reset()
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    else
        love.load()
    end
end

function love.mousereleased(_, _, button)
    if game_over then
        reset()
        return
    end

    if (button == 1) and (grid[selected_y][selected_x].state ~= "flag") then
        if first_click then
            first_click = false

            local flower_positions = {}
            for y = 1, grid_y_count do
                for x = 1, grid_x_count do
                    if (x ~= selected_x) or (y ~= selected_y) then
                        table.insert(flower_positions, { x = x, y = y, })
                    end
                end
            end

            for _ = 1, 40 do
                local pos = table.remove(flower_positions,
                    love.math.random(#flower_positions))
                grid[pos.y][pos.x].flower = true
            end
        end

        if grid[selected_y][selected_x].flower then
            grid[selected_y][selected_x].state = "uncovered"
            game_over = true
            return
        end

        local stack = {
            {
                x = selected_x,
                y = selected_y,
            }
        }

        while #stack > 0 do
            local current = table.remove(stack)
            local x = current.x
            local y = current.y
            grid[y][x].state = "uncovered"

            if count_flowers(x, y) > 0 then
                goto continue
            end
            for dy = -1, 1 do
                for dx = -1, 1 do
                    if ((dy ~= 0) or (dx ~= 0))
                        and grid[y + dy] and grid[y + dy][x + dx] then
                        local state = grid[y + dy][x + dx].state
                        if (state == "covered") or (state == "question") then
                            table.insert(stack, { x = x + dx, y = y + dy, })
                        end
                    end
                end
            end
            ::continue::
        end

        local complete = true
        for y = 1, grid_y_count do
            for x = 1, grid_x_count do
                if (grid[y][x].state ~= "uncovered")
                    and (not grid[y][x].flower) then
                    complete = false
                end
            end
        end

        if complete then
            game_over = true
        end
    elseif button == 2 then
        if grid[selected_y][selected_x].state == "covered" then
            grid[selected_y][selected_x].state = "flag"
        elseif grid[selected_y][selected_x].state == "flag" then
            grid[selected_y][selected_x].state = "question"
        elseif grid[selected_y][selected_x].state == "question" then
            grid[selected_y][selected_x].state = "covered"
        end
    end
end

function love.update(_)
    selected_x = math.floor(love.mouse.getX() / cell_size) + 1
    if selected_x > grid_x_count then
        selected_x = grid_x_count
    end

    selected_y = math.floor(love.mouse.getY() / cell_size) + 1
    if selected_y > grid_y_count then
        selected_y = grid_y_count
    end
end

function love.draw()
    for y = 1, grid_y_count do
        for x = 1, grid_x_count do
            local function draw_cell(image)
                love.graphics.draw(image,
                    (x - 1) * cell_size, (y - 1) * cell_size)
            end

            if grid[y][x].state == "uncovered" then
                draw_cell(images.uncovered)
            elseif (x == selected_x) and (y == selected_y)
                and (not game_over) then
                if love.mouse.isDown(1) then
                    draw_cell((grid[y][x].state == "flag")
                        and images.covered or images.uncovered)
                else
                    draw_cell(images.covered_highlighted)
                end
            else
                draw_cell(images.covered)
            end

            if grid[y][x].flower then
                if game_over then
                    draw_cell(images.flower)
                end
            else
                local flower_count = count_flowers(x, y)
                if (flower_count > 0) and (grid[y][x].state == "uncovered") then
                    draw_cell(images[flower_count])
                end
            end

            if grid[y][x].state == "flag" then
                draw_cell(images.flag)
            elseif grid[y][x].state == "question" then
                draw_cell(images.question)
            end
        end
    end
end
