#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform float transparency;
uniform sampler2D ourTexture;

void main()
{
    vec4 color = texture(ourTexture, TexCoord);
    color.w = color.w - transparency;
    FragColor = color;
}