#!/bin/bash
cmake -B build
make -C build -j
./build/Box_artefacts/Box
