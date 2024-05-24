
local frames
local current_frame
local atlas
local quads

function love.load()
    love.graphics.setBackgroundColor(1, 1, 1, 1)
    --love.graphics.setBackgroundColor({ 7 / 255, 54 / 255, 66 / 255, 1 })

    frames = {}
    for i = 1, 5 do
        table.insert(frames,
            love.graphics.newImage("images/jump" .. i .. ".png"))
    end

    current_frame = 1

    quads = {}
    atlas = love.graphics.newImage("images/jump.png")
    local quad_width = 117
    local quad_height = 233
    for i = 0, 4 do
        table.insert(quads, love.graphics.newQuad(i * quad_width, 0,
            quad_width, quad_height, atlas:getDimensions()))
    end
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    current_frame = current_frame + 7 * dt
    if current_frame > 5 then
        current_frame = 1
    end
end

function love.draw()
    love.graphics.draw(frames[math.floor(current_frame)])
    love.graphics.draw(atlas, quads[math.floor(current_frame)], 100, 100)
end
