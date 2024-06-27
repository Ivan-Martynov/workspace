return {
	{
		"nvim-treesitter/nvim-treesitter",
		build = ":TSUpdate",
		config = function()
			local status_ok, configs = pcall(require, "nvim-treesitter.configs")
			if not status_ok then
				return
			end

			configs.setup({
				ensure_installed = { "c", "cpp", "lua", "vim" },
				-- Automatically installs when a new file type is encountered.
				-- auto_install = true,
				highlight = { enable = true },
				indent = { enable = true },
			})
		end,
	},

	{
		"nvim-treesitter/nvim-treesitter-refactor",

		dependencies = {
			"nvim-treesitter/nvim-treesitter",
		},

		config = function()
			local status_ok, configs = pcall(require, "nvim-treesitter.configs")
			if not status_ok then
				return
			end

			configs.setup({
				refactor = {
					highlight_definitions = {
						enable = true,
						-- Set to false if 'updatetime' is ~100.
						clear_on_cursor_move = false,
					},
				},
			})
		end,
	},
}
