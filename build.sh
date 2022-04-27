#!/usr/bin/env bash

BOLD_MAGENTA="\e[1;35m"
BOLD_RED="\e[1;31m"
END="\e[0m"

mkdir -p $(dirname $0)/build
cd $(dirname $0)/build
pwd
cmake ../src
cmake --build ./
if mv ./bin/main ../main ; then
    echo -e "${BOLD_MAGENTA}main binary successfully moved to the project root directory$END"
else
    echo -e "${BOLD_RED}main binary couldn't be moved to the project root directory: no such file$END"
fi
