local gamestates = require "gamestates"

function love.load()
    gamestates.set_state("menu")
end

function love.keyreleased(key)
    gamestates.state_event("keyreleased", key)
end

function love.update(dt)
    gamestates.state_event("update", dt)
end

function love.draw()
    gamestates.state_event("draw")
end

function love.quit(mode)
    print(mode == "restart" and "Try again!" or "See ya!")
end
