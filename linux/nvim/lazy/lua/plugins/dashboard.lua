return {
    "nvimdev/dashboard-nvim",

    dependencies = {
        { "nvim-tree/nvim-web-devicons" },
    },

    event = "VimEnter",

    config = function()
        local status_ok, dashboard = pcall(require, "dashboard")
        if not status_ok then
            return
        end

        local function hyper_setup()
            return {
                theme = "hyper",

                hide = {
                    statusline = true,
                },

                config = {
                    week_header = {
                        enable = true,
                        append = {},
                        concat = "",
                    },

                    shortcut = {
                        {
                            icon = "󰊳 ",
                            desc = "Update",
                            group = "@property",
                            action = "Lazy update",
                            key = "u",
                        },
                        {
                            icon = "󱫥 ",
                            desc = "Recent files",
                            group = "Label",
                            action = "Telescope oldfiles",
                            key = "r",
                        },
{
                            icon = " ",
                            desc = "Sessions",
                            group = "String",
                            action = "Telescope session-lens search_session",
                            key = "s",
                        },
                        {
                            icon = " ",
                            desc = "Config",
                            group = "Conditional",
                            action = "edit ~/.config/nvim/init.lua",
                            key = "c",
                        },
                    },

                    --footer = {}, --your footer
                },
            }
        end

        dashboard.setup(hyper_setup())
    end,
}
