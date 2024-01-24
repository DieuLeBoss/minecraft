g++ -Wextra -O0 -ggdb $(find src/ -name '*.cpp') -o bin/progGL -Iinclude -lglfw -lGLEW -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl
gdb --args ./bin/progGL
