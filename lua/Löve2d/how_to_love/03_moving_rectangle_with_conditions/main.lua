local x

function love.load()
    x = 100
end

function love.update(dt)
    if x < 600 then
        x = x + 100 * dt
    end

    if love.keyboard.isDown("right") then
        x = x + 100 * dt
    elseif love.keyboard.isDown("left") then
        x = x - 100 * dt
        print("Moving to the left")
    end
end

function love.draw()
    love.graphics.rectangle("line", x, 50, 200, 150)
end
