local Account = { balance = 0 }

function Account:deposit(v)
    self.balance = self.balance + v
end

--local mt = { __index = Account }

function Account:new(t)
    t = t or {}
    self.__index = self
    setmetatable(t, self)

    --print("Before setmetatable:\t", t)
    --local v = setmetatable(t or {}, mt)
    --print("Metatable:\t\t", v)
    --print("After setmetatable:\t", t)

    return t
end

function Account:withdraw(v)
    if v > self.balance then
        io.write("Insufficient funds.\n")
    else
        self.balance = self.balance - v
    end
end

local SpecialAccount = Account:new()

function SpecialAccount:get_limit()
    return self.limit or 0
end

function SpecialAccount:withdraw(v)
    if v > self.balance + self:get_limit() then
        io.write("Insufficient funds.\n")
    else
        self.balance = self.balance - v
    end
end

return Account
