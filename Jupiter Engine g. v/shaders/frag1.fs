#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
flat in int isLn;

uniform float transparency;
uniform sampler2D ourTexture;

void main()
{
    vec4 color = texture(ourTexture, TexCoord);
    color.w = color.w - transparency;
    if(isLn == 1) {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        //gl_PointSize = 2;
    }
    else{FragColor = color;}
}