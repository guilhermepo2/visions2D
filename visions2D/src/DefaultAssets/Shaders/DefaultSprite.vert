#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uCameraViewProjection;

uniform mat4 model;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
    vec4 pos = vec4(aPos, 0.0, 1.0);
    gl_Position = uCameraViewProjection * model * pos;
    // gl_Position = pos;
    TexCoord = aTexCoord;
}