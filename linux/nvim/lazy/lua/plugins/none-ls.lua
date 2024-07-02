return {
	"nvimtools/none-ls.nvim",

	config = function()
		local status_ok, null_ls = pcall(require, "null-ls")
		if not status_ok then
			return
		end

		null_ls.setup({
			sources = {
				null_ls.builtins.formatting.stylua,
				null_ls.builtins.formatting.clang_format,
			},
		})

		vim.keymap.set("n", "<leader>gf", vim.lsp.buf.format, {})
	end,
}
