local MouseController = {}

function MouseController.clicked_point(button)
    return love.mouse.isDown(button) and love.mouse.getPosition() or nil
end

return MouseController
