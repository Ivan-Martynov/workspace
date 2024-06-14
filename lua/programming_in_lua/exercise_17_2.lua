local geom = require("geom")

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

local c1 = geom.disk(0, 0, 1)
local c2 = geom.rectangle(0.1, 0.5, 0.1, 0.5)
io.output("files/test.bmp")
plot(geom.union(geom.rotate(c2, -math.pi * 0.333),
    geom.difference(geom.rotate(c1, math.pi * 0.5),
    geom.translate(c1, -0.1, 0))), 500, 500)
