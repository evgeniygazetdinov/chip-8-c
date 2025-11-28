echo "Formatting C files..."
find . -name "*.c" -exec clang-format -i {} \;

echo "Formatting header files..."
find . -name "*.h" -exec clang-format -i {} \;

echo "Done!"
gcc -Iinclude -o hello_sdl src/main.c src/helpers.c src/core.c src/prepare.c -lSDL2 -lSDL2_ttf && ./hello_sdl

