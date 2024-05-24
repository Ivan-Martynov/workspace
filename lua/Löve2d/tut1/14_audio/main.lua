
local tilemap
local colors
local color_tilemap
local tile_image
local tile_atlas
local quads
local atlas_map

local quad_width
local quad_height

local player

local song
local sfx

local function load_simple_tilemap()
    tilemap = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 1, 1, 1, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    }
end

local function load_color_tilemap()
    colors = {
        { 1, 1, 1 },
        { 1, 0, 0 },
        { 1, 0, 1 },
        { 0, 0, 1 },
        { 0, 1, 1 },
    }

    color_tilemap = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 3, 4, 5, 5, 4, 3, 2, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    }
end

local function load_tiles()
    tile_image = { sprite = love.graphics.newImage("tile.png") }
    tile_image.width = tile_image.sprite:getWidth()
    tile_image.height = tile_image.sprite:getHeight()
end

local function load_atlas()
    tile_atlas = { sprite = love.graphics.newImage("tileset.png") }
    tile_atlas.width = tile_atlas.sprite:getWidth()
    tile_atlas.height = tile_atlas.sprite:getHeight()

    quads = {}
    quad_width = tile_atlas.width / 3 - 2
    quad_height = tile_atlas.height / 2 - 2
    for i = 0, 1 do
        for j = 0, 2 do
            table.insert(quads, love.graphics.newQuad(
                1 + j * (quad_width + 2),
                1 + i * (quad_height + 2),
                quad_width, quad_height, tile_atlas.width, tile_atlas.height))
        end
    end

    atlas_map = {
        { 1, 6, 6, 2, 1, 6, 6, 2 },
        { 3, 0, 0, 4, 5, 0, 0, 3 },
        { 3, 0, 0, 0, 0, 0, 0, 3 },
        { 4, 2, 0, 0, 0, 0, 1, 5 },
        { 1, 5, 0, 0, 0, 0, 4, 2 },
        { 3, 0, 0, 0, 0, 0, 0, 3 },
        { 3, 0, 0, 1, 2, 0, 0, 3 },
        { 4, 6, 6, 5, 4, 6, 6, 5 },
    }
end

local function load_player()
    player = {
        sprite = love.graphics.newImage("player.png"),
        tile_x = 2,
        tile_y = 2
    }
end

local function load_audio()
    song = love.audio.newSource("song.ogg", "stream")
    song:setLooping(true)
    song:setVolume(0.07)
    --song:play()

    sfx = love.audio.newSource("sfx.ogg", "static")
end

function love.load()
    load_simple_tilemap()

    load_color_tilemap()

    load_tiles()
    load_atlas()

    load_player()

    load_audio()
end

function love.keypressed(key)
    local x = player.tile_x
    local y = player.tile_y

    if key == "left" then
        x = x - 1
    elseif key == "right" then
        x = x + 1
    elseif key == "up" then
        y = y - 1
    elseif key == "down" then
        y = y + 1
    end

    if ((player.tile_x ~= x) or (player.tile_y ~= y)) and
        ((not atlas_map[y][x]) or (atlas_map[y][x] == 0)) then
        sfx:play()
        player.tile_x = x
        player.tile_y = y
    end

    if key == "space" then
        sfx:play()
    end

    if key == "escape" then
        love.event.quit()
    end
end

local function draw_tilemap(tiles, color_map, shift_x, shift_y, image, isAtlas)
    if not shift_x then
        shift_x = 0
    end

    if not shift_y then
        shift_y = 0
    end

    local tile_width = 20
    local tile_height = 20

    local mode = "line"
    if color_map then
        mode = "fill"
    end

    for i, row in ipairs(tiles) do
        for j, tile in ipairs(row) do
            if tile ~= 0 then
                if color_map then
                    love.graphics.setColor(color_map[tile])
                end
                if image then
                    if isAtlas then
                        love.graphics.draw(image.sprite, quads[tile],
                            shift_x + j * quad_width,
                            shift_y + i * quad_height)
                    else
                        love.graphics.draw(image.sprite,
                            shift_x + (j - 1) * image.width,
                            shift_y + (i - 1) * image.height)
                    end
                else
                    love.graphics.rectangle(mode,
                        shift_x + j * tile_width, shift_y + i * tile_height,
                        tile_width, tile_height)
                end
            end
        end
    end

    love.graphics.setColor(1, 1, 1, 1)
end

function love.draw()
    draw_tilemap(tilemap)
    draw_tilemap(color_tilemap, colors, 25 * 10, 0)
    draw_tilemap(color_tilemap, colors, 0, 25 * 5, tile_image)

    local x = 25 * 12
    local y = 25 * 5
    draw_tilemap(atlas_map, nil, x, y, tile_atlas, true)

    love.graphics.draw(player.sprite,
        x + player.tile_x * quad_width, y + player.tile_y * quad_height)
end
