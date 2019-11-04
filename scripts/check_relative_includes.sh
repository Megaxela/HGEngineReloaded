#!/bin/bash

RELATIVE_INCLUDES=`find . | egrep "(cpp|hpp)" | grep -v "libraries" | xargs egrep -rn "#include\s*\""`

if [[ `echo $RELATIVE_INCLUDES | wc -c` -lt 2 ]]; then
    exit 0
fi
echo "Found relative includes:"
echo $RELATIVE_INCLUDES
exit 1