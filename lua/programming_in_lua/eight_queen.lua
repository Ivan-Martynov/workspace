local N = 8

-- Check whether position (n, c) is safe.
local function is_place_ok(a, n, c)
    for i = 1, n - 1 do
        if (a[i] == c) or (a[i] - i == c - n) or (a[i] + i == c + n) then
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

local function add_queen(a, n)
    if n > N then
        print_board(a)
    else
        for c = 1, N do
            if is_place_ok(a, n, c) then
                a[n] = c
                add_queen(a, n + 1)
            end
        end
    end
end

add_queen({}, 1)
