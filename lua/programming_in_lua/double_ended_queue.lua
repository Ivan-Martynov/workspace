local M = {}

function M.init_queue()
    return { first = 0, last = 0 }
end

function M.push_first(queue, value)
    queue.first = queue.first - 1
    queue[queue.first] = value
end

function M.push_last(queue, value)
    queue[queue.last] = value
    queue.last = queue.last + 1
end

function M.is_empty(queue)
    return queue.first == queue.last
end

function M.pop_first(queue)
    if M.is_empty(queue) then
        error("The queue is empty.")
    end

    local value = queue[queue.first]

    queue[queue.first] = nil
    queue.first = queue.first + 1

    if M.is_empty(queue) then
        queue.first, queue.last = 0, 0
    end

    return value
end

function M.pop_last(queue)
    if M.is_empty(queue) then
        error("The queue is empty.")
    end

    queue.last = queue.last - 1
    local value = queue[queue.last]

    queue[queue.last] = nil

    if M.is_empty(queue) then
        queue.first, queue.last = 0, 0
    end

    return value
end

return M
