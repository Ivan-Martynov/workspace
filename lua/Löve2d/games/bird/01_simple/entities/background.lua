local MovableObject2d = require "structures.movable_object_2d"
local ColorSchemeHelper = require "color_tools.color_scheme_helper"

local BackgroundClass = MovableObject2d:extend()
local CloudClass = MovableObject2d:extend()

local dx = -50

function CloudClass:init(x, y, main_radius)
    self.parent.init(self, x, y, dx, 0, ColorSchemeHelper.current.foreground)
    self.main_radius = main_radius
    local min_ratio = 0.6
    self.left_radius = main_radius * (min_ratio + love.math.random() * 0.2)
    self.right_radius = main_radius * (min_ratio + love.math.random() * 0.2)

    -- Width to check the bounds.
    self.width = self.main_radius * 2 + self.right_radius + self.left_radius
end

function CloudClass:draw()
    love.graphics.setColor(self.color)
    love.graphics.circle("fill", self.x, self.y, self.main_radius)
    love.graphics.circle("fill", self.x - self.main_radius, self.y,
        self.left_radius)
    love.graphics.circle("fill", self.x + self.main_radius, self.y,
        self.right_radius)
end

function BackgroundClass:init(x, y, color)
    self.parent.init(self, x, y, dx, 0, color)
    self.width, self.height = love.window.getMode()
end

function BackgroundClass:draw()
    local r, g, b, a = love.graphics.getColor()

    love.graphics.setColor(self.color)
    love.graphics.rectangle("fill", self.x, self.y, self.width, self.height)

    love.graphics.setColor(r, g, b, a)
end

local window_width = love.window.getMode()
local background = {
    left_part = BackgroundClass(0, 0, ColorSchemeHelper.current.background),
    right_part = BackgroundClass(window_width, 0,
        ColorSchemeHelper.current.background),
    cloud_x = 0,
    clouds = {},
}

function background:add_cloud()
    local cloud_min_main_radius = 20
    local cloud_max_main_radius = 40
    local cloud_min_y = cloud_max_main_radius * 1.5
    local cloud_max_y = cloud_max_main_radius * 2.5

    if self.cloud_x - cloud_max_main_radius * 2 <= window_width then
        table.insert(self.clouds,
            CloudClass(self.cloud_x, love.math.random(cloud_min_y, cloud_max_y),
                love.math.random(cloud_min_main_radius, cloud_max_main_radius)))

        self.cloud_x = self.cloud_x +
            love.math.random(cloud_max_main_radius * 4,
                cloud_max_main_radius * 5)
    end
end

function background:update(dt)
    self.left_part:update(dt)
    self.right_part:update(dt)

    self:add_cloud()

    for _, cloud in ipairs(self.clouds) do
        cloud:update(dt)
    end

    if self.clouds[1].x < -self.clouds[1].width * 0.5 then
        table.remove(self.clouds, 1)
    end

    self.cloud_x = self.cloud_x + dx * dt
end

function background:draw()
    self.left_part:draw()
    self.right_part:draw()

    if self.left_part.x < -self.left_part.width then
        self.left_part.x = self.right_part.x + self.right_part.width
    elseif self.right_part.x < -self.right_part.width then
        self.right_part.x = self.left_part.x + self.left_part.width
    end

    for _, cloud in ipairs(self.clouds) do
        cloud:draw()
    end
end

return background
