local fn = vim.fn

-- Automatically install packer
local install_path = fn.stdpath "data" .. "/site/pack/packer/start/packer.nvim"
if fn.empty(fn.glob(install_path)) > 0 then
  PACKER_BOOTSTRAP = fn.system {
    "git",
    "clone",
    "--depth",
    "1",
    "https://github.com/wbthomason/packer.nvim",
    install_path,
  }
  print "Installing packer close and reopen Neovim..."
  vim.cmd [[packadd packer.nvim]]
end

-- Autocommand that reloads neovim whenever you save the plugins.lua file
vim.cmd [[
  augroup packer_user_config
    autocmd!
    autocmd BufWritePost plugins.lua source <afile> | PackerSync
  augroup end
]]

-- Use a protected call so we don't error out on first use
local status_ok, packer = pcall(require, "packer")
if not status_ok then
  return
end

-- Have packer use a popup window
packer.init {
  display = {
    open_fn = function()
      return require("packer.util").float { border = "rounded" }
    end,
  },
}

-- Install your plugins here
return packer.startup(function(use)
  -- Plugins here.
  use "wbthomason/packer.nvim" -- Packer manage itself.
  use "nvim-lua/popup.nvim"    -- An implementation of the Popup API from vim in Neovim.
  use "nvim-lua/plenary.nvim"  -- Useful lua functions used by lots of plugins.

  -- Colorscheme.
  -- use "Tsuzat/NeoSolarized.nvim"
  -- use "ishan9299/nvim-solarized-lua"
  use "shaunsingh/solarized.nvim"

  -- Cmp (completion) plugins.
  use "hrsh7th/nvim-cmp"          -- Completion.
  use "hrsh7th/cmp-buffer"        -- buffer completions.
  use "hrsh7th/cmp-path"          -- path completions.
  use "hrsh7th/cmp-cmdline"       -- command line completions.
  use "hrsh7th/cmp-nvim-lsp"      -- lsp.
  use "hrsh7th/cmp-nvim-lua"      -- lua.
  use "saadparwaiz1/cmp_luasnip"  -- snippet completions.

  -- Snippets
  use "L3MON4D3/LuaSnip"             --snippet engine
  use "rafamadriz/friendly-snippets" -- a bunch of snippets to use

  -- LSP
  use "neovim/nvim-lspconfig"           -- enable LSP
  use "williamboman/nvim-lsp-installer" -- simple to use language server installer

  -- Automatically set up your configuration after cloning packer.nvim
  -- Put this at the end after all plugins
  if PACKER_BOOTSTRAP then
    require("packer").sync()
  end
end)

