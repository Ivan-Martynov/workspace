local function polynomial_value(coefficients, x)
    local result = coefficients[#coefficients] * x
    for i = #coefficients - 1, 2, -1 do
        result = (result + coefficients[i]) * x
    end
    return result + coefficients[1]
end

local function debug_polynomial(coefficients, x)
    io.write(coefficients[1])
    for i = 2, #coefficients do
        io.write(" + " .. coefficients[i] .. " * " .. x .. "^" .. i - 1)
    end
    io.write(" = " .. polynomial_value(coefficients, x) .. "\n")
end

local coefficients = {2, 3, 4}
debug_polynomial(coefficients, -1)
debug_polynomial(coefficients, 0)
debug_polynomial(coefficients, 1)
debug_polynomial(coefficients, 2)
