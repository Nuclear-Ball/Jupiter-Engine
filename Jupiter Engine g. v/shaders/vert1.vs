#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

//uniform int linesize;
uniform int isLine;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;
flat out int isLn;

void main()
{
    isLn = isLine;
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}