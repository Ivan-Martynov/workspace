local function is_valid_sequenc(seq)
    local count = 1
    for i, v in pairs(seq) do
        if (i ~= count) or (v == nil) then
            return false
        end

        count = count + 1
    end

    return true
end

print("Is valid = " .. tostring(is_valid_sequenc({"a", "k", "p"})))
print("Is valid = " .. tostring(is_valid_sequenc({[1] = "a", [3] = "k", [4] = "p"})))
print("Is valid = " .. tostring(is_valid_sequenc({[1] = "a", [2] = nil, [3] = "p"})))
print("Is valid = " .. tostring(is_valid_sequenc({[1] = "a", [2] = "k", [3] = nil})))

local function add(...)
    local s = 0
    for i = 1, select("#", ...) do
        local t = select(i, ...)
        if t then
            s = s + t
        end
    end
    return s
end

local function add2(...)
    local s = 0
    local arg = table.pack(...)
    for i = 1, arg.n do
        if arg[i] then
            s = s + arg[i]
        end
    end
    return s
end

print(add(1, 2, 3))
print(add2(1, 2, nil, 3))
print(add(1, 2, 3, nil, 4))

