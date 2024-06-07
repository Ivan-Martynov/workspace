local function rectangle(left, right, bottom, up)
    return function(x, y)
        return (x >= left) and (x <= right) and (y >= bottom) and (y <= up)
    end
end

local function disk(cx, cy, r)
    return function(x, y)
        return (x - cx) ^ 2 + (y - cy) ^ 2 <= r ^ 2
    end
end

local function complement(r)
    return function(x, y)
        return not r(x, y)
    end
end

local function union(r1, r2)
    return function(x, y)
        return r1(x, y) or r2(x, y)
    end
end

local function difference(r1, r2)
    return function(x, y)
        return r1(x, y) and not r2(x, y)
    end
end

local function translate(r, dx, dy)
    return function(x, y)
        return r(x - dx, y - dy)
    end
end

local function rotate(r, a)
    local cos_value = math.cos(a)
    local sin_value = math.sin(a)
    return function(x, y)
        return r(x * cos_value - y * sin_value, y * cos_value + x * sin_value)
    end
end

local function plot(r, m, n)
    io.write("P1\n", m, " ", n, "\n")
    for i = 1, n do
        local y = (n - i * 2) / n
        for j = 1, m do
            local x = (j * 2 - m) / m
            io.write(r(x, y) and "1" or "0")
        end
        io.write("\n")
    end
end

local c1 = disk(0, 0, 1)
local c2 = rectangle(0.1, 0.5, 0.1, 0.5)
io.output("files/test.bmp")
plot(union(rotate(c2, -math.pi * 0.333),
    difference(rotate(c1, math.pi * 0.5), translate(c1, -0.1, 0))), 500, 500)
