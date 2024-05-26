local fontSize = 36
local fruits

function love.load()
    fruits = {"apple", "banana"}
    table.insert(fruits, "pear")
    table.insert(fruits, "pineapple")
    table.remove(fruits, 2)
    fruits[1] = "orange"

    local font = love.graphics.newFont(fontSize)
    love.graphics.setFont(font)
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.draw()
    for i, fruit in pairs(fruits) do
        love.graphics.print(fruit, 100, 100 + fontSize * i)
    end
end
