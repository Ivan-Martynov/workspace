local account = require "account"

local function search(k, parent_list)
    for _, v in ipairs(parent_list) do
        local value = v[k]
        if value then
            return value
        end
    end
end

local function create_class(...)
    local c = {}
    local parents = { ... }
    setmetatable(c, {
        __index = function(_, k)
            return search(k, parents)
        end
    })

    c.__index = c

    function c:new(t)
        return setmetatable(t or {}, c)
    end

    return c
end

local named = {}
function named:get_name()
    return self.name
end

function named:set_name(name)
    self.name = name
end

local named_account = create_class(account, named)

local a1 = named_account:new { name = "John" }
print(a1:get_name())
