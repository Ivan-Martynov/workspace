local options = {
    expandtab = true,
    tabstop = 4,
    softtabstop = 4,
    shiftwidth = 4,
    hlsearch = false,
    colorcolumn = "80",
}

for k, v in pairs(options) do
	vim.opt[k] = v
end

