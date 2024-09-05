local StateController = {}

function StateController:init(states)
    for key, value in pairs(states) do
        self[key] = value
    end
end

function StateController:set_state(name, ...)
    if StateController.current then
        StateController.current:unload()
    end

    StateController.current = StateController[name]()
    StateController.current.name = name
    StateController.current:load(...)
end

return StateController
