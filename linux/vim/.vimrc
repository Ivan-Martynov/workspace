set nocompatible               " Use Vim settings (not Vi)
set t_Co=256                   " Enable 256 colours
set number                     " Line numbers
set backspace=indent,eol,start " Allow backspace in insert mode
set history=1000               " Lots for :cmdline history
set showcmd                    " Show incomplete cmds down the bottom
set showmode                   " Show current mode down the bottom
set showmatch                  " Show matching words during a search
set gcr=a:blinkon0             " Disable cursor blink
set autoread                   " Reload files changed outside
set mouse=a                    " Mouse

" set UTF-8 encoding
set enc=utf-8
set fenc=utf-8
set termencoding=utf-8

" Reload .vimrc automatically
autocmd! bufwritepost .vimrc source %

filetype on        " File detection
filetype plugin on " Plugins for the detected file type
filetype indent on " Load an indent file for the detected file type
syntax on          " Syntax highlighting

set clipboard=unnamed " allow copy/paste across the system

set modelines=0 " Turn off mode lines

" Set maximum text width
set textwidth=80

"set cursorline " Highlight cursor line
"set cursorcolumn " Highlight cursor column

set shiftwidth=4 " Shift width
set tabstop=4 " Tab width
set softtabstop=4
set shiftround
set expandtab " Use space instead of tab
set tw=80
set colorcolumn=81
set cindent

set nobackup " Do not save backup files

"set scrolloff=10 " Do not let cursor scroll below or above N lines

set incsearch  " Incrementally highlight matching characters while typing
set ignorecase " Ignore capital letters during search
set smartcase  " Search specifically for capital letters
set hlsearch   " Use highlighting when doing a search
" Remove search highlight with leader (probably '\'), followed by space
nnoremap <leader><space> :nohlsearch<CR>

set wildmenu " Enable autocompletion menu
"set wildmode=list:longest " Make similar to bash completion
" Files to ignore with wildmenu
set wildignore=*.docx,*.jpg,*.png,*.gif,*.pdf,*.pyc,*.exe,*.flv,*.img,*.xlsx

set lazyredraw " Redraw lazily

set background=light
colorscheme solarized " Choose colorscheme

