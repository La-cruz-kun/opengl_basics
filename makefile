CC = clang
C_FLAGS = -std=c99 -Wall -Wextra
SRC_FILES = include/shader_s.c include/stb.c 
COLOR_FILES = color/color.c color/vertex.glsl color/fragment.glsl color/light_fragment.glsl color/light_vertex.glsl
MATERIAL_FILES = material/material.c material/vertex.glsl material/fragment.glsl material/light_fragment.glsl material/light_vertex.glsl

LIGHTING_MAPS_FILES = lighting_maps/lighting_maps.c lighting_maps/vertex.glsl lighting_maps/fragment.glsl lighting_maps/light_fragment.glsl lighting_maps/light_vertex.glsl

color1: $(COLOR_FILES)
	$(CC) color/color.c $(SRC_FILES) $(C_FLAGS) -o color1 -lglad -lglfw -lGL -lm -lcglm

material1: $(MATERIAL_FILES)
	$(CC) material/material.c $(SRC_FILES) $(C_FLAGS) -o material1 -lglad -lglfw -lGL -lm -lcglm

lighting_maps1: $(LIGHTING_MAPS_FILES)
	$(CC) lighting_maps/lighting_maps.c $(SRC_FILES) $(C_FLAGS) -o lighting_maps1 -lglad -lglfw -lGL -lm -lcglm
