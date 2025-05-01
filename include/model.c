#include "model.h"
#include <string.h>
#include <stdlib.h>

int parse_obj(FILE *file, Vertices *vertices, Indices *indices)
{
    if (!file) {
        printf("couldn't open obj file\n");
        return -1;
    }

    
    char buffer[512]; //buffer to hold each line of the file
    vertices->size = 0;
    indices->size = 0;
    // first iteration to get the number of vertex, faces etc
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "v ", 2) == 0) {
            vertices->size += 1;
        }
        if (strncmp(buffer, "f ", 2) == 0) {
            indices->size += 6;
        }
    }

    // allocating memory for the datas
    vertices->data = (Vertex *) malloc(sizeof(Vertex) * vertices->size);
    indices->data = (unsigned int *) malloc(sizeof(unsigned int) * indices->size);
    
    rewind(file);
    unsigned int total_vertex = 0;
    unsigned int total_indices = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "v ", 2) == 0) {
            int i = 0;
            float token = atof(strtok(buffer+2, " ")); // +2 to offset the string
            vertices->data[total_vertex].Vertices[i] = token;
            i++;
            for (; i < 3; i++) {
                token = atof(strtok(NULL, " "));
                vertices->data[total_vertex].Vertices[i] = token;
            }
            total_vertex++;
        }
        if (strncmp(buffer, "f ", 2) == 0) {
            int i = 0;
            char *tokens[4]; // a pointer to an array of 4 strings
                             // first of all get all the tokens in a line and store them in an array
            char *first_token = strtok(buffer+1, " ");
            tokens[i] = first_token;
            i++;
            for (; i < 4; i++) {
                first_token = strtok(NULL, " ");
                tokens[i] = first_token;
            }
            // then go through the array and split by / 
            for (int j = 0; j < 4; j++) {
                unsigned int second_token = atoi(strtok(tokens[j], "/"));
                indices->data[total_indices] = second_token - 1;
                for (int k = 0; k < 2; k++) {
                    second_token = atoi(strtok(NULL, "/"));
                }
                total_indices++;
            }
            indices->data[total_indices] = indices->data[total_indices - 1];
            indices->data[total_indices - 1] = indices->data[total_indices - 2];
            indices->data[total_indices + 1] = indices->data[total_indices - 4];
            total_indices += 2;
        }

    }
    /*for (int i = 0; i < vertices->size; i++) {
        printf("%f, %f, %f\n", vertices->data[i].Vertices[0], vertices->data[i].Vertices[1], vertices->data[i].Vertices[2]);
    }*/

    /*for (int i = 0; i < total_indices; i++) {
        printf("%u\n", indices->data[i]);
    }*/

    printf("%lu\n", indices->size);
    return 0;
}


