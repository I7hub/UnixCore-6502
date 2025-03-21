#!/bin/bash

# Example usage: 
#
# ./6502Assembler.sh /tmp/UnixCore-6502.s

# Please, ensure that both vasm and jq are installed for the script to operate.

if ! command -v jq > /dev/null || ! command -v vasm > /dev/null; then
    echo "Please, ensure that both vasm and jq are installed for the script to operate." >&2
    exit 1
fi

filePrefix="UnixCore-6502"

jsonFileName="$filePrefix.json"
assemblyFileName="$filePrefix.s"
binaryFileName="$filePrefix.out"

if [ ! -e "$1" ] || [[ ! "$1" =~ UnixCore-6502\.s$ ]]; then

    echo "Warning: the provided path is invalid. The current directory will be used instead." >&2
    echo "Looking for the '$assemblyFileName' in the current directory."

    if ! ls "$assemblyFileName" 1>/dev/null 2>&1; then

        echo "Error: no '$assemblyFileName' file found in the current directory." >&2
        exit 1

    else
    
        filePath="$assemblyFileName"

    fi

else

    filePath="$1"

fi

vasm -Fbin -dotdir "$filePath" -o "$binaryFileName" && hexCode=$(hexdump -v -e '1/1 "0x%02x "' "$binaryFileName" | xargs -n 17) || exit 1

echo "The 'code' array in the file '$jsonFileName' will be cleared."
temporaryJsonData=$(jq '.code = []' ../$jsonFileName)

for ((row = 1; row <= $(echo "$hexCode" | wc -l); row++)); do

    temporaryJsonData=$(echo "$temporaryJsonData" | jq ".code[$(( $row - 1 ))] = \"$(echo "$hexCode" | sed -n "$row p")\"")

done

authorFullName="$(git config --global user.name)"

if [ ! -z "$authorFullName" ]; then

    temporaryJsonData=$(echo "$temporaryJsonData" | jq ".author = \"$authorFullName\"")

else

    echo "Error: you must set your user name for Git. Use 'git config --global user.name "Name Surname"' to configure it." >&2
    exit 1

fi

echo "$temporaryJsonData" | jq > ../$jsonFileName 
echo "Data successfully stored in the '$jsonFileName' json file!"

rm "$binaryFileName"

exit 0