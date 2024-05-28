local function factorial(n)
    if n < 0 then
        print("Factorial is not defined for negative numbers (" .. n .. ")")
        return nil
    end

    if n == 0 then
        return 1
    else
        return n * factorial(n - 1)
    end
end

print("Enter a number:")
local n = io.read("*n")
print(factorial(n))
