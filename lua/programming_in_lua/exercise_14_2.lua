local function init_queue()
    return { first = 0, last = 0 }
end

local function push_first(queue, value)
    queue.first = queue.first - 1
    queue[queue.first] = value
end

local function push_last(queue, value)
    queue[queue.last] = value
    queue.last = queue.last + 1
end

local function is_empty(queue)
    return queue.first == queue.last
end

local function pop_first(queue)
    if is_empty(queue) then
        error("The queue is empty.")
    end

    local value = queue[queue.first]

    queue[queue.first] = nil
    queue.first = queue.first + 1

    if is_empty(queue) then
        queue.first, queue.last = 0, 0
    end

    return value
end

local function pop_last(queue)
    if is_empty(queue) then
        error("The queue is empty.")
    end

    queue.last = queue.last - 1
    local value = queue[queue.last]

    queue[queue.last] = nil

    if is_empty(queue) then
        queue.first, queue.last = 0, 0
    end

    return value
end

local queue = init_queue()
push_first(queue, 1)
push_first(queue, 4)
push_first(queue, 6)
push_last(queue, 8)

print(pop_last(queue))
print(pop_last(queue))
print(pop_first(queue))
print(pop_first(queue))
print(queue.first .. " " .. queue.last)
