local N = 8

local check_count = 0

-- Check whether position (n, c) is safe.
local function is_place_ok(a, n, c)
    check_count = check_count + 1
    for i = 1, n - 1 do
        --if (a[i] == c) or (a[i] - i == c - n) or (a[i] + i == c + n) then
        if (a[i] == c) or (math.abs(a[i] - c) == math.abs(n - i)) then
            return false
        end
    end

    return true
end

local function print_board(a)
    for i = 1, N do
        for j = 1, N do
            io.write(a[i] == j and "X" or "-", " ")
        end
        io.write("\n")
    end
    io.write("\n")
end

local solution_count = 0
local function add_queen(a, n)
    if n > N then
        solution_count = solution_count + 1
        print_board(a)
        --return true
    else
        for c = 1, N do
            if is_place_ok(a, n, c) then
                a[n] = c
                add_queen(a, n + 1)
                --if (add_queen(a, n + 1)) then
                    --return true
                --end
            end
        end
        return false
    end
end

add_queen({}, 1)

print("Check performed " .. check_count .. " times.")
print("Solution count = " .. solution_count)
