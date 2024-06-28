local gamestates = require "gamestates"

local menu = {}

function menu.load()
end

function menu.enter()
end

function menu.update(_)
end

function menu.draw()
    love.graphics.print("Menu gamestate. Press Enter to continue.", 280, 250)
end

function menu.keyreleased(key)
    if key == "return" then
        gamestates.set_state("game", { level = 1 })
    elseif key == "escape" then
        love.event.quit()
    end
end

function menu.exit()
end

return menu
