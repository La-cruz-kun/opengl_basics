CFLAGS = -std=c99 -Wall -Werror 
SRC_FILES = ../include/stb.c ../include/shader_s.c

camera: camera_test.c
	clang camera_test.c $(SRC_FILES) $(CFLAGS) -o ../../opengl_projects/camera -lglad -lglfw -lGL -lm -lcglm
