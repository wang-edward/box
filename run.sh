#!/bin/bash

OS="$(uname)"

if [[ "$OS" == "Darwin" ]]; then
    # macOS
    BIN_PATH="./build/Box_artefacts/Box"
elif [[ "$OS" == "Linux" ]]; then
    BIN_PATH="./build/Box_artefacts/Debug/Box"
else
    echo "Unsupported OS: $OS"
    exit 1
fi

cmake -B build
make -C build -j6
$BIN_PATH
