return {
    -- Lualine.
    {
        "nvim-lualine/lualine.nvim",
        dependencies = { "nvim-tree/nvim-web-devicons" },
        config = function()
            local status_ok, lualine = pcall(require, "lualine")
            if not status_ok then
                return
            end

            lualine.setup({
                options = {
                    theme = "NeoSolarized",
                },
            })
        end,
    },

    -- Bufferline.
    {
        "akinsho/bufferline.nvim",
        version = "*",
        dependencies = "nvim-tree/nvim-web-devicons",

        config = function()
            local status_ok, bufferline = pcall(require, "bufferline")
            if not status_ok then
                return
            end

            bufferline.setup({})

            vim.keymap.set("n", "<C-PageDown>", ":BufferLineCycleNext<CR>", {})
            vim.keymap.set("n", "<C-tab>", ":BufferLineCycleNext<CR>", {})

            vim.keymap.set("n", "<C-PageUp>", ":BufferLineCyclePrev<CR>", {})
            vim.keymap.set("n", "<C-S-tab>", ":BufferLineCyclePrev<CR>", {})
        end,
    },
}
