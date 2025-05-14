CC = clang
C_FLAGS = -std=c99 -Wall -Wextra -g
SRC_FILES = include/shader_s.c include/stb.c include/model.c
COLOR_FILES = color/color.c color/vertex.glsl color/fragment.glsl color/light_fragment.glsl color/light_vertex.glsl
MATERIAL_FILES = material/material.c material/vertex.glsl material/fragment.glsl material/light_fragment.glsl material/light_vertex.glsl
LIGHTING_MAPS_FILES = lighting_maps/lighting_maps.c lighting_maps/vertex.glsl lighting_maps/fragment.glsl lighting_maps/light_fragment.glsl lighting_maps/light_vertex.glsl
DIRECTIONAL_FILES = light_casters_directional/directional.c light_casters_directional/vertex.glsl light_casters_directional/fragment.glsl light_casters_directional/light_fragment.glsl light_casters_directional/light_vertex.glsl
POINT_FILES = light_casters_point/point.c light_casters_point/vertex.glsl light_casters_point/fragment.glsl light_casters_point/light_fragment.glsl light_casters_point/light_vertex.glsl
SPOT_FILES = light_casters_spot/spot.c light_casters_spot/vertex.glsl light_casters_spot/fragment.glsl light_casters_spot/light_fragment.glsl light_casters_spot/light_vertex.glsl
COORD_SYS_FILES = coordinate_system/coord.c coordinate_system/vertex.glsl coordinate_system/fragment.glsl 
MODEL_FILES = assimp_model_loading/model_loading.c assimp_model_loading/shader.vs assimp_model_loading/shader.fs 
color1: $(COLOR_FILES)
	$(CC) color/color.c $(SRC_FILES) $(C_FLAGS) -o color1 -lglad -lglfw -lGL -lm -lcglm

material1: $(MATERIAL_FILES)
	$(CC) material/material.c $(SRC_FILES) $(C_FLAGS) -o material1 -lglad -lglfw -lGL -lm -lcglm

lighting_maps1: $(LIGHTING_MAPS_FILES)
	$(CC) lighting_maps/lighting_maps.c $(SRC_FILES) $(C_FLAGS) -o lighting_maps1 -lglad -lglfw -lGL -lm -lcglm

directional: $(DIRECTIONAL_FILES)
	$(CC) light_casters_directional/directional.c $(SRC_FILES) $(C_FLAGS) -o directional -lglad -lglfw -lGL -lm -lcglm

point: $(POINT_FILES) 
	$(CC) light_casters_point/point.c $(SRC_FILES) $(C_FLAGS) -o point -lglad -lglfw -lGL -lm -lcglm

spot: $(SPOT_FILES) 
	$(CC) light_casters_spot/spot.c $(SRC_FILES) $(C_FLAGS) -o spot -lglad -lglfw -lGL -lm -lcglm

coord1: $(COORD_SYS_FILES)
	$(CC) coordinate_system/coord.c $(SRC_FILES) $(C_FLAGS) -o coord1 -lglad -lglfw -lGL -lm -lcglm

model: $(MODEL_FILES) $(SRC_FILES) 
	$(CC) assimp_model_loading/model_loading.c $(SRC_FILES) $(C_FLAGS) -o model -lglad -lglfw -lGL -lm -lcglm
