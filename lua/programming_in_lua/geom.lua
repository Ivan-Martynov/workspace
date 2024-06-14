local M = {}

function M.rectangle(left, right, bottom, up)
    return function(x, y)
        return (x >= left) and (x <= right) and (y >= bottom) and (y <= up)
    end
end

function M.disk(cx, cy, r)
    return function(x, y)
        return (x - cx) ^ 2 + (y - cy) ^ 2 <= r ^ 2
    end
end

function M.complement(r)
    return function(x, y)
        return not r(x, y)
    end
end

function M.union(r1, r2)
    return function(x, y)
        return r1(x, y) or r2(x, y)
    end
end

function M.difference(r1, r2)
    return function(x, y)
        return r1(x, y) and not r2(x, y)
    end
end

function M.translate(r, dx, dy)
    return function(x, y)
        return r(x - dx, y - dy)
    end
end

function M.rotate(r, a)
    local cos_value = math.cos(a)
    local sin_value = math.sin(a)
    return function(x, y)
        return r(x * cos_value - y * sin_value, y * cos_value + x * sin_value)
    end
end

return M
