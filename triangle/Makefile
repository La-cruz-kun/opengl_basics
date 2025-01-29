CFLAGS = -std=c99 -Wall -Werror 

triangle: triangle.c
	clang triangle.c -o ../opengl_projects/triangle -lglad -lglfw -lGL

triangle2: triangle2.c
	gcc triangle2.c shader_s.c stb.c $(CFLAGS) -o ../opengl_projects/tri2 -lglad -lglfw -lGL -lm

foo: foo.c shader_s.h shader.fs shader.vs
	gcc foo.c shader_s.c -o foo -lglad
