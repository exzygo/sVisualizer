all:
	gcc main.c -o sv `sdl2-config --cflags --libs` -lSDL2_image
