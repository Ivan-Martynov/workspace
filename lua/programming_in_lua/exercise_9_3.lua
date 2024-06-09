local function newpoly(coefficients)
    return function(x)
        local result = coefficients[#coefficients] * x
        for i = #coefficients - 1, 2, -1 do
            result = (result + coefficients[i]) * x
        end
        return result + coefficients[1]
    end
end

local f = newpoly({2, 3, 4})
print(f(-1))
print(f(0))
print(f(1))
print(f(2))

