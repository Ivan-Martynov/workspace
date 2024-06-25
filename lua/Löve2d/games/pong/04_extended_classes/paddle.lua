local MovableObject2d = require "movable_object_2d"
local ColorSchemeHelper = require "color_scheme_helper"
local MovePaddleCommand = require "move_paddle_command"

local InputController = require "input_controller"
local MouseController = require "mouse_controller"

-- Make a paddle class from MovableObject2d class.
local Paddle = MovableObject2d:extend()

function Paddle:reset()
    local _, window_height = love.window.getMode()
    self.y = (window_height - self.height) / 2
    self.score = 0
end

-- Create a new paddle using top-left corner coordinates, width, height and
-- keys, which determing paddle movement up and down.
function Paddle:init(x, y, width, height, keys, color)
    self.parent.init(self, x, y, 0, 0, color)
    self.width = width
    self.height = height
    self.score = 0
    self.keys = keys
end

function Paddle:update(dt)
    -- Using commands for paddle movement.
    local paddle_speed = 280
    local move_paddle_up_command = MovePaddleCommand(self, -paddle_speed, dt)
    local move_paddle_down_command = MovePaddleCommand(self, paddle_speed, dt)

    -- Check if the touch/click was eligible for paddle movement.
    local function check_click(x, y)
        if (x >= self.x - self.width) and
            (x <= self.x + self.width * 2) then
            if y < self.y + self.height / 2 then
                move_paddle_up_command:execute()
            elseif y > self.y + self.height / 2 then
                move_paddle_down_command:execute()
            end
        end
    end

    -- Get all touches on the screen.
    for _, id in ipairs(love.touch.getTouches()) do
        local x, y = love.touch.getPosition(id)
        check_click(x, y)
    end

    -- Check mouse left button clicks.
--    if MouseController.clicked_point(1) then end
    if love.mouse.isDown(1) then
        local x, y = love.mouse.getPosition()
        check_click(x, y)
    end

    -- Check if one of the assigned keys was pressed.
    if InputController.key_pressed(self.keys.up) then
        move_paddle_up_command:execute()
    elseif InputController.key_pressed(self.keys.down) then
        move_paddle_down_command:execute()
    end

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
