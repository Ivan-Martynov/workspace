return {
    "nvim-lualine/lualine.nvim",
    dependencies = { 'nvim-tree/nvim-web-devicons' },
    config = function()
        require("lualine").setup()
        --require("lualine").setup({
            --options = {
            ----    theme = "solarized-light",
            --}
        --})
    end
}