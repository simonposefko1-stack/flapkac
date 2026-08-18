all:
	g++ flappy.cpp -o flappy.exe -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
	flappy.exe