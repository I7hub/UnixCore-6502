#!/bin/bash

# Please, ensure jq is installed for the script to operate.

if ! command -v jq > /dev/null; then
    echo "Please, ensure jq is installed for the script to operate." >&2
    exit 1
fi

libraryFilePath="libraries/rommanager/rommanager"
jsonFileName="UnixCore-6502.json"
maxSIZE=16384
modifiedFile=$(git diff --name-only ../${jsonFileName})

if ! jq empty ../$jsonFileName 2> /dev/null; then

    echo "Error: The JSON file $jsonFileName is invalid. Please check the structure and try again!" >&2
    exit 1

fi

if [ -z "$modifiedFile" ]; then

    echo "The $jsonFileName file has not been modified!" >&2
    exit 1

else

    if invalidEntries=$(jq -r '.code[]' ../$jsonFileName | grep -Pv '^(0x[0-9a-fA-F]{2}( 0x[0-9a-fA-F]{2}){0,16})$'); then

        echo "Invalid lines detected in ${jsonFileName}. These lines need to be reviewed and fixed:" >&2
        echo "$invalidEntries" >&2
        exit 1

    else

        SIZE=$(jq -r '.code | join(" ")' ../$jsonFileName | wc -w)
    
        if [ $SIZE -le $maxSIZE ]; then
        
            arrayElements=$(jq -r '.code | join(" ") | split(" ") | join(", ") | "{" + . + "}"' ../$jsonFileName)
            sed -i "s/byte lowrom\[16384\] = {[^}]*};/byte lowrom[16384] = ${arrayElements};/" ../${libraryFilePath}.cpp
            sed -i "s/#define SIZE\s\+[0-9]\+/#define SIZE ${SIZE}/" ../${libraryFilePath}.h
            echo "Done."
            exit 0
    
        else

            echo "You have exceeded the allowed size!" >&2
            exit 1

        fi
    
    fi

fi 