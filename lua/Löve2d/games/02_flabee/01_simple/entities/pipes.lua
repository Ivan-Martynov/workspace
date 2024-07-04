local MovableObject2d = require "structures.movable_object_2d"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"

local ground = require "entities.ground"

local dx = -100

local window_width, window_height = love.window.getMode()

local pipe_color = ColorSchemeHelper.current.green

local collar_width = 60
local collar_height = 20

local PipeClass = MovableObject2d:extend()

function PipeClass:set_tube(x, y, width, height)
    self.tube = {
        x = x,
        y = y,
        width = width,
        height = height,
    }
end

function PipeClass:set_collar(width, height, is_upper)
    self.collar = {
        width = width,
        height = height,
    }
    self.collar.x = self.tube.x - (self.collar.width - self.tube.width) / 2
    if is_upper then
        self.collar.y = self.tube.y + self.tube.height
    else
        self.collar.y = self.tube.y - self.collar.height
    end
end

function PipeClass:init(x, y, tube_width, tube_height, is_upper, color)
    self.parent.init(self, x, y, dx, 0, color)

    self:set_tube(x, y, tube_width, tube_height)
    self:set_collar(self.tube.width * 1.2, collar_height, is_upper)
end

function PipeClass:update(dt)
    self.parent.update(self, dt)

    self.tube.x = self.tube.x + self.dx * dt
    self.collar.x = self.collar.x + self.dx * dt
end

function PipeClass:draw()
    local r, g, b, a = love.graphics.getColor()

    love.graphics.setColor(self.color)
    love.graphics.rectangle("fill", self.tube.x, self.tube.y,
        self.tube.width, self.tube.height)
    love.graphics.rectangle("fill", self.collar.x, self.collar.y,
        self.collar.width, self.collar.height)

    love.graphics.setColor(r, g, b, a)
end

local current_x = window_width + collar_width

local pipes = {
    pairs = {}
}

function pipes:reset()
    for i = #self.pairs, 1, -1 do
        table.remove(self.pairs, i)
    end
    self.pairs = {}
    current_x = window_width + collar_width
end

function pipes:update(dt)
    current_x = current_x + dx * dt
    self:add_pipe_pair()

    for _, pipe_pair in ipairs(self.pairs) do
        pipe_pair.upper:update(dt)
        pipe_pair.lower:update(dt)
    end

    for i = #self.pairs, 1, -1 do
        local pipe_pair = self.pairs[i]
        if pipe_pair.upper.collar.x + pipe_pair.upper.collar.width < 0 then
            table.remove(self.pairs, i)
        end
    end
end

function pipes:draw()
    for _, pipe_pair in ipairs(self.pairs) do
        pipe_pair.upper:draw()
        pipe_pair.lower:draw()
    end
end

function pipes:add_pipe_pair()
    if current_x > window_width + collar_width then
        return
    end

    local horizontal_gap_min = window_height * 0.4
    local horizontal_gap_max = window_height * 0.5
    local horizontal_gap = love.math.random(horizontal_gap_min,
        horizontal_gap_max)

    local vertical_gap_min = 120
    local vertical_gap_max = 135
    local vertical_gap = love.math.random(vertical_gap_min, vertical_gap_max)

    local tube_width = 50
    local tube_min_height = collar_height * 2
    local tube_max_height = window_height * 0.75 - ground.height
    local upper_tube_height = love.math.random(tube_min_height, tube_max_height)
    local lower_tube_height = window_height - ground.height
        - upper_tube_height - vertical_gap

    local upper_pipe = PipeClass(current_x, 0, tube_width, upper_tube_height,
        true, pipe_color)
    local lower_pipe = PipeClass(current_x, window_height - ground.height
        - lower_tube_height, tube_width, lower_tube_height, false, pipe_color)

    local pair = {
        upper = upper_pipe,
        lower = lower_pipe,
        scored = false,
    }

    table.insert(self.pairs, pair)

    table.insert(self, upper_pipe)
    table.insert(self, lower_pipe)

    current_x = current_x + horizontal_gap
end

return pipes
