local set = require("set")

local s1 = set.new({ 10, 20, 30, 50 })
local s2 = set.new({ 30, 1 })

print(getmetatable(s1))
print(getmetatable(s2))

local s3 = s1 + s2
print(set.tostring(s3))
print(set.tostring((s1 + s2) * s1))
print(set.tostring(s1 - s1))
print(set.tostring(s1 - s2))
print(set.tostring(s3 - s1))
print(#s3)
print(#(s1 - s1))
