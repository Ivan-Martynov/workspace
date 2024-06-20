local MovableObject2d = require "movable_object_2d"
local ColorSchemeHelper = require "color_scheme_helper"

-- Make a paddle class from MovableObject2d class.
local Paddle = setmetatable({}, MovableObject2d)
Paddle.__index = Paddle

-- Create a new paddle using top-left corner coordinates, width, height and
-- keys, which determing paddle movement up and down.
function Paddle.new(x, y, width, height, keys, color)
    local instance = setmetatable(MovableObject2d.new(x, y, 0, 0, color),
        Paddle)
    instance.width = width
    instance.height = height
    instance.score = 0
    instance.keys = keys
    return instance
end

function Paddle:update(dt)
    local paddle_speed = 280
    -- Flags to determine if a paddle is to be moved up or down.
    local move_up, move_down = false, false

    -- Check if the touch/click was eligible for paddle movement.
    local function check_click(x, y)
        if (x >= self.x - self.width) and
            (x <= self.x + self.width * 2) then
            if y < self.y + self.height / 2 then
                move_up = true
            elseif y > self.y + self.height / 2 then
                move_down = true
            end
        end
    end

    -- Get all touches on the screen.
    for _, id in ipairs(love.touch.getTouches()) do
        local x, y = love.touch.getPosition(id)
        check_click(x, y)
    end

    -- Check mouse left button clicks.
    if love.mouse.isDown(1) then
        local x, y = love.mouse.getPosition()
        check_click(x, y)
    end

    -- Check if one of the assigned keys was pressed.
    if love.keyboard.isDown(self.keys.up) then
        move_up = true
    elseif love.keyboard.isDown(self.keys.down) then
        move_down = true
    end

    -- Change paddles speed depending whether it is moving up or down, or is
    -- remaining in place.
    if move_up then
        self:set_velocity(0, -paddle_speed)
    elseif move_down then
        self:set_velocity(0, paddle_speed)
    else
        self:set_velocity(0, 0)
    end

    -- Update paddle using movable object class.
    MovableObject2d.update(self, dt)

    -- Make sure the paddle doesn't go beyoned the screen.
    local _, window_height = love.window.getMode()
    self.y = math.min(math.max(self.y, 0), window_height - self.height)
end

-- Draw a paddle and its score. Optionally can provide a drawing mode.
function Paddle:draw(mode)
    -- Draw the paddle itself as a rectangle.
    love.graphics.setColor(self.color)
    love.graphics.rectangle(mode or "fill", self.x, self.y,
        self.width, self.height)

    local center_x = love.window.getMode() / 2

    -- Determine the half of the screen. Then printf function uses this value
    -- to position the text in the middle of the corresponding half.
    local x = math.min(self.x, center_x)

    love.graphics.setColor(ColorSchemeHelper.current["white"])

    local font = love.graphics.newFont(32)
    love.graphics.printf(self.score, font, x, font:getHeight(self.score),
        center_x, "center")
end

return Paddle
