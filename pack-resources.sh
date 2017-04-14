#!/bin/sh

PATH_ROOT="."

if [ -n "$1" ]; then
    PATH_ROOT="$1"
fi

$PATH_ROOT/sciter-sdk/bin.gtk/packfolder $PATH_ROOT/src/res $PATH_ROOT/src/resources.h -v "resources"