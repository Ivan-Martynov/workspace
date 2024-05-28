local function is_boolean(value)
    return (value == true) or (value == false)
end 

print(is_boolean(3))
print(is_boolean(nil))
print(is_boolean(false))
print(is_boolean(true))
print(is_boolean(0))
print(is_boolean("Test"))
print(is_boolean({ true }))
