
local function atan2(y, x)
    if x == 0 then
        if y == 0 then
            return 0
        else
            return y > 0 and math.pi / 2 or -math.pi / 2
        end
    else
        local arctan = math.atan(y / x)
        if x > 0 then
            return arctan
        else
            return y < 0 and -math.pi + arctan or math.pi + arctan
        end
    end
end

function love.draw()
    local function draw_eye(index)
        local eye_radius = 53
        local pupil_radius = 15

        local window_center_x = love.graphics.getWidth() / 2
        local eye_x = (index == 1) and window_center_x - eye_radius * 1.5
            or window_center_x + eye_radius * 1.5
        local eye_y = (love.graphics.getHeight() - eye_radius) / 2
        local distance_x = love.mouse.getX() - eye_x
        local distance_y = love.mouse.getY() - eye_y

        local distance = math.min(math.sqrt(distance_x * distance_x
            + distance_y * distance_y), eye_radius - pupil_radius - 5)

        local angle = atan2(distance_y, distance_x)

        local pupil_x = eye_x + math.cos(angle) * distance
        local pupil_y = eye_y + math.sin(angle) * distance

        love.graphics.setColor(1, 1, 1)
        love.graphics.circle("fill", eye_x, eye_y, eye_radius)

        love.graphics.setColor(0, 0, 0.4)
        love.graphics.circle("fill", pupil_x, pupil_y, pupil_radius)
    end

    draw_eye(1)
    draw_eye(2)

    love.graphics.setColor(1, 1, 1)
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end
