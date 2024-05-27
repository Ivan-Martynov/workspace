local begin_contact_counter = 0
local end_contact_counter = 0
local pre_solve_contact_counter = 0
local post_solve_contact_counter = 0

local function print_callback_info(fixture_a, fixture_b, contact, text)
    local name_a = fixture_a:getUserData()
    local name_b = fixture_b:getUserData()
    
    io.write(name_a, name_b)

    local x1, y1, x2, y2 = contact:getPositions()
    if x1 and y1 then
        io.write(" at [" .. string.format("%.2f", x1) .. ", " ..
            string.format("%.2f", y1) .. "] ")
    end

    if x2 and y2 then
        io.write("and [" .. string.format("%.2f", x2) .. ", " ..
            string.format("%.2f", y2) .. "] ")
    end

    print(text)
end

local begin_contact_callback = function (fixture_a, fixture_b, contact)
    --begin_contact_counter = begin_contact_counter + 1
    --print("Begin contact called " .. begin_contact_counter .. " times")
    print_callback_info(fixture_a, fixture_b, contact, "beginning contact.")
end

local end_contact_callback = function (fixture_a, fixture_b, contact)
    print_callback_info(fixture_a, fixture_b, contact, "ending contact.")
end

local pre_solve_contact_callback = function (fixture_a, fixture_b, contact)
    print_callback_info(fixture_a, fixture_b, contact,
        "about to resolve a contact.")
end

local post_solve_contact_callback = function (fixture_a, fixture_b, contact)
    print_callback_info(fixture_a, fixture_b, contact,
        "just resolved a contact.")
end

local world = love.physics.newWorld(0, 9.81 * 9.81)

world:setCallbacks(begin_contact_callback, end_contact_callback,
    pre_solve_contact_callback, post_solve_contact_callback)

return world
