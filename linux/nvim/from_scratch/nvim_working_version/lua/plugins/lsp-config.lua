return {
    -- Mason.
    {
        "williamboman/mason.nvim",
        config = function()
            require("mason").setup()
        end
    },

    -- Mason lspconfig
    {
        "williamboman/mason-lspconfig.nvim",
        config = function()
            require("mason-lspconfig").setup({
                ensure_installed = { "lua_ls", "clangd" },
            })
        end
    },

    -- Nvim-lspconfig
    {
        "neovim/nvim-lspconfig",
        config = function()
            local lsp_config = require("lspconfig")

            lsp_config.lua_ls.setup({})
            lsp_config.clangd.setup({})

            vim.keymap.set("n", "K", vim.lsp.buf.hover, {})
            vim.keymap.set("n", "<F12>", vim.lsp.buf.definition, {})
            vim.keymap.set({"n", "v"}, "<leader>ca", vim.lsp.buf.code_action, {})
        end
    },
}
