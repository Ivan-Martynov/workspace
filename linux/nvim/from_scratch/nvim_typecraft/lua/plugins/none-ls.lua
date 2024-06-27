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

			--on_attach = function(client)
				-- if client.resolved_capabilities.document_formatting then
					-- vim.cmd("autocmd BufWritePre <buffer> lua vim.lsp.buf.formatting_sync()")
				-- end

                -- vim.cmd([[
                --     augroup document_highlight
                --     autocmd! * <buffer>
                --     autocmd CursorHold <buffer> lua vim.lsp.buf.document_highlight()
                --     autocmd CursorMoved <buffer> lua vim.lsp.buf.clear_references()
                --     augroup END
                -- ]])
			--end,
		})

		vim.keymap.set("n", "<leader>gf", vim.lsp.buf.format, {})
	end,
}
