local gamestates = require "gamestates"

local game_finished = {}

function game_finished.load()
end

function game_finished.enter()
end

function game_finished.keyreleased(key)
    if (key == "return") then
        gamestates.set_state("menu")
    elseif key == "escape" then
        love.event.quit()
    end
end

function game_finished.update(_)
end

function game_finished.draw()
    local message = [[
        Congratulations!
        You have finished the game!
        Press enter to go to the main menu.
    ]]
    love.graphics.printf(message, 300, 250, 200, "center")
end

function game_finished.exit()
end

return game_finished
