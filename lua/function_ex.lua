
function cat_strings(...)
    res = ""
    for k, s in pairs{...} do
        res = res .. s
    end

    return res
end

function print_array(a)
    print(table.unpack(a))
end

function all_except_first(a, ...)
    return ...
end

--print(cat_strings("hello", " test ", "string"))

--a = {"hello", 3, true}
--print_array(a)

print(all_except_first("simple", 2, true, "testing"))
print(all_except_first())
