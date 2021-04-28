#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;
uniform sampler2D textureSampler;
  
void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}