local function set_default(t, d)
    setmetatable(t, {
        __index = function()
            return d
        end
    })
end

local key = {}
-- Shared metatable.
local mt_shared = {
    __index = function(t)
        return t[key]
    end,

    __call = function (t, param)
        local sum = 0
        for _, value in pairs(t) do
            sum = value + param
        end
        return sum
    end
}
local function set_default_shared(t, d)
    t[key] = d
    setmetatable(t, mt_shared)
end

local function track_access(t)
    local proxy = {}

    local mt = {
        __index = function(_, k)
            print("*access to element " .. tostring(k))
            return t[k]
        end,

        __newindex = function(_, k, v)
            print("*update element " .. tostring(k) .. " to " .. tostring(v))
            t[k] = v
        end,

        __pairs = function()
            return function(_, k)
                local next_key, next_value = next(t, k)
                if next_key ~= nil then
                    print("*traversing element " .. tostring(next_key))
                end
                return next_key, next_value
            end
        end,

        __len = function()
            return #t
        end,

        __metatable = "Not your business.",
    }

    setmetatable(proxy, mt)
    return proxy
end

local private_index = {}

local read_only_mt_shared = {
    __index = function(t, k)
        return t[private_index][k]
    end,

    __newindex = function(_, _, _)
        error("Attemp to update a read-only table", 2)
    end,
}

local function read_only_shared(t)
    return setmetatable({ [private_index] = t }, read_only_mt_shared)
end

local function read_only(t)
    local proxy = {}

    local mt = {
        __index = t,
        __newindex = function(_, _, _)
            error("Attemp to update a read-only table", 2)
        end
    }

    setmetatable(proxy, mt)
    return proxy
end

local t1 = {}
t1 = track_access(t1)

--setmetatable(t1, {})

t1[2] = "Hello"
print(t1[2])

local t2 = track_access({10, 20})
print(#t2)
for k, v in pairs(t2) do
    print(k, v)
end

--local days = read_only({ "Monday", "Tuesday", "Wednesday", "Thursday",
--    "Friday", "Saturday", "Sunday" })
local days = read_only_shared({ "Monday", "Tuesday", "Wednesday", "Thursday",
    "Friday", "Saturday", "Sunday" })
--read_only_shared(days)
print(days[3])
--days[1] = "Nonday" -- raises an error.
print(days[1])

local t3 = {}
set_default_shared(t3, 3)
t3[2] = 5
print(t3[2], t3.z)
print(t3(3))
