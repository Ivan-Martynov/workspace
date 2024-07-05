local grid_x_count
local grid_y_count
local block_size

local inert

local colors

local piece_x
local piece_y
local piece_x_count
local piece_y_count
local piece_type
local piece_rotation
local piece_structures

local sequence

local fall_timer
local fall_timer_limit = 0.5

local function new_sequence()
    sequence = {}
    for i = 1, #piece_structures do
        local pos = love.math.random(#sequence + 1)
        table.insert(sequence, pos, i)
    end
end

local function new_piece()
    piece_x = 3
    piece_y = 0
    piece_rotation = 1
    piece_type = table.remove(sequence)

    if #sequence == 0 then
        new_sequence()
    end
end

local function init_pieces()
    piece_structures = {
        {
            {
                { ' ', ' ', ' ', ' ' },
                { 'i', 'i', 'i', 'i' },
                { ' ', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 'i', ' ', ' ' },
                { ' ', 'i', ' ', ' ' },
                { ' ', 'i', ' ', ' ' },
                { ' ', 'i', ' ', ' ' },
            },
        },
        {
            {
                { ' ', ' ', ' ', ' ' },
                { ' ', 'o', 'o', ' ' },
                { ' ', 'o', 'o', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
        },
        {
            {
                { ' ', ' ', ' ', ' ' },
                { 'j', 'j', 'j', ' ' },
                { ' ', ' ', 'j', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 'j', ' ', ' ' },
                { ' ', 'j', ' ', ' ' },
                { 'j', 'j', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { 'j', ' ', ' ', ' ' },
                { 'j', 'j', 'j', ' ' },
                { ' ', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 'j', 'j', ' ' },
                { ' ', 'j', ' ', ' ' },
                { ' ', 'j', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
        },
        {
            {
                { ' ', ' ', ' ', ' ' },
                { 'l', 'l', 'l', ' ' },
                { 'l', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 'l', ' ', ' ' },
                { ' ', 'l', ' ', ' ' },
                { ' ', 'l', 'l', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', ' ', 'l', ' ' },
                { 'l', 'l', 'l', ' ' },
                { ' ', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { 'l', 'l', ' ', ' ' },
                { ' ', 'l', ' ', ' ' },
                { ' ', 'l', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
        },
        {
            {
                { ' ', ' ', ' ', ' ' },
                { 't', 't', 't', ' ' },
                { ' ', 't', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 't', ' ', ' ' },
                { ' ', 't', 't', ' ' },
                { ' ', 't', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 't', ' ', ' ' },
                { 't', 't', 't', ' ' },
                { ' ', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 't', ' ', ' ' },
                { 't', 't', ' ', ' ' },
                { ' ', 't', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
        },
        {
            {
                { ' ', ' ', ' ', ' ' },
                { ' ', 's', 's', ' ' },
                { 's', 's', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { 's', ' ', ' ', ' ' },
                { 's', 's', ' ', ' ' },
                { ' ', 's', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
        },
        {
            {
                { ' ', ' ', ' ', ' ' },
                { 'z', 'z', ' ', ' ' },
                { ' ', 'z', 'z', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
            {
                { ' ', 'z', ' ', ' ' },
                { 'z', 'z', ' ', ' ' },
                { 'z', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ' },
            },
        },
    }
end

local function reset()
    inert = {}

    for y = 1, grid_y_count do
        inert[y] = {}
        for x = 1, grid_x_count do
            inert[y][x] = " "
        end
    end

    new_sequence()
    new_piece()
    fall_timer = 0
end

function love.load()
    grid_x_count = 10
    grid_y_count = 18
    block_size = 20

    colors = {
        [' '] = { 0.87, 0.87, 0.87 },
        i = { 0.47, 0.76, 0.94 },
        j = { 0.93, 0.91, 0.42 },
        l = { 0.49, 0.85, 0.76 },
        o = { 0.92, 0.69, 0.47 },
        s = { 0.83, 0.54, 0.93 },
        t = { 0.97, 0.58, 0.77 },
        z = { 0.66, 0.83, 0.46 },
        preview = { 0.75, 0.75, 0.75 },
    }

    init_pieces()

    piece_x_count = 4
    piece_y_count = 4

    reset()
end

local function can_move_piece(test_x, test_y, test_rotation)
    for y = 1, piece_y_count do
        for x = 1, piece_x_count do
            local block_x = test_x + x
            local block_y = test_y + y
            if (piece_structures[piece_type][test_rotation][y][x] ~= " ")
                and ((block_x < 1) or (block_x > grid_x_count)
                    or (block_y > grid_y_count)
                    or (inert[block_y][block_x] ~= " ")) then
                return false
            end
        end
    end
    return true
end

function love.keypressed(key)
    if key == "up" then
        local test_rotation = piece_rotation + 1
        if test_rotation > #piece_structures[piece_type] then
            test_rotation = 1
        end

        if can_move_piece(piece_x, piece_y, test_rotation) then
            piece_rotation = test_rotation
        end
    elseif key == "down" then
        local test_rotation = piece_rotation - 1
        if test_rotation < 1 then
            test_rotation = #piece_structures[piece_type]
        end

        if can_move_piece(piece_x, piece_y, test_rotation) then
            piece_rotation = test_rotation
        end
    elseif key == "left" then
        local test_x = piece_x - 1
        if can_move_piece(test_x, piece_y, piece_rotation) then
            piece_x = test_x
        end
    elseif key == "right" then
        local test_x = piece_x + 1
        if can_move_piece(test_x, piece_y, piece_rotation) then
            piece_x = test_x
        end
    elseif key == "return" then
        if can_move_piece(piece_x, piece_y + 1, piece_rotation) then
            piece_y = piece_y + 1
            fall_timer = fall_timer_limit
        end
    elseif key == "escape" then
        love.event.quit()
    end
end

function love.update(dt)
    fall_timer = fall_timer + dt
    if fall_timer >= fall_timer_limit then
        fall_timer = fall_timer - 0.5
        local test_y = piece_y + 1
        if can_move_piece(piece_x, test_y, piece_rotation) then
            piece_y = test_y
        else
            for y = 1, piece_y_count do
                for x = 1, piece_x_count do
                    local block =
                        piece_structures[piece_type][piece_rotation][y][x]
                    if block ~= " " then
                        inert[piece_y + y][piece_x + x] = block
                    end
                end
            end

            for y = 1, grid_y_count do
                local complete_row = true
                for x = 1, grid_x_count do
                    if inert[y][x] == " " then
                        complete_row = false
                        break;
                    end
                end
                if complete_row then
                    for remove_y = y, 2, -1 do
                        for remove_x = 1, grid_x_count do
                            inert[remove_y][remove_x] =
                                inert[remove_y - 1][remove_x]
                        end
                    end

                    for remove_x = 1, grid_x_count do
                        inert[1][remove_x] = " "
                    end
                end
            end


            new_piece()
            if not can_move_piece(piece_x, piece_y, piece_rotation) then
                reset()
            end
        end
    end
end

function love.draw()
    local shift_x = 2
    local shift_y = 5
    local function draw_block(block, x, y)
        love.graphics.setColor(colors[block])
        love.graphics.rectangle("fill", (x - 1) * block_size,
            (y - 1) * block_size, block_size - 1, block_size - 1)
    end
    for y = 1, grid_y_count do
        for x = 1, grid_x_count do
            draw_block(inert[y][x], x + shift_x, y + shift_y)
        end
    end

    for y = 1, piece_y_count do
        for x = 1, piece_x_count do
            local block = piece_structures[piece_type][piece_rotation][y][x]
            if block ~= " " then
                draw_block(block, x + piece_x + shift_x, y + piece_y + shift_y)
            end
        end
    end

    for y = 1, piece_y_count do
        for x = 1, piece_x_count do
            local block = piece_structures[sequence[#sequence]][1][y][x]
            if block ~= " " then
                draw_block("preview", x + 5, y + 1)
            end
        end
    end
end
