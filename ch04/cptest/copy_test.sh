#!/bin/bash

PROG=$1
SUCCESS_COUNT=0

./gen_test_files.sh

for file in *.img
    do
        echo "FILE: $file"
        ./$1 "$file" "$file.cpy"
        ./verify.sh "$file" "$file.cpy"
        # I want to get return value of verify and get count of success
        if [ $? -eq 0 ]; then
            SUCCESS_COUNT=$((SUCCESS_COUNT+1))
        fi
    done

echo "SUCCESS_COUNT = $SUCCESS_COUNT" 

    
