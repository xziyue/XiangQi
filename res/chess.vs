#version 330 core
layout (location = 0) in vec2 centerPos;
layout (location = 1) in vec2 inTextureTopLeft;

out VertexData{
    vec2 outCenterPos;
    vec2 textureTopLeftCoor;
} outData;

uniform float radius;
uniform mat4 projection;


void main(){
    outData.outCenterPos = centerPos;
    outData.textureTopLeftCoor = inTextureTopLeft;
    gl_Position = projection * vec4(centerPos, 0.0f, 1.0f);
}
