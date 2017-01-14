#!/usr/bin/env bash
set -e

#
# This script takes care of installing all the dependencies required to
#  build the code. Read the comments below for more
#  specific details on what it does.
#
# REQUIREMENTS:
#  - This script only works on macOS.
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

# check that brew is installed
if ! [ -x "$(command -v brew)" ]; then
  echo 'brew is not installed, attempting to install.' >&2
  /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

brew update
brew upgrade
brew install cppcheck
brew install cmake

INSTALL_DEPS_DIRECTORY=${BASH_SOURCE%/*}
LIB_DIRECTORY=${INSTALL_DEPS_DIRECTORY}/../lib

${LIB_DIRECTORY}/protofiles/scripts/install_deps_osx.sh
