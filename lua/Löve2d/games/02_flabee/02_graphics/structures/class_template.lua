local ClassTemplate = {}
ClassTemplate.__index = ClassTemplate

-- Initialization function. Can be used by extended classes without any need to
-- rewrite the __call method.
function ClassTemplate:init(...)
end

-- Make class "callable", that is using as a constructor.
function ClassTemplate:__call(...)
    -- Set metatable to point to the class, giving access to its methods.
    local instance = setmetatable({}, self)

    -- Initialize the instance, using corresponding init function.
    instance:init(...)

    return instance
end

-- Extend the class.
function ClassTemplate:extend()
    local extended_class = setmetatable({}, self)

    extended_class.__index = extended_class

    extended_class.__call = self.__call

    -- Use parent field as a reference to the parent/super class.
    extended_class.parent = self

    return extended_class
end

return ClassTemplate
