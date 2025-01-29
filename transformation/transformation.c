#include <cglm/cglm.h>
#include <cglm/mat4.h>



int main(int argc, char *argv[])
{
    vec4 vec = {1.0f, 0.0f, 0.0f, 1.0f};
    vec4 result;
    mat4 trans = GLM_MAT4_IDENTITY_INIT;

    glm_translate(trans, (vec3) {1.0f, 1.0f, 0.0f});

    glm_mat4_mulv(trans, vec, result);

    printf("%f, %f, %f\n", result[0], result[1], result[2]);

    return EXIT_SUCCESS;
}

