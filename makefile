CC = clang
C_FLAGS = -std=c99 -Wall -Wextra
SRC_FILES = include/shader_s.c include/stb.c 
COLOR_FILES = color/color.c color/vertex.glsl color/fragment.glsl color/light_fragment.glsl color/light_vertex.glsl
color1: $(COLOR_FILES)
	$(CC) color/color.c $(SRC_FILES) $(C_FLAGS) -o color1 -lglad -lglfw -lGL -lm -lcglm
