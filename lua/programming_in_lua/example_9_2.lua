local function disk(cx, cy, r)
    return function(x, y)
        return (x - cx) ^ 2 + (y - cy) ^ 2 <= r ^ 2
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
io.output("files/test.bmp")
plot(difference(c1, translate(c1, 0.3, 0)), 500, 500)
