#version 330 core
out vec4 outColor;  

in vec2 TexCoord;
uniform sampler2D textureSampler;
uniform vec4 uColor;
  
void main()
{
    outColor = uColor * vec4(1.0, 1.0, 1.0, 1.0);
}