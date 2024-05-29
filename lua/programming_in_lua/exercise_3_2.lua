
local function sum_times(value, n)
    local result = 0
    for _ = 1, n do
        result = result + value
    end
    return result
end

--[[
    Sum for mininteger switches from zero to mininteger. Since, mininteger is
    an even number, then multiplying mininteger by itself is summing it even
        number of times. And on each even step the value is zero.
]]
local n = 8
for i = 1, n do
    print("Sum for min: " .. i .. " => " .. sum_times(math.mininteger, i))
end

--[[
    maxinteger = mininteger - 1 =>
    maxinteger * maxinteger = (mininteger - 1) * (mininteger - 1) =
    mininteger * mininteger - (mininteger + mininteger) + 1 = 0 - 0 + 1 = 1
]]
for i = 1, n do
    print("Sum for max: " .. i .. " => " .. sum_times(math.maxinteger, i))
end
