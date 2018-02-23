#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VertexData{
    vec2 outCenterPos;
    vec2 textureTopLeftCoor;
} inData[];

out vec2 fragCenterPos;
out vec2 fragTextureCoor;
out vec2 fragTextureTopLeft;

uniform mat4 projection;
uniform vec2 textureSpan;

uniform float radius;

void main(){
    vec2 actualPos = inData[0].outCenterPos.xy;
    fragCenterPos = actualPos;

    vec2 up = vec2(0.0f, radius);
    vec2 right = vec2(radius, 0.0f);

    vec2 textureCoor = inData[0].textureTopLeftCoor;
    fragTextureTopLeft = textureCoor;

    float scaleMutation = atan(abs(textureCoor.x) + abs(textureCoor.y));
    scaleMutation = min(1.2, scaleMutation);
    scaleMutation = max(0.6, scaleMutation);
    vec2 _textureSpan = textureSpan * scaleMutation;

    gl_Position = projection * vec4(actualPos + up - right, 0.0f, 1.0f);
    fragTextureCoor = textureCoor;
    EmitVertex();
    gl_Position = projection * vec4(actualPos + up + right, 0.0f, 1.0f);
    fragTextureCoor = textureCoor + vec2(_textureSpan.x, 0.0f);
    EmitVertex();
    gl_Position = projection * vec4(actualPos - up - right, 0.0f, 1.0f);
    fragTextureCoor = textureCoor + vec2(0.0f, _textureSpan.y);
    EmitVertex();
    gl_Position = projection * vec4(actualPos - up + right, 0.0f, 1.0f);
    fragTextureCoor = textureCoor + _textureSpan;
    EmitVertex();

    EndPrimitive();
}