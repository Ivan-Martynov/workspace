local rectangles

function love.load()
    rectangles = {}
end

function createRect()
    local rect = {}
    rect.x = 100
    rect.y = 100
    rect.width = 70
    rect.height = 90
    rect.speed = 100

    table.insert(rectangles, rect)
end

function love.keyreleased(key)
    if key == "space" then
        createRect()
    end

    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    for _, rect in pairs(rectangles) do
        rect.x = rect.x + rect.speed * dt
    end
end

function love.draw()
    for _, rect in pairs(rectangles) do
        love.graphics.rectangle("line", rect.x, rect.y, rect.width, rect.height)
    end
end
