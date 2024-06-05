#!/usr/bin/lua5.3
function get_all_factors(number)
    --[[--
    Gets all the factors of a given number

    @Parameter: number
        the number of the factors of

    @Returns: table of factors of the number

    --]]--
    local factors = {}
    for possible_factor = 1, math.sqrt(number), 1 do
        local remainder = number % possible_factor

        if remainder == 0 then
            local factor, factor_pair = possible_factor,
            number / possible_factor
            table.insert(factors, factor)

            if factor ~= factor_pair then
                table.insert(factors, factor_pair)
            end
        end
    end

    table.sort(factors)
    return factors
end

-- The meaning of the Universe is 42
the_universe = 42
factors_of_the_universe = get_all_factors(the_universe)

-- Print out each factor

print("Count", "The Factors of Life, the Universe and Evertything")
for i, v in ipairs(factors_of_the_universe) do
    print(i, v)
end

