local stack = { first = 0, last = 0 }

function stack:is_empty()
    return self.first == self.last
end

function stack:push(value)
    self.last = self.last + 1
    self[self.last] = value
end

function stack:new(t)
    self.__index = self
    t = setmetatable(t or {}, self)
    for _, v in pairs(t) do
        t:push(v)
    end
    return t
end

function stack:pop()
    if self:is_empty() then
        return
    end

    local value = self[self.last]

    self[self.last] = nil
    self.last = self.last - 1

    return value
end

function stack:top()
    return self[self.last]
end

local stack_queue = stack:new()

function stack_queue:insert_bottom(value)
    self[self.first] = value
    self.first = self.first - 1
end

local s1 = stack:new()
s1:push(5.3)
s1:push("string")
s1:push(true)
s1:push(false)

local s2 = stack_queue:new({8, -9, 23.89024})
s2:push(1000.0)
s2:insert_bottom(0.001)

while not s1:is_empty() do
    print(s1:pop())
end

while not s2:is_empty() do
    print(s2:pop())
end
