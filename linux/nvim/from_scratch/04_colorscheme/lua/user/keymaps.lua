-- Options.
local opts = { noremap = true, silent = true }

local term_opts = { silent = true }

-- Shorthand name.
local keymap = vim.api.nvim_set_keymap

-- Remap the leader key.
-- keymap("", "<Space>", "<Nop>", opts)
-- vim.g.mapleader = " "
-- vim.g.maplocalleader = " "

-- Modes:
-- "n" => normal_mode
-- "i" => insert_mode
-- "v" => visual_mode
-- "x" => visual_block_mode
-- "t" => term_mode
-- "c" => command_mode

-- Calling Lexplore with given size.
keymap("n", "<leader>e", ":Lex 50<CR>", opts)

-- Window navigation.
keymap("n", "<C-h>", "<C-w>h", opts)
keymap("n", "<C-j>", "<C-w>j", opts)
keymap("n", "<C-k>", "<C-w>k", opts)
keymap("n", "<C-l>", "<C-w>l", opts)

-- Window resizing.
keymap("n", "<C-Up>", ":resize -2<CR>", opts)
keymap("n", "<C-Down>", ":resize +2<CR>", opts)
keymap("n", "<C-Left>", ":vertical resize -2<CR>", opts)
keymap("n", "<C-Right>", ":vertical resize +2<CR>", opts)

-- Buffer navigation using the Shift modifier key.
keymap("n", "<S-l>", ":bnext<CR>", opts)
keymap("n", "<S-h>", ":bprevious<CR>", opts)

-- Pressing jk quickly leaves the insert mode.
--keymap("i", "jk", "<ESC>", opts)

-- Stay visual in indent mode.
--keymap("v", "<", "<gv", opts)
--keymap("v", ">", ">gv", opts)

-- Moving text up and down.
--keymap("v", "<A-j>", ":m .+1<CR>==", opts)
--keymap("v", "<A-k>", ":m .-2<CR>==", opts)
--keymap("v", "p", '"_dP', opts)

-- Moving visual block.
--keymap("x", "j", ":move '>+1<CR>gv-gv", opts)
--keymap("x", "k", ":move '<-2<CR>gv-gv", opts)
--keymap("x", "<A-j>", ":move '>+1<CR>gv-gv", opts)
--keymap("x", "<A-k>", ":move '<-2<CR>gv-gv", opts)

-- Terminal navigation
--keymap("t", "<C-h>", "<C-\\><C-N><C-w>h", term_opts)
--keymap("t", "<C-j>", "<C-\\><C-N><C-w>j", term_opts)
--keymap("t", "<C-k>", "<C-\\><C-N><C-w>k", term_opts)
--keymap("t", "<C-l>", "<C-\\><C-N><C-w>l", term_opts)

