#version 330 core
out vec4 outColor;  

in vec2 TexCoord;
in vec4 uColor;
uniform sampler2D textureSampler;
  
void main()
{
    outColor = uColor * texture(textureSampler, TexCoord);
}