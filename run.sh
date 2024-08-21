#!/bin/bash
cmake -B build
make -C build -j6
./build/Box_artefacts/Debug/Box
