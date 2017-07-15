"global settings"

filetype on
filetype plugin on " 根据侦测到的不同类型加载对应的插件
filetype indent on " 自动缩进

syntax enable " 开启语法高亮功能
syntax on " 允许用指定语法高亮配色方案替换默认方案

set incsearch "开启实时搜索功能
set ignorecase "搜索忽略大小写
set wildmenu "vim命令自动补全
set autoread "文件自动更新
set gcr=a:block-blinkon0 "禁止关闭闪烁
set ruler "显示光标位置
set number "显示行号
set hlsearch "高亮显示搜索结果
set backspace=2 "回退键生效
set novisualbell

" 禁止显示滚动条
set guioptions-=l
set guioptions-=L
set guioptions-=r
set guioptions-=R

" 禁止显示菜单和工具条
set guioptions-=m
set guioptions-=T

" 将制表符扩展为空格
set expandtab
" 设置编辑时制表符占用空格数
set tabstop=4
" 设置格式化时制表符占用空格数
set shiftwidth=4
" 让 vim 把连续数量的空格视为一个制表符
set softtabstop=4
" 基于缩进或语法进行代码折叠
set foldmethod=syntax
" 启动 vim 时关闭折叠代码
set nofoldenable

" window shortcut
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>

" vundle
let mapleader="," "change the leader to , vs splash
filetype off
set rtp+=~/.vim/bundle/Vundle.vim
" vundle 管理的插件列表必须位于 vundle#begin() 和 vundle#end() 之间
call vundle#begin()
Plugin 'tpope/vim-fugitive'  "git
Plugin 'VundleVim/Vundle.vim'
Plugin 'octol/vim-cpp-enhanced-highlight' "对c++语法高亮增强
Plugin 'majutsushi/tagbar' "taglist的增强版，查看标签，依赖于ctags
Plugin 'scrooloose/nerdtree' "文件浏览
Plugin 'vim-scripts/Solarized.git' "主题方案
Plugin 'klen/python-mode'
Plugin 'davidhalter/jedi-vim'  "python autocompeletion
Plugin 'Lokaltog/powerline', {'rtp': 'powerline/bindings/vim/'} "status
Plugin 'https://github.com/fatih/vim-go.git'  "golang 
Plugin 'nvie/vim-flake8' " flake8
Plugin 'ekalinin/Dockerfile.vim' "dockerfile
Plugin 'Valloric/YouCompleteMe'
call vundle#end()
filetype on

" Powerline 设置
" 设置状态栏主题风格
let g:Powerline_colorscheme='solarized256'

""" color settings
set t_Co=256
if has("gui_running")
    set guioptions=gR
    set mousemodel=popup
    set background=light
    ""hi LineNr cterm=bold guibg=black guifg=white
    ""hi CursorLine cterm=none ctermbg=lightgray ctermfg=none
    ""hi CursorColumn cterm=none ctermbg=lightgray ctermfg=none
else
    set background=dark
    ""hi LineNr cterm=bold ctermbg=black ctermfg=white
    ""hi CursorLine cterm=none ctermbg=darkgray ctermfg=none
    ""hi CursorColumn cterm=none ctermbg=darkgray ctermfg=none
endif
colorscheme solarized

" Plugin nerdtree settings
let NERDTreeWinPos='right'
let NERDTreeWinSize=30
map <F2> :NERDTreeToggle<CR>
" Plugin tagbar settings
nmap <Leader>tb :TagbarToggle<CR>      "快捷键设置
let g:tagbar_ctags_bin='ctags'          "ctags程序的路径
let g:tagbar_width=30                   "窗口宽度的设置
map <F3> :Tagbar<CR>
" YouCompleteMe
let g:ycm_autoclose_preview_window_after_completion=1
map <leader>g  :YcmCompleter GoToDefinitionElseDeclaration<CR>
"python with virtualenv support
py << EOF
import os.path
import sys
import vim
if 'VIRTUALENV' in os.environ:
    project_base_dir = os.environ['VIRTUAL_ENV']
    sys.path.insert(0, project_base_dir)
    activate_this = os.path.join(project_base_dir, 'bin/activate_this.py')
    execfile(activate_this, dict(__file__=activate_this))
EOF

" python 
let python_highlight_all=1
au FileType python syn keyword pythonDecorator True None False self

" Go config
au FileType go setlocal tabstop=4 shiftwidth=4 softtabstop=4 noexpandtab
