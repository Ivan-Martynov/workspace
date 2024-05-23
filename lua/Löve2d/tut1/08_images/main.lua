

function love.load()
    image = love.graphics.newImage("sheep.png")
    love.graphics.setBackgroundColor({ 7 / 255, 54 / 255, 66 / 255, 1 })
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.draw()
    love.graphics.draw(image, 0, 0, 0, 1, 1,
        image:getWidth() / 2, image:getHeight() / 2)

    love.graphics.setColor(1, 0.78, 0.15, 0.5)
    love.graphics.draw(image, 200, 90, 0, 2, 2, 39, 57)
    love.graphics.setColor(1, 1, 1, 1)

    love.graphics.setColor(1, 1, 1, 0.5)
    love.graphics.draw(image, 100, 100, 0, -1, 1)
    love.graphics.setColor(1, 1, 1, 1)

    love.graphics.draw(image, 400, 100, 0, 1, 1, 0, 0, 0.5, 0)
    love.graphics.draw(image, 400, 200, 0, 1, 1, 0, 0, 0, 0.5)
end
