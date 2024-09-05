local vector = require "vector"

-- Platform object.
local platform = {
    position = vector(500, 500),
    speed = vector(300, 0),
    width = 70,
    height = 20,
    moving_to_the_left = false,
    moving_to_the_right = false,
}

function platform.rebound(shift)
    local min_shift = math.min(math.abs(shift.x), math.abs(shift.y))

    if math.abs(shift.x) == min_shift then
        shift.y = 0
    else
        shift.x = 0
    end

    platform.position.x = platform.position.x + shift.x
    platform.position.y = platform.position.y + shift.y
end

-- Update the platform.
function platform.update(dt)
    if love.keyboard.isDown("left") then
        platform.moving_to_the_left = true
        platform.moving_to_the_right = false

        platform.position = platform.position - platform.speed * dt
    elseif love.keyboard.isDown("right") then
        platform.moving_to_the_left = false
        platform.moving_to_the_right = true

        platform.position = platform.position + platform.speed * dt
    else
        platform.moving_to_the_left = false
        platform.moving_to_the_right = false
    end
end

-- Draw the platform.
function platform.draw()
    love.graphics.rectangle("line", platform.position.x, platform.position.y,
        platform.width, platform.height)
end

return platform
