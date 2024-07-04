local MovableObject2d = require "movable_object_2d"
local SoundHelper = require "sound_helper"
local ColorSchemeHelper = require "color_scheme_helper"

-- Make a ball class from MovableObject2d class.
local Ball = setmetatable({}, MovableObject2d)
Ball.__index = Ball

-- Generate ball's initial velocity, randomly choosing its direction and having
-- its value within given limits.
local function generate_initial_velocity()
    -- Note: love.math.random already generates seed, so no need to explicitly
    -- doing it again.
    local dx = love.math.random(150, 200)
    local dy = love.math.random(150, 200)

    if love.math.random(2) == 1 then
        dx = -dx
    end

    if love.math.random(2) == 1 then
        dy = -dy
    end

    return dx, dy
end

-- Reset ball's position and velocity.
function Ball:reset()
    local window_width, window_height = love.window.getMode()
    self.x = window_width / 2
    self.y = window_height / 2

    self.dx, self.dy = generate_initial_velocity()
end

-- Create a new ball using centre coordinates, width, height and keys, which
-- determing paddle movement up and down.
function Ball.new(x, y, radius, color)
    local dx, dy = generate_initial_velocity()

    local instance = setmetatable(MovableObject2d.new(x, y, dx, dy, color),
        Ball)
    instance.radius = radius

    return instance
end

--! @brief Helper function to clamp the value within the given range.
--! @param x Value to clamp.
--! @param a Range's smaller value.
--! @param b Range's larger value.
local function clamp(x, a, b)
    if x < a then
        return a
    elseif x > b then
        return b
    else
        return x
    end
end

--! @brief Helper function to check whether the ball collides with a paddle.
--! @param rect Rectangle representing a paddle.
local function ball_collides(ball, rect)
    local closest_x = clamp(ball.x, rect.x, rect.x + rect.width)
    local closest_y = clamp(ball.y, rect.y, rect.y + rect.height)

    return (ball.x - closest_x) ^ 2 + (ball.y - closest_y) ^ 2
        <= ball.radius ^ 2
end

-- Resolve ball's collisions with paddles. Function changes ball's velocity
-- if the ball bounces from a paddle or top/bottom wall. It also adjusts ball's
-- position to avoid repetitive collisions. If the ball passes by a paddle, the
-- function increments the opponent's score.
function Ball:resolve_collisions(left_paddle, right_paddle)
    local _, window_height = love.window.getMode()
    if ball_collides(self, left_paddle) then
        self:set_velocity(-self.dx * 1.1, self.dy * 1.1)
        self.x = left_paddle.x + left_paddle.width + self.radius
        SoundHelper.hit_paddle:play()
    elseif ball_collides(self, right_paddle) then
        self:set_velocity(-self.dx * 1.1, self.dy * 1.1)
        self.x = right_paddle.x - self.radius
        SoundHelper.hit_paddle:play()
    elseif self.y <= self.radius then
        self:set_velocity(self.dx, -self.dy)
        self.y = self.radius
        SoundHelper.hit_wall:play()
    elseif self.y + self.radius >= window_height then
        self:set_velocity(self.dx, -self.dy)
        self.y = window_height - self.radius
        SoundHelper.hit_wall:play()
    elseif (self.x < left_paddle.x + left_paddle.width) then
        right_paddle.score = right_paddle.score + 1
        SoundHelper.score:play()
        self:reset()
    elseif (self.x > right_paddle.x) then
        left_paddle.score = left_paddle.score + 1
        SoundHelper.score:play()
        self:reset()
    end
end

-- Draw the ball as a circle. Optionally can provide a drawing mode.
function Ball:draw(mode)
    love.graphics.setColor(self.color)
    love.graphics.circle(mode or "fill", self.x, self.y, self.radius)
    love.graphics.setColor(ColorSchemeHelper.current["white"])
end

return Ball
