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

ubuntu_version=`lsb_release -rs | sed 's/\.//'`

# Clone submodules
git submodule update --init --recursive

# Update package manager indices to latest available.
sudo apt-get update

# Get the required packages for build
sudo apt-get install curl build-essential clang libboost-dev libboost-program-options-dev libglew-dev libglm-dev libeigen3-dev cppcheck xorg-dev libglu1-mesa-dev cmake -y

if [[ $ubuntu_version > 1610 ]]; then
    sudo apt-get -y install libeccodes-dev libeccodes-tools
else
    wget "https://confluence.ecmwf.int/download/attachments/45757960/eccodes-2.10.0-Source.tar.gz"
    tar -xzf eccodes-2.10.0-Source.tar.gz
    mkdir build2100
    cd build2100
    cmake -DENABLE_FORTRAN=OFF ../eccodes-2.10.0-Source
    make -j 4
    ctest
    make install
    cd ..
fi


INSTALL_DEPS_DIRECTORY=${BASH_SOURCE%/*}
LIB_DIRECTORY=${INSTALL_DEPS_DIRECTORY}/../lib

${LIB_DIRECTORY}/protofiles/scripts/install_deps_debian.sh
