return {
    -- Telescope.
    {
        'nvim-telescope/telescope.nvim', tag = '0.1.6',

        dependencies = { 'nvim-lua/plenary.nvim' },

        config = function()
            local status_ok, builtin = pcall(require, "telescope.builtin")
            if not status_ok then
                    return
            end

            vim.keymap.set("n", "<leader>ff", builtin.find_files, {})
            vim.keymap.set("n", "<leader>fg", builtin.live_grep, {})
            vim.keymap.set("n", "<leader>fb", builtin.buffers, {})
        end
    },

    -- Telescope-ui-select
    {
        "nvim-telescope/telescope-ui-select.nvim",
        config = function()
            require("telescope").setup({
                extensions = {
                    ["ui-select"] = {
                        require("telescope.themes").get_dropdown{}
                    }
                }
            })
            require("telescope").load_extension("ui-select")
        end
    },
}
