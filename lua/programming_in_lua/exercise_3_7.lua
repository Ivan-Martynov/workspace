
local function random_gaussian()
    return math.cos(math.pi * 2) * math.sqrt(-2 * math.log(math.random()))
end

math.randomseed(os.time())
for _ = 1, 10 do
    print(random_gaussian())
end

