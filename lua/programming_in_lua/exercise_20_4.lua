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
            local input = assert(io.open(file_path, "wb"))

            assert(input:seek("set", k))
            assert(input:write(v))

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
            local k = 1
            return function(_, _)

                assert(input:seek("set", k - 1))
                local next_value = input:read(1)
                if next_value ~= nil then
                    print("*traversing element " .. tostring(k) .. " => " .. next_value)
                end

                --local next_key, next_value = next(t, k)
                --if next_key ~= nil then
                --    print("*traversing element " .. tostring(next_key))
                --end
                --return next_key, next_value
                return k + 1, next_value
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
--local input = assert(io.open(file_path, "wb"))

--local t = input:read("a")
--print(t)
local t = file_as_array(file_path)
print(t[1])
--t[2] = 'u'
print(t[2])
print(t[3])
print(t[4])
print(#t)

for _ in pairs(t) do end

--assert(input:close())
