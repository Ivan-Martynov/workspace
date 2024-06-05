local function shuffle_list(a)
    local result = {}
    math.randomseed(os.time())

    while #a > 1 do
        local r = math.random(#a)
        result[#result + 1] = a[r]
        table.remove(a, r)
    end
    result[#result + 1] = a[1]

    return result
end

local b = shuffle_list({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })
for _, v in ipairs(b) do
    io.write(v, " ")
end
print()
