io.stdout:setvbuf("no")

local player1
local player2

local coins

local lume

local data_file_path

local shake_time
local shake_wait
local shake_offset

local screen_canvas

local function make_player(x, y, sprite)
    local player = {
        x = x, y = y, dx = 290, dy = 290, size = 25,
    }
    player.sprite = sprite
    player.origin = {}
    player.origin.x = player.sprite:getWidth() / 2
    player.origin.y = player.sprite:getHeight() / 2
    player.score = 0

    return player
end

local function make_coin(x, y, sprite)
    return {
        x = x,
        y = y,
        size = 10,
        sprite = sprite,
        origin = {
            x = sprite:getWidth() / 2,
            y = sprite:getHeight() / 2
        }
    }
end

function love.load()
    lume = require "lume"

    data_file_path = "savedata.txt"

    screen_canvas = love.graphics.newCanvas(400, 600)

    local face_sprite = love.graphics.newImage("face.png")

    shake_time = 0
    shake_wait = 0
    shake_offset = { x = 0, y = 0 }

    player1 = make_player(100, 100, face_sprite)
    player2 = make_player(300, 100, face_sprite)

    coins = {}
    local coin_sprite = love.graphics.newImage("dollar.png")

    if love.filesystem.getInfo(data_file_path) then
        local file = love.filesystem.read(data_file_path)
        local data = lume.deserialize(file)
        player1.x = data.player1.x
        player1.y = data.player1.y
        player1.size = data.player1.size

        for i, coin in ipairs(data.coins) do
            coins[i] = make_coin(coin.x, coin.y, coin_sprite)
        end
    else
        local window_width = love.graphics.getWidth()
        local window_height = love.graphics.getHeight()
        local margin_x = 50
        local margin_y = 40
        for _ = 1, 25 do
            table.insert(coins,
                make_coin(love.math.random(margin_x, window_width - margin_x),
                    love.math.random(margin_y, window_height - margin_y),
                    coin_sprite))
        end
    end
end

local function save_game()
    local data = {}
    data.player1 = { x = player1.x, y = player1.y, size = player1.size }
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

local function is_circle_collision_base(center1, center2, radius1, radius2)
    return (center2.x - center1.x) ^ 2 + (center2.y - center1.y) ^ 2
        < (radius1 + radius2) ^ 2
end

local function is_circle_collision(first, second)
    return is_circle_collision_base({ x = first.x, y = first.y },
        { x = second.x, y = second.y }, first.size, second.size)
end

local function update_player(player, other, move_keys, dt)
    local x = player.x
    local y = player.y
    if love.keyboard.isDown(move_keys.left) then
        x = x - player.dx * dt
        if x < player.size then
            x = player.size
        end
    elseif love.keyboard.isDown(move_keys.right)  then
        x = x + player.dx * dt
        local window_width = love.graphics.getWidth()
        if x > window_width - player.size then
            x = window_width - player.size
        end
    end

    if  love.keyboard.isDown(move_keys.up) then
        y = y - player.dy * dt
        if y < player.size then
            y = player.size
        end
    elseif love.keyboard.isDown(move_keys.down)  then
        y = y + player.dy * dt
        local window_height = love.graphics.getHeight()
        if y > window_height - player.size then
            y = window_height - player.size
        end
    end

    if not is_circle_collision_base({ x = x, y = y },
            { x = other.x, y = other.y }, player.size, other.size) then
        player.x = x
        player.y = y
    end
end

local function try_pick_coin(player, i)
    if is_circle_collision(player, coins[i]) then
        table.remove(coins, i)
        player.size = player.size + 1
        player.score = player.score + 1
        --shake_time = 0.1789
        return true
    else
        return false
    end
end

function love.update(dt)
    if shake_time > 0 then
        shake_time = shake_time - dt
        if shake_wait > 0 then
            shake_wait = shake_wait - dt
            shake_offset.x = 0
            shake_offset.y = 0
        else
            shake_offset.x = love.math.random(-2, 2)
            shake_offset.y = love.math.random(-2, 2)
            shake_wait = 0.1
        end
    end

    update_player(player1, player2,
        { left = "left", right = "right", up = "up", down = "down" }, dt)
    update_player(player2, player1,
        { left = "a", right = "d", up = "w", down = "s" }, dt)

    for i = #coins, 1, -1 do
        if not try_pick_coin(player1, i) then
            try_pick_coin(player2, i)
        end
    end
end

local function draw_game(focus)
    love.graphics.push()

    love.graphics.scale(0.8)

    love.graphics.translate(200 - focus.x, 300 - focus.y)

    if shake_time > 0 then
        love.graphics.translate(shake_offset.x, shake_offset.y)
    end

    love.graphics.circle("line", player1.x, player1.y, player1.size)
    love.graphics.draw(player1.sprite, player1.x, player1.y, 0, 1, 1,
        player1.origin.x, player1.origin.y)

    love.graphics.circle("line", player2.x, player2.y, player2.size)
    love.graphics.draw(player2.sprite, player2.x, player2.y, 0, 1, 1,
        player2.origin.x, player2.origin.y)

    for _, coin in ipairs(coins) do
        love.graphics.circle("line", coin.x, coin.y, coin.size)
        love.graphics.draw(coin.sprite, coin.x, coin.y, 0, 1, 1,
            coin.origin.x, coin.origin.y)
    end

    love.graphics.pop()
end

function love.draw()
    love.graphics.setCanvas(screen_canvas)
    love.graphics.clear()

    draw_game(player1)

    love.graphics.setCanvas()
    love.graphics.draw(screen_canvas)

    love.graphics.setCanvas(screen_canvas)
    love.graphics.clear()

    draw_game(player2)

    love.graphics.setCanvas()
    love.graphics.draw(screen_canvas, 400)

    love.graphics.line(400, 0, 400, 600)

    love.graphics.print("Player 1 - " .. player1.score, 10, 10)
    love.graphics.print("Player 2 - " .. player2.score, 10, 30)
end
