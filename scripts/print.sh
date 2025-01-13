#!/bin/bash

# # Define the directory where you want to search recursively
# DIRECTORY="."

# # Use find to locate all .cc and .hh files recursively
# find "$DIRECTORY" -type f \( -name "*.cc" -o -name "*.hh" \) |
# while IFS= read -r file; do
#     # Print the filename
#     echo "$(basename "$file")"
#     echo '```cpp'
#     # Print the contents of the file
#     cat "$file"
#     echo '```'
# done

# # Loop through all arguments provided to the script
# for file in "$@"; do
#     # Check if the file exists and is readable
#     if [[ -f "$file" ]]; then
#         # Print the filename
#         echo "$(basename "$file")"
#         echo '```cpp'
#         # Print the contents of the file
#         cat "$file"
#         echo '```'
#     else
#         echo "Warning: $file is not a valid file." >&2
#     fi
# done

# Loop through all arguments (can be files or directories)
for path in "$@"; do
    # Check if the argument is a directory
    if [[ -d "$path" ]]; then
        # Find all .cc and .hh files recursively in the directory
        find "$path" -type f \( -name "*.cc" -o -name "*.hh" \) |
        while IFS= read -r file; do
            # Print the filename
            echo "$(basename "$file")"
            echo '```cpp'
            # Print the contents of the file
            cat "$file"
            echo '```'
        done
    elif [[ -f "$path" ]]; then
        # If it's a file, process it directly
        echo "$(basename "$path")"
        echo '```cpp'
        cat "$path"
        echo '```'
    else
        echo "Warning: $path is not a valid file or directory." >&2
    fi
done
