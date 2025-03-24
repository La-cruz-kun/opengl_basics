CFLAGS = -std=c99 -Wall -Werror 
cc = clang
SRC_FILES = ../include/stb.c ../include/shader_s.c

triangle: triangle/triangle.c
	$(cc) triangle/triangle.c -o triangle -lglad -lglfw -lGL

triangle2: triangle/triangle2.c
	$(cc) triangle/triangle2.c $(SRC_FILES) $(CFLAGS) -o tri2 -lglad -lglfw -lGL -lm

foo: foo.c 
	$(cc) foo.c shader_s.c -o foo -lglad

transformation: transformation/transformation.c
	$(cc) transformation/transformation.c $(SRC_FILES) $(CFLAGS) -o transformation -lglad -lglfw -lGL -lm -lcglm

coord: coordinate_system/coord.c
	$(cc) coordinate_system/coord.c $(SRC_FILES) $(CFLAGS) -o coord -lglad -lglfw -lGL -lm -lcglm

camera: camera/camera_test.c
	$(cc) camera/camera_test.c $(SRC_FILES) $(CFLAGS) -o camera -lglad -lglfw -lGL -lm -lcglm

color: color/color
	$(cc) color/color.c $(SRC_FILES) $(CFLAGS) -o color -lglad -lglfw -lGL -lm -lcglm
