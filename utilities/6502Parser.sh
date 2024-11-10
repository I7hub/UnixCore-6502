#!/bin/bash

# Please, ensure jq is installed for the script to operate.

if ! command -v jq > /dev/null; then
    echo "Please, ensure jq is installed for the script to operate." >&2
    exit 1
fi

filePath="libraries/rommanager/rommanager"
fileName="UnixCore-6502.json"
maxSIZE=16384
modifiedFile=$(git diff --name-only ../${fileName})

if [ -z "$modifiedFile" ]; then

    echo "The $fileName file has not been modified!" >&2
    exit 1

else

    SIZE=$(jq -r '.code | join(" ")' ../$fileName | wc -w)
    
    if [ $SIZE -le $maxSIZE ]; then
        
        arrayElements=$(jq -r '.code | join(" ") | split(" ") | join(", ") | "{" + . + "}"' ../$fileName)
        sed -i "s/byte lowrom\[16384\] = {[^}]*};/byte lowrom[16384] = ${arrayElements};/" ../${filePath}.cpp
        sed -i "s/#define SIZE\s\+[0-9]\+/#define SIZE ${SIZE}/" ../${filePath}.h
        echo "Done."
        exit 0
    
    else

        echo "You have exceeded the allowed size!" >&2
        exit 1

    fi

fi 