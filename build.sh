#!/bin/bash

libs=-luser32
warnings="-Wno-writable-strings -Wno-format-security"

clang++ -g src/main.cpp -oplungs.exe $libs $warnings