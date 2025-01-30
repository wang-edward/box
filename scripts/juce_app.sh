#!/bin/bash

OS="$(uname)"

if [[ "$OS" == "Darwin" ]]; then
    echo "not working on mac yet"
    exit
    # BIN_PATH="./build/Box_artefacts/Debug/Box"
elif [[ "$OS" == "Linux" ]]; then
    BIN_PATH="./build/examples/GuiApp/Gui_artefacts/Debug/Gui"
else
    echo "Unsupported OS: $OS"
    exit 1
fi

cmake -DCMAKE_BUILD_TYPE=Debug -B build

if [[ $? == 0 ]]; then
    make -C build -j6
else
    echo "cmake failed"
    exit 1
fi

if [[ $? == 0 ]]; then
    $BIN_PATH
else
    echo "make failed"
    exit 1
fi
