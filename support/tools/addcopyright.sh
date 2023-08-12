#!/bin/bash

# Copyright (c) 2023, Henrik Söderlund

## Solution found as part of this stackoverflow discussion: https://stackoverflow.com/questions/151677/tool-for-adding-license-headers-to-source-files

set -o pipefail

for x in $*; do

has_copyright="$(grep $x -e '[C|c]opyright')"

if [[ -n $has_copyright ]]; then
    continue
fi

COPYRIGHT_CONTENT="$(cat copyright.txt)"

if [[ $x =~ \.(c|cpp|h|hpp)(\.(template|in))?$ ]]; then
    COPYRIGHT_CONTENT="// $COPYRIGHT_CONTENT"
elif [[ $x =~ \.(py|bazel|bzl|yaml|yml|sh)(\.(template|in))?$ ]]; then
    COPYRIGHT_CONTENT="# $COPYRIGHT_CONTENT"
else
    continue
fi

has_shebang=$(head -1 $x | cut -f1 -d '/' | diff <(echo "#!") -)

if [[ -n $has_shebang ]]; then
    head -1 $x | diff <(echo "$COPYRIGHT_CONTENT") - || ( ( echo "$COPYRIGHT_CONTENT"; echo; cat $x) > /tmp/file;
    mv /tmp/file $x )
else
    sed -n '3p' $x | diff <(echo "$COPYRIGHT_CONTENT") - || ( ( head -1 $x; echo; echo "$COPYRIGHT_CONTENT"; echo; tail -n +2 $x) > /tmp/file;
    chmod --reference=$x /tmp/file; mv /tmp/file $x )
fi

done
