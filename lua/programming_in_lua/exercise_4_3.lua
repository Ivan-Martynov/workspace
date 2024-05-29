local s = "Жähdään"
for i = 1, utf8.len(s) do
    print(utf8.offset(s, i))
end
print("Code point = " .. utf8.codepoint(s, utf8.offset(s, 5)))
print(utf8.len(s) .. " vs " .. #s)

local function insert(first, i, second)
    local k = utf8.offset(first, i)
    return first:sub(1, k - 1) .. second .. first:sub(k)
end

local function remove(str, i, len)
    local string_length = utf8.len(str)
    if i + len > string_length + 1 then
        len = string_length - i + 1
    end
    return str:sub(1, utf8.offset(str, i) - 1) ..
        str:sub(utf8.offset(str, i + len))
end

local function is_palindrom(str)
    str = str:gsub("%s", "")
    str = str:gsub("%p", "")

    local string_length = utf8.len(str)
    for i = 1, math.floor(string_length / 2) do
        if utf8.codepoint(str, utf8.offset(str, i)) ~=
            utf8.codepoint(str, utf8.offset(str, string_length - i + 1)) then
            return false
        end
    end
    return true
end

print(insert("hello world", 1, "start: "))
print(insert("hello world", 7, "small "))
print(insert("ação", 3, "!"))

print(remove("hello world", 7, 4))
print(remove("ação", 3, 2))

local function debug_palindrom(str)
    print("Is \"", str, "\" a palindrom? " .. tostring(is_palindrom(str)))
end

debug_palindrom("step, on; ! no ... pets?")
debug_palindrom("banana")
debug_palindrom("açãёo,;.ж.?! oёãça")
