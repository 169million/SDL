#!/bin/bash
g++ main.cpp dialogue.cpp Player.cpp -o my_game $(pkg-config --cflags --libs sdl3 sdl3-image sdl3-ttf) && ./my_game