#!/usr/bin/env bash

SCRIPTS_DIRECTORY=${BASH_SOURCE%/*}
SRC_DIRECTORY=${SCRIPTS_DIRECTORY}/../src

cppcheck ${SRC_DIRECTORY} --enable=all
