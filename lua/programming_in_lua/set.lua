local Set = {}

local mt = {}

function Set.new(list)
	local set = {}
	setmetatable(set, mt)
	for _, v in ipairs(list) do
		set[v] = true
	end
	return set
end

function Set.union(a, b)
	if (getmetatable(a) ~= mt) or (getmetatable(b) ~= mt) then
		error("Attempt to 'add' a set with a non-set value.", 2)
	end

	local result = Set.new({})

	for k in pairs(a) do
		result[k] = true
	end

	for k in pairs(b) do
		result[k] = true
	end

	return result
end

mt.__len = function(set)
    local n = 0
    for _ in pairs(set) do
        n = n + 1
    end
	return n
end

mt.__add = Set.union

mt.__sub = function(a, b)
	if (getmetatable(a) ~= mt) or (getmetatable(b) ~= mt) then
		error("Attempt to 'subtract' a set with a non-set value.", 2)
	end

	local result = Set.new({})

	for k in pairs(a) do
		if b[k] == nil then
			result[k] = true
		end
	end

	return result
end

function Set.intersection(a, b)
	if (getmetatable(a) ~= mt) or (getmetatable(b) ~= mt) then
		error("Attempt to 'multiply' a set with a non-set value.", 2)
	end

	local result = Set.new({})

	for k in pairs(a) do
		result[k] = b[k]
	end

	return result
end

mt.__mul = Set.intersection

function Set.tostring(set)
	local list = {}

	for element in pairs(set) do
		list[#list + 1] = tostring(element)
	end

	return "{" .. table.concat(list, ", ") .. "}"
end

return Set
