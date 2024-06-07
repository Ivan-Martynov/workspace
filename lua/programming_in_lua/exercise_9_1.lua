local function integral(f, n)
    n = n or 1e2
    return function(a, b)
        local result = 0
        local step = (b - a) / n
        for i = 1, n + 1 do
            result = result + f(a + (i - 1) * step)
        end
        return result * step
    end
end

local c = integral(function(x)
    return x
end, 1000)
print(c(0, 1))
