io.stdout:setvbuf("no")

local player
local coins
local lume
local data_file_path

function love.load()
    lume = require "lume"

    data_file_path = "savedata.txt"

    player = { x = 100, y = 100, dx = 305, dy = 295, size = 25 }
    player.sprite = love.graphics.newImage("face.png")
    player.origin = {}
    player.origin.x = player.sprite:getWidth() / 2
    player.origin.y = player.sprite:getHeight() / 2

    coins = {}
    local coin_sprite = love.graphics.newImage("dollar.png")

    if love.filesystem.getInfo(data_file_path) then
        local file = love.filesystem.read(data_file_path)
        local data = lume.deserialize(file)
        player.x = data.player.x
        player.y = data.player.y
        player.size = data.player.size

        for i, coin in ipairs(data.coins) do
            coins[i] = {
                x = coin.x,
                y = coin.y,
                size = 10,
                sprite = coin_sprite,
                origin = {
                    x = coin_sprite:getWidth() / 2,
                    y = coin_sprite:getHeight() / 2
                }
            }
        end
    else
        local window_width = love.graphics.getWidth()
        local window_height = love.graphics.getHeight()
        local margin_x = 50
        local margin_y = 40

        for _ = 1, 25 do
            table.insert(coins,
                {
                    x = love.math.random(margin_x, window_width - margin_x),
                    y = love.math.random(margin_y, window_height - margin_y),
                    size = 10,
                    sprite = coin_sprite,
                    origin = {
                        x = coin_sprite:getWidth() / 2,
                        y = coin_sprite:getHeight() / 2
                    }
                })
        end
    end
end

local function save_game()
    local data = {}
    data.player = { x = player.x, y = player.y, size = player.size }
    data.coins = {}
    for i, coin in ipairs(coins) do
        data.coins[i] = { x = coin.x, y = coin.y }
    end

    local serialized = lume.serialize(data)
    love.filesystem.write(data_file_path, serialized)
end

function love.keypressed(key)
    if key == "f1" then
        save_game()
    elseif key == "f2" then
        love.filesystem.remove(data_file_path)
        love.event.quit("restart")
    elseif key == "escape" then
        love.event.quit()
    end
end

local function is_circle_collision(first, second)
    return math.sqrt((second.x - first.x) ^ 2 + (second.y - first.y) ^ 2)
        < first.size + second.size
end

function love.update(dt)
    if love.keyboard.isDown("left") then
        player.x = player.x - player.dx * dt
        if player.x < player.size then
            player.x = player.size
        end
    elseif love.keyboard.isDown("right") then
        player.x = player.x + player.dx * dt
        local window_width = love.graphics.getWidth()
        if player.x > window_width - player.size then
            player.x = window_width - player.size
        end
    end

    if love.keyboard.isDown("up") then
        player.y = player.y - player.dy * dt
        if player.y < player.size then
            player.y = player.size
        end
    elseif love.keyboard.isDown("down") then
        player.y = player.y + player.dy * dt
        local window_height = love.graphics.getHeight()
        if player.y > window_height - player.size then
            player.y = window_height - player.size
        end
    end

    for i = #coins, 1, -1 do
        if is_circle_collision(player, coins[i]) then
            table.remove(coins, i)
            player.size = player.size + 1
        end
    end
end

function love.draw()
    love.graphics.circle("line", player.x, player.y, player.size)
    love.graphics.draw(player.sprite, player.x, player.y, 0, 1, 1,
        player.origin.x, player.origin.y)

    for _, coin in ipairs(coins) do
        love.graphics.circle("line", coin.x, coin.y, coin.size)
        love.graphics.draw(coin.sprite, coin.x, coin.y, 0, 1, 1,
            coin.origin.x, coin.origin.y)
    end
end
