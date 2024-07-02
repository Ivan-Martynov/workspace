local sequence
local current
local timer
local state
local flashing

local function add_to_sequence()
    table.insert(sequence, love.math.random(4))
end

function love.load()
    love.window.setTitle("Repeat")
    love.window.setMode(200, 100)

    sequence = {}
    add_to_sequence()

    current = 1

    timer = 0

    state = "watch"

    flashing = false
end

function love.keypressed(key)
    if state == "repeat" then
        if tonumber(key) == sequence[current] then
            current = current + 1
            if current > #sequence then
                current = 1
                add_to_sequence()
                state = "watch"
            end
        else
            state = "gameover"
        end
    elseif state == "gameover" then
        love.load()
    end

    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    if state == "watch" then
        timer = timer + dt
        if timer >= 1 then
            timer = 0
            flashing = not flashing
            if not flashing then
                current = current + 1
                if current > #sequence then
                    state = "repeat"
                    current = 1
                end
            end
        end
    end
end

function love.draw()
    love.graphics.setNewFont(20)

    local function draw_square(number, color, flash_color)
        local square_size = 50

        local x = square_size * (number - 1)
        if (state == "watch") and flashing and
            (number == sequence[current]) then
            love.graphics.setColor(flash_color)
        else
            love.graphics.setColor(color)
        end
        love.graphics.rectangle("fill", x, 0, square_size, square_size)
        love.graphics.setColor(1, 1, 1)
        love.graphics.print(number, x + 19, 14)
        
    end

    draw_square(1, {0.2, 0, 0}, {1, 0, 0})
    draw_square(2, {0, 0.2, 0}, {0, 1, 0})
    draw_square(3, {0, 0, 0.2}, {0, 0, 1})
    draw_square(4, {0.2, 0.2, 0}, {1, 1, 0})

    if state == "repeat" then
        love.graphics.print(current .. "/" .. #sequence, 20, 100)
    elseif state == "gameover" then
        love.graphics.print("Game over!", 20, 60)
    end
end
