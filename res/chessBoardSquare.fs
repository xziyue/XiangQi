#version 330 core

const vec4 colorBlack = vec4(0.0f, 0.0f, 0.0f, 1.0f);

in vec2 outTexturePos;
uniform vec3 chessBoardColor;
uniform sampler2D myTexture;

out vec4 color;

void main(){
    color = mix(colorBlack, vec4(chessBoardColor, 1.0f), texture(myTexture, outTexturePos).r);
}