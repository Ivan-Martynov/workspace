local double_ended_queue = require("double_ended_queue")

local queue = double_ended_queue.init_queue()
double_ended_queue.push_first(queue, 1)
double_ended_queue.push_first(queue, 4)
double_ended_queue.push_first(queue, 6)
double_ended_queue.push_last(queue, 8)

print(double_ended_queue.pop_last(queue))
print(double_ended_queue.pop_last(queue))
print(double_ended_queue.pop_first(queue))
print(double_ended_queue.pop_first(queue))
print(queue.first .. " " .. queue.last)
