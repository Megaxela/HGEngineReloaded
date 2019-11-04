#!/bin/bash

# Running clang-format actually
git ls-files | grep -E "(cpp|hpp)" | grep -v "libraries" | xargs clang-format-9 -i

# Adding newlines if there was no anywhere
FILES=$(git ls-files | grep -E "(\.cpp|\.hpp|\.txt|\.cmake)" | grep -v -E "(/|\\\)tests(/|\\\)static(/|\\\)" | grep -v "libraries")
for line in $FILES; do
    if [[ -n "$(tail -c1 "$line")" ]]; then
        echo -e "        \e[33mNo newline\e[0m at $line"
        echo >> "$line";
    fi
done
