
local Player = require "player"
local Enemy = require "enemy"
local Bullet = require "bullet"

local player
local enemy
local bullets

function love.load()
    player = Player()
    enemy = Enemy()

    bullets = {}
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    elseif key == "space" then
        table.insert(bullets,
            Bullet(player.x + player.width / 2, player.y + player.height))
    end
end

function love.update(dt)
    player:update(dt)
    enemy:update(dt)

    for i, bullet in ipairs(bullets) do
        bullet:update(dt)

        if bullet:check_aabb_collision(enemy) then
            if enemy.speed > 0 then
                enemy.speed = enemy.speed + 50
            else
                enemy.speed = enemy.speed - 50
            end
            bullet.used = true
        end

        if bullet.used then
            table.remove(bullets, i)
        end

        if bullet.y > love.graphics.getHeight() then
            love.load()
        end
    end
end

function love.draw()
    player:draw()
    enemy:draw()

    for _, bullet in ipairs(bullets) do
        bullet:draw()
    end
end
