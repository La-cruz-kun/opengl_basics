CFLAGS = -std=c99 -Wall -Werror 
SRC_FILES = ../include/stb.c ../include/shader_s.c

coord: coord.c
	clang coord.c $(SRC_FILES) $(CFLAGS) -o ../../opengl_projects/coord -lglad -lglfw -lGL -lm -lcglm
