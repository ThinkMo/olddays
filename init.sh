#!/bin/bash

echo "Install Zsh"

sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"\n

echo "Install powerlevel10k"
git clone https://github.com/romkatv/powerlevel10k.git $ZSH_CUSTOM/themes/powerlevel10k

echo "Install bundle"
git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim

curl -fsSL https://raw.githubusercontent.com/ThinkMo/olddays/master/vimrc -o .vimrc

echo "Add ZSH_THEME=\"powerlevel10k/powerlevel10k\" to .zshrc before oh-my-zsh"
echo "vim PluginInstall"
