
local function read_and_sort(first, second)
    if second then
        local f = io.open(second, "r")
        if f then
            local do_overwrite = true
            io.write("File ", second, " exists, overwrite? [Yes/no]: ")
            local answer = string.lower(io.read())
            if not ((answer == "yes") or (answer == "y")) then
                do_overwrite = false
            end
            f:close()
            if not do_overwrite then
                return
            end
        end
    end

    io.input(first or io.stdin)
    io.output(second or io.stdout)
    --local g = io.output(second or io.stdout)
    --if g then
    --    print("file exists")
    --end

    local lines = {}
    for line in io.lines() do
        lines[#lines + 1] = line
    end

    table.sort(lines)

    for _, line in ipairs(lines) do
        io.write(line, "\n")
    end
end

read_and_sort(arg[1], arg[2])
