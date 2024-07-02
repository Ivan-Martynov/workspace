return {
	"mfussenegger/nvim-dap",

	dependencies = {
		"nvim-neotest/nvim-nio",
		"rcarriga/nvim-dap-ui",
	},

	config = function()
		local status_ok, dap = pcall(require, "dap")
        if not status_ok then
            return
        end

		local status_ui_ok, dapui = pcall(require, "dapui")
        if not status_ui_ok then
            return
        end

		require("dapui").setup()

		dap.listeners.before.attach.dapui_config = function()
			dapui.open()
		end
		dap.listeners.before.launch.dapui_config = function()
			dapui.open()
		end
		dap.listeners.before.event_terminated.dapui_config = function()
			dapui.close()
		end
		dap.listeners.before.event_exited.dapui_config = function()
			dapui.close()
		end

		vim.keymap.set("n", "<F9>", dap.toggle_breakpoint, {})
		vim.keymap.set("n", "<F10>", dap.continue, {})
	end,
}
