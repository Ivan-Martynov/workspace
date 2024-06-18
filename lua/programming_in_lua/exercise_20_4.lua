local function string_to_table(str)
    local t = {}
    str:gsub(".", function(b)
        t[#t + 1] = b
    end)
    return t
end

local function file_as_array(file_path)
    local proxy = {}
    --local input = assert(io.open(file_path, "rb"))

    local mt = {
        __index = function(_, k)
            local input = assert(io.open(file_path, "rb"))

            assert(input:seek("set", k - 1))
            local bytes = input:read(1)

            assert(input:close())

            return bytes
        end,

        __newindex = function(_, k, v)
            local input = assert(io.open(file_path, "rb"))
            local bytes = input:read("a")
            assert(input:close())
            local s = bytes:sub(1, k - 1) .. v .. bytes:sub(k + 1, -1)
            input = assert(io.open(file_path, "wb"))

            --assert(input:seek("set", k))
            assert(input:write(s))

            assert(input:close())
        end,

        __next = function(_, k)
            local input = assert(io.open(file_path, "rb"))
            print("* calling next" .. tostring(k))
            assert(input:seek("set", k - 1))
            local bytes = input:read(1)
            assert(input:close())

            return k, bytes
        end,

        __pairs = function()
            local input = assert(io.open(file_path, "rb"))
            local bytes = string_to_table(input:read("a"))
            assert(input:close())

            return function(_, k)
                local next_key, next_value = next(bytes, k)
                return next_key, next_value
            end
        end,

        __len = function()
            local input = assert(io.open(file_path, "rb"))
            local size = assert(input:seek("end"))
            assert(input:close())

            return size
        end
    }

    setmetatable(proxy, mt)
    return proxy
end

local file_path = "files/sample.txt"
--local file_path = "files/test.bmp"

--print(t)
local t = file_as_array(file_path)
io.write(string.format("0x%X\n", string.byte(t[1])))
print(#t)

local i = 0
for _, v in pairs(t) do
    io.write(string.format("0x%X ", string.byte(v)))
    if v == "1" then
        i = i + 1
    end
end
io.write("\n", i, "\n")

