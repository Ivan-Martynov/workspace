local ball = require("entities/ball")
local paddle = require("entities/paddle")
local boundary_horizontal = require("entities/boundary-horizontal")
local boundary_vertical = require("entities/boundary-vertical")
local brick = require("entities/brick")
local pause_text = require("entities/pause-text")
local game_over_text = require("entities/game-over-text")
local stage_clear_text = require("entities/stage-clear-text")

local entities = {
    boundary_horizontal(400, 606, true),
    boundary_vertical(-6, 300),
    boundary_vertical(806, 300),
    boundary_horizontal(400, -6),
    paddle(200, 500),
    ball(200, 200),
    pause_text(),
    game_over_text(),
    stage_clear_text(),
}

local brick_width = 50
local brick_height = 20

local row_count = 1
local col_count = 3

local brick_gap_x = 10
local brick_gap_y = 5

local total_width = col_count * (brick_width + brick_gap_x) - brick_gap_x
local total_height = row_count * (brick_height + brick_gap_y) - brick_gap_y

local window_width, window_height = love.window.getMode()

local brick_start_x = math.floor((window_width - total_width + brick_width) / 2)
local brick_start_y = window_height / 2 -
    (window_height / 2 + total_height) / 2 - brick_height / 2

for row = 0, row_count - 1 do
    local brick_y = brick_start_y + row * (brick_height + brick_gap_y)
    for col = 0, col_count - 1 do
        local brick_x = brick_start_x + col * (brick_width + brick_gap_x)

        if (brick_x >= 0) and (brick_x <= window_width) and
            (brick_y >= 0) and (brick_y <= window_height) then
            table.insert(entities, brick(brick_x, brick_y))
        end
    end
end

return entities
