local Player = require "player"
local Wall = require "wall"
local Box = require "box"

local player
local wall
local boxes
local objects

function love.load()
    player = Player(100, 100)
    wall = Wall(200, 100)
    boxes = {}
    table.insert(boxes, Box(400, 150))
    table.insert(boxes, Box(400, 350))
    table.insert(boxes, Box(500, 120))

    objects = {}
    table.insert(objects, player)
    table.insert(objects, wall)
    for _, box in ipairs(boxes) do
        table.insert(objects, box)
    end
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    for _, v in ipairs(objects) do
        v:update(dt)
    end

    local looping = true
    local limit = 100
    local count = 0

    while looping do
        looping = false
        count = count + 1

        if count > limit then
            break
        end

        for i = 1, #objects - 1 do
            for j = i + 1, #objects do
                if objects[i]:resolve_collision(objects[j]) then
                    looping = true
                end
            end
        end
    end
end

function love.draw()
    for _, v in ipairs(objects) do
        v:draw()
    end
end
