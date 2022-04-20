#!/usr/bin/env bash

BOLD_RED="\e[1;31m"
BOLD_MAGENTA="\e[1;35m"
END="\e[0m"

if [ $# -eq 0 ]; then
    echo -e "${BOLD_RED}filename expected as argument$END"
    exit 1
fi

folder=$(dirname ${1})
filename=$(basename ${1})
extension="${filename##*.}"
filename="${filename%.*}"

cd $(dirname $0)
mkdir -p build
if pdflatex -output-directory=build ${folder}/${filename}.${extension} && \
   pdflatex -output-directory=build ${folder}/${filename}.${extension} && \
   mv build/${filename}.pdf ${filename}.pdf; then
    echo -e "${BOLD_MAGENTA}pdf report successfully moved to the project root directory$END"
else
    echo -e "${BOLD_RED}pdf report couldn't be moved to the project root directory: no such file$END"
fi
