
# chip-8 clang implement

### install depends
`sudo apt update && sudo apt install clang libsdl2-dev libsdl2-ttf-dev && sudo apt-get install clang-format` 
### run
`gcc -Iinclude -o hello_sdl src/main.c src/helpers.c src/core.c src/prepare.c -lSDL2 -lSDL2_ttf && ./hello_sdl`
### or use ./run.sh

base project notes
 16 registers of common use
 - 0x000-0x1FF: reserved for interp
 - 0x050-0x0A0: intenral fonts
 - 0x200-0xFFF: program and data
 - videomemory begin from OXF00    