local function print_all(a)
    for i, v in pairs(a) do
        print(i .. " => " .. tostring(v))
    end
end

local function all_except_first(...)
    return select(2, ...)
end

local function all_except_last(...)
    local n = select("#", ...)
    if n > 1 then
        return select(1, ...), all_except_last(select(2, ...))
    end
end


print_all({1, 3, ["x"] = "hello"})
print("\nAll except first:")
print_all(table.pack(all_except_first(1, 3, "hello", true)))
print("\nAll except last:")
print_all(table.pack(all_except_last(1, 3, "hello", true)))
