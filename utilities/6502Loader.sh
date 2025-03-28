#!/bin/bash

# Example usage: 
#
# ./6502Loader.sh /tmp/UnixCore-6502.s

jsonFileName="UnixCore-6502.json"

if [ ! -s ../$jsonFileName ]; then

    echo "Error: the $jsonFileName file is either empty or does not exist in the repository directory." >&2
    exit 1

fi

./6502Assembler.sh "$1" && ./6502Parser.sh || exit 1
