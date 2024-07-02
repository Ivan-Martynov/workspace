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
				autotag = { enable = true },
				rainbow = { enable = true },
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

	-- Rainbow delimiters.
	{
		"HiPhish/rainbow-delimiters.nvim",

		config = function()
			local status_ok, rainbow_delimiters =
                pcall(require, "rainbow-delimiters")

			if not status_ok then
				return
			end

			vim.g.rainbow_delimiters = {
				strategy = {
					[""] = rainbow_delimiters.strategy["global"],
					vim = rainbow_delimiters.strategy["local"],
				},
				query = {
					[""] = "rainbow-delimiters",
					lua = "rainbow-blocks",
				},
				priority = {
					[""] = 110,
					lua = 210,
				},
				highlight = {
					"RainbowDelimiterRed",
					"RainbowDelimiterYellow",
					"RainbowDelimiterBlue",
					"RainbowDelimiterOrange",
					"RainbowDelimiterGreen",
					"RainbowDelimiterViolet",
					"RainbowDelimiterCyan",
				},
			}
		end,
	},
}
