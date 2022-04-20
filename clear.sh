#!/usr/bin/env bash

BOLD_MAGENTA="\e[1;35m"
BOLD_RED="\e[1;31m"
END="\e[0m"

if find $(dirname $0) -type d -name build -exec rm -rf {} + ; then
    echo -e "${BOLD_MAGENTA}all build directories successfully deleted$END"
else
    echo -e "${BOLD_RED}an error occurred while deleting build directories$END"
fi
