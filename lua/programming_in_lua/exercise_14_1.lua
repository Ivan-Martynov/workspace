-- Add to sparse matrices.
local function sparse_matrix_sum(first, second)
    local result = {}
    for i, first_row in pairs(first) do
        local second_row = second[i]
        if second_row then
            result[i] = {}
            for j, first_value in pairs(first_row) do
                if second_row[j] then
                    result[i][j] = first_value + second_row[j]
                end
            end
        end
    end
    return result
end

local function print_matrix(matrix)
    for i, row in pairs(matrix) do
        for j, value in pairs(row) do
            io.write("M[", i, ", ", j, "] = ", value, "\n")
        end
    end
end

local first_matrix = {
    [8] = { [2] = 3, [5] = 5 },
    [12] = { [3] = 7, [4] = 9, [6] = 23 },
}

local second_matrix = {
    [12] = { [2] = 13, [5] = 15, [3] = 11 },
    [8] = { [2] = 17, [5] = 19 },
}

print_matrix(sparse_matrix_sum(first_matrix, second_matrix))
