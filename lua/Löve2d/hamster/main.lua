-- Tutorial 1: Hamster ball.
-- Add an mage to the game and move it around using arrow keys.

function love.load()
    hamster = love.graphics.newImage("hamster.png")
    x = 50
    y = 50
    speed = 300
end

function love.update(dt)
    local shift = speed * dt
    if love.keyboard.isDown("right") then
        x = x + shift
    end

    if love.keyboard.isDown("left") then
        x = x - shift
    end

    if love.keyboard.isDown("down") then
        y = y + shift
    end

    if love.keyboard.isDown("up") then
        y = y - shift
    end
end

function love.draw()
    love.graphics.draw(hamster, x, y)
end
