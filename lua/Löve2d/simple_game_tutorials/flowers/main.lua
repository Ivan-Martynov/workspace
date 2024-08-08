local images
local cell_size
local grid_x_count
local grid_y_count
local selected_x
local selected_y

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

end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
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
            local image
            if (x == selected_x) and (y == selected_y) then
                image = love.mouse.isDown(1) and images.uncovered
                    or images.covered_highlighted
            else
                image = images.covered
            end
            love.graphics.draw(image, (x - 1) * cell_size, (y - 1) * cell_size)
        end
    end

    love.graphics.setColor(0, 0, 0)
    love.graphics.print("Selected x = " .. selected_x .. ", y = " .. selected_y)
    love.graphics.setColor(1, 1, 1)
end
