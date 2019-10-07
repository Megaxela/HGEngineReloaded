#!/bin/bash

git ls-files | egrep "(cpp|hpp)" | grep -v "libraries" | xargs clang-format-9 -i 
