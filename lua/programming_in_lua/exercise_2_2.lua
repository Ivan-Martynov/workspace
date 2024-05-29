local N = tonumber(arg[1]) or 8

local function perm_gen(a, n, res)
    if n == 0 then
        local row = {}
        for _, v in ipairs(a) do
            table.insert(row, v)
        end
        table.insert(res, row)
    else
        for i = 1, n do
            a[n], a[i] = a[i], a[n]

            perm_gen(a, n - 1, res)

            a[n], a[i] = a[i], a[n]
        end
    end
end

local function make_permutations(n)
    local a = {}
    for i = 1, n do
        a[i] = i
    end

    local result = {}
    perm_gen(a, #a, result)
    return result
end

local permutations = make_permutations(N)

local function print_board_line(start_symbol, middle_symbol, end_symbol)
    io.write(start_symbol, "───")
    for _ = 1, N - 1 do
        io.write(middle_symbol, "───")
    end
    io.write(end_symbol, "\n")
end

local function print_board(a)
    print_board_line("┌", "┬", "┐")

    for i = 1, N do
        io.write("│")
        for j = 1, N do
            io.write(a[i] == j and " ♛ │" or "   │")
        end
        io.write("\n")

        if i ~= N then
            print_board_line("├", "┼", "┤")
        end
    end

    print_board_line("└", "┴", "┘")
end

local check_count = 0
local function is_solution(v)
    for i = 1, N do
        for j = i + 1, N do
            check_count = check_count + 1
            if (math.abs(v[j] - v[i]) == math.abs(i - j)) then
                return false
            end
        end
    end

    return true
end

local solution_count = 0
for _, v in ipairs(permutations) do
    if is_solution(v) then
        solution_count = solution_count + 1
        print_board(v)
    end
end

print("Permutation count = " .. #permutations)
print("Solution count = " .. solution_count)
print("Check performed " .. check_count .. " times.")
