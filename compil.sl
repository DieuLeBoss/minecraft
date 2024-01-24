echo "Compiling ..."
g++ $(find src/ -name '*.cpp') -o bin/progGL -Iinclude -lglfw -lGLEW -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl
echo "Compilation finish"
./bin/progGL
