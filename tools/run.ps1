#!/bin/bash

cd ..
g++ -o build/demo.exe src/main.c \
    -I lib/include \
    -L lib -L build \
    -lOpenGL32 -lglew32 -lglfw3 -lm \
    -lglew32 -lglfw3 \
    lib/libglew32.dll.a lib/glfw3dll.a

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the executable..."
    
    # Run the executable
    ./build/demo.exe
else
    echo "Compilation failed."
fi
