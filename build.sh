#!/usr/bin/env zsh

cmake -S . -B build/ -D_SDL_TTF_H=TRUE
cmake --build build/