return {
	-- Mason.
	{
		"williamboman/mason.nvim",
		config = function()
			require("mason").setup()
		end,
	},

	-- Mason lspconfig
	{
		"williamboman/mason-lspconfig.nvim",
		config = function()
			require("mason-lspconfig").setup({
				ensure_installed = {
					"lua_ls",
					"clangd",
					"cmake",
					--   "vimls",
				},
			})
		end,
	},

	-- Nvim-lspconfig
	{
		"neovim/nvim-lspconfig",
		config = function()
			local cmp_status_ok, cmp_lsp = pcall(require, "cmp_nvim_lsp")
			if not cmp_status_ok then
				return
			end

			local lsp_status_ok, lsp_config = pcall(require, "lspconfig")
			if not lsp_status_ok then
				return
			end

			local capabilities = cmp_lsp.default_capabilities()

			--[[ lsp_config.vimls.setup({
                capabilities = capabilities,
            }) ]]

			lsp_config.lua_ls.setup({
				capabilities = capabilities,
			})

			lsp_config.clangd.setup({
				capabilities = capabilities,
				cmd = {
					"clangd",
					"--offset-encoding=utf-16",
					"-style=file:<~/workspace/cpp/.clang_format",
				},
			})

			lsp_config.cmake.setup({
				capabilities = capabilities,
			})

			vim.keymap.set(
				"n",
				"<leader>e",
				":lua vim.diagnostic.open_float(0, { scope = 'line' })<CR>",
				{}
			)
			vim.keymap.set("n", "K", vim.lsp.buf.hover, {})
			vim.keymap.set("n", "<F2>", vim.lsp.buf.rename, {})
			vim.keymap.set("n", "<F12>", vim.lsp.buf.definition, {})
			vim.keymap.set("n", "<leader><F12>", vim.lsp.buf.references, {})
			vim.keymap.set("n", "<C-k>", vim.lsp.buf.signature_help, {})
			vim.keymap.set(
				{ "n", "v" },
				"<leader>ca",
				vim.lsp.buf.code_action,
				{}
			)
		end,
	},
}
