#!/bin/sh

PATH_ROOT="."

if [ -n "$2" ]; then
    PATH_ROOT="$2"
fi

$PATH_ROOT/sciter-sdk/bin.$1/packfolder $PATH_ROOT/src/res $PATH_ROOT/src/resources.h -v "resources"
