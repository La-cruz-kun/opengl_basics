CFLAGS = -std=c99 -Wall -Werror 
SRC_FILES = ../include/stb.c ../include/shader_s.c

transformation: transformation.c
	clang transformation.c $(SRC_FILES) $(CFLAGS) -o ../../opengl_projects/transformation -lglad -lglfw -lGL -lm -lcglm
