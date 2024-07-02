local options = {
    expandtab = true,
    tabstop = 4,
    softtabstop = 4,
    shiftwidth = 4,
    hlsearch = false,   -- Disable highlighting for search.
    colorcolumn = "80", -- Column to wrap at.
    updatetime = 100,   -- Update time (ms) for highlighting variables.
    number = true,      -- Line numbers.
    ignorecase = true,  -- Case insensitive search.
}

for k, v in pairs(options) do
	vim.opt[k] = v
end

