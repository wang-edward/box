#!/bin/bash

# Define the directory where you want to search recursively
DIRECTORY="."

# Use find to locate all .cc and .hh files recursively
find "$DIRECTORY" -type f \( -name "*.cc" -o -name "*.hh" \) |
while IFS= read -r file; do
    # Print the filename
    echo "$(basename "$file")"
    echo '```'
    # Print the contents of the file
    cat "$file"
    echo '```'
done
