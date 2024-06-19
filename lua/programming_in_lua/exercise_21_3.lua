local first = {}
local last = {}

local stack = {}

function stack:is_empty()
    return first[self] == last[self]
end

function stack:push(value)
    last[self] = last[self] + 1
    self[last[self]] = value
end

function stack:new(t)
    t = setmetatable(t or {}, self)
    self.__index = self

    first[t] = 0
    last[t] = 0

    for _, v in pairs(t) do
        t:push(v)
    end

    return t
end

function stack:pop()
    if self:is_empty() then
        return
    end

    --local value = self[self.last]
    local value = self[last[self]]

    self[last[self]] = nil
    last[self] = last[self] - 1

    return value
end

function stack:top()
    return self[last[self]]
end

local s1 = stack:new({4, "great"})
s1:push(5.3)
s1:push("string")
s1:push(true)
s1:push(false)

while not s1:is_empty() do
    print(s1:pop())
end
