local variables = {
    ___ = "Three underscores.",
    _end = "end prepended by an underscore.",
    End = "End with capital E.",
    -- Reserved word is an illegal identifier.
    -- end
    --until?
    -- nil
    NULL = "NULL is not a reserved word in Lua.",
    -- one-step => Only letters, numbers (after at least one letter or an
    -- underscore) and underscores are allowed.
}

for k, v in pairs(variables) do
    print("Variable: " .. k .. " => " .. v)
end
