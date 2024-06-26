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
                ensure_installed = { "lua_ls", "clangd", "cmake" },
            })
        end,
    },

    -- Nvim-lspconfig
    {
        "neovim/nvim-lspconfig",
        config = function()
            local capabilities = require("cmp_nvim_lsp").default_capabilities()

            local lsp_config = require("lspconfig")

            lsp_config.lua_ls.setup({
                capabilities = capabilities,
            })

            lsp_config.clangd.setup({
                capabilities = capabilities,
                cmd = {
                    "clangd",
                    "--offset-encoding=utf-16",
                },
            })

            lsp_config.cmake.setup({
                capabilities = capabilities,
            })

            vim.keymap.set("n", "<leader>e", ":lua vim.diagnostic.open_float(0, { scope = 'line' })<CR>", {})
            vim.keymap.set("n", "K", vim.lsp.buf.hover, {})
            vim.keymap.set("n", "gd", vim.lsp.buf.definition, {})
            vim.keymap.set("n", "<F12>", vim.lsp.buf.definition, {})
            vim.keymap.set("n", "<C-k>", vim.lsp.buf.signature_help, {})
            vim.keymap.set({ "n", "v" }, "<leader>ca", vim.lsp.buf.code_action, {})
        end,
    },
}
