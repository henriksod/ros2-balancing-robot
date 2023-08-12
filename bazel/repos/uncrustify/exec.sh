#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
binary="$SCRIPT_DIR/uncrustify_build/bin/uncrustify"
$binary $@
