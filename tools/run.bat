cd ..
g++ -o build/test.exe src/main.c \
    lib/libglew32.dll.a lib/glfw3dll.a \
    -I lib/include -I src -I test \
    -L lib -L build \
    -lOpenGL32 -lglew32 -lglfw3 -lm
./build/test.exe
