return {
    "nvim-treesitter/nvim-treesitter",
    build = ":TSUpdate",
    config = function()
        local status_ok, configs = pcall(require, "nvim-treesitter.configs")
        if not status_ok then
            return
        end

        configs.setup({
            --ensure_installed = { "c", "lua", "vim" },
            -- Automatically installs when a new file type is encountered.
            auto_install = true,
            highlight = { enable = true },
            indent = { enable = true },
        })
    end,
}