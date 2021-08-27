#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aVertexColor;

uniform mat4 uCameraViewProjection;

out vec2 TexCoord;
out vec4 uColor;

void main()
{
    vec4 pos = vec4(aPos, 0.0, 1.0);
    gl_Position = uCameraViewProjection * pos;
    TexCoord = aTexCoord;
    uColor = aVertexColor;
}