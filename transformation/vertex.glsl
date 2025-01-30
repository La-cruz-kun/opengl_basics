#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 transform;
uniform mat4 scale;
void main()
{
    gl_Position = transform * scale * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
