local ball = require "ball"
local bricks = require "bricks"

-- Levels.
local levels = {
    current_level = 1,
    game_finished = false,
    sequence = require "levels.sequence",
}

function levels.require_current_level()
    local file_name = "levels/" .. levels.sequence[levels.current_level]
    local level = require(file_name)
    return level
end

function levels.switch_to_next_level()
    if bricks.no_more_bricks then
        if levels.current_level < #levels.sequence then
            levels.current_level = levels.current_level + 1
            bricks.construct_level(levels.require_current_level())
            ball.reposition()
        else
            levels.game_finished = true
        end
    end
end

return levels
