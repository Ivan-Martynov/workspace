local ClassTemplate = require "structures.class_template"

local IGameState = ClassTemplate:extend()

function IGameState:load(...)
end

function IGameState:update(_)
end

function IGameState:draw()
end

function IGameState:unload()
end

return IGameState
