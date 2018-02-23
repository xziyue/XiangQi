#version 330 core
layout (location = 0) in vec2 pointPos;
layout (location = 1) in vec2 texturePos;

uniform mat4 projection;
out vec2 outTexturePos;

void main(){
    gl_Position = projection * vec4(pointPos, 0.0f, 1.0f);
    outTexturePos = texturePos;
}
