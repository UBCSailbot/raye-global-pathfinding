#!/usr/bin/env bash
set -e

#
# This script takes care of installing all the dependencies required to
#  build the code. Read the comments below for more
#  specific details on what it does.
#
# REQUIREMENTS:
#  - This script only works on Debian-based flavors of Linux, e.g. Ubuntu.
#  - It's assumed that you're starting from a freshly cloned copy of
#     the repository.
#  - A working internet connection is needed to install any missing
# 	  packages as well as the git submodules.
#
#  If you're having problems cloning git repos, check that ssh-agent
#   has your private key loaded. For reference, see e.g.
#   https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/
#

# Clone submodules
git submodule update --init --recursive

# Update package manager indices to latest available.
sudo apt-get update

sudo apt-get install cmake build-essential unzip clang libboost-dev libboost-program-options-dev libglew-dev libglm-dev libeigen3-dev cppcheck xorg-dev libglu1-mesa-dev -y

if [  $(apt-cache show libprotobuf-dev | grep -Po '(?<=Version: )[0-9]') -gt 2 ]; then
    sudo apt install libprotobuf-dev protobuf-compiler -y
else
    sudo add-apt-repository ppa:maarten-fonville/protobuf -y
    sudo apt-get update
    sudo apt-get install protobuf-compiler
fi


INSTALL_DEPS_DIRECTORY=${BASH_SOURCE%/*}

