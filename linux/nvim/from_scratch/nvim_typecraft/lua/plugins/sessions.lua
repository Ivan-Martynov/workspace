return {
    "rmagatti/auto-session",

    config = function()
        local status_ok, auto_session = pcall(require, "auto-session")
        if not status_ok then
            return
        end

        auto_session.setup({
            log_level = "error",
            auto_session_suppress_dirs = { "~/", "~/Downloads", "/" },
            auto_session_enabled = true,
            auto_save_enabled = true,

            -- Telescope stuff.
            session_lens = {
                buftypes_to_ignore = {},
                load_on_setup = true,
                theme_conf = { border = true },
                previewer = false,
            },

            vim.keymap.set("n", "<leader>ls", require("auto-session.session-lens").search_session, {
                noremap = true,
            }),
        })
    end,
}

