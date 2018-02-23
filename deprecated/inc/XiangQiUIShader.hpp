// deprecated : shaders will be loaded from file
// shaders for XiangQiUI

#ifndef XIANGQI_XIANGQIUISHADER_HPP
#define XIANGQI_XIANGQIUISHADER_HPP

#include "GLDef.hpp"

const string chessBoardSquareVertexShaderSource{R"(
#version 330 core
layout (location = 0) in vec2 pointPos;
layout (location = 1) in vec2 texturePos;

uniform mat4 projection;
out vec2 outTexturePos;

void main(){
    gl_Position = projection * vec4(pointPos, 0.0f, 1.0f);
    outTexturePos = texturePos;
}

)"
};

const string chessBoardSquareFragmentShaderSource{R"(
#version 330 core

const vec4 colorBlack = vec4(0.0f, 0.0f, 0.0f, 1.0f);

in vec2 outTexturePos;
uniform vec3 chessBoardColor;
uniform sampler2D myTexture;

out vec4 color;

void main(){
    color = mix(colorBlack, vec4(chessBoardColor, 1.0f), texture(myTexture, outTexturePos).r);
}

)"
};

const string chessBoardLineVertexShaderSource{R"(
#version 330 core
layout (location = 0) in vec2 pointPos;
uniform mat4 projection;

void main(){
    gl_Position = projection * vec4(pointPos, 0.0f, 1.0f);
}
)"
};

/*
const string chessBoardLineGeometryShaderSource{R"(
#version 330 core

uniform mat4 projection;
uniform float pixelLineWidth;

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

void main(){
    // applying transformation to a vector
    vec4 tfLine = projection * vec4(pixelLineWidth, pixelLineWidth, 0.0, 0.0f);
    float lineWidthDecimal = max(abs(tfLine.x), abs(tfLine.y));
    //float lineWidth = float(ceil(lineWidthDecimal * 1000.0f)) / 1000.0f;
    float lineWidth = 0.009f;

    vec2 p1 = vec2(gl_in[0].gl_Position.xy);
    vec2 p2 = vec2(gl_in[1].gl_Position.xy);
    vec2 lineDir = p2 - p1;
    vec2 lineNormal;

    if (abs(lineDir[1]) < 0.001f){
        lineNormal = vec2(0.0f, 1.0f);
    } else {
        lineNormal = vec2(1.0f, -lineDir[0] / lineDir[1]);
        float lineNormalLength = sqrt(dot(lineNormal, lineNormal));
        lineNormal = lineNormal / lineNormalLength;
    }

    float lineSpan = lineWidth / 2.0f;

    gl_Position = vec4(p1 - lineSpan * lineNormal, 0.0f, 1.0f);
    EmitVertex();
    gl_Position = vec4(p1 + lineSpan * lineNormal, 0.0f, 1.0f);
    EmitVertex();
    gl_Position = vec4(p2 - lineSpan * lineNormal, 0.0f, 1.0f);
    EmitVertex();
    gl_Position = vec4(p2 + lineSpan * lineNormal, 0.0f, 1.0f);
    EmitVertex();

    EndPrimitive();
}

)"
};
*/

const string chessBoardLineFragmentShaderSource{R"(
#version 330 core

uniform vec3 lineColor;
out vec4 color;

void main(){
    color = vec4(lineColor, 1.0);
}
)"
};

const string chessVertexShaderSource{R"(
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

)"
};

const string chessGeometryShaderSource{R"(
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
)"
};

const string chessFragmentShaderSource{R"(
#version 330 core
#define circleEdgeFactor 0.97f

const vec4 colorBlack = vec4(vec3(0.0f), 1.0f);

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform vec3 chessColor;
uniform vec3 ringColor;
uniform float radius;
uniform float ringRadius;
uniform sampler2D myTexture;

in vec2 fragTextureTopLeft;
in vec2 fragTextureCoor;
in vec2 fragCenterPos;

out vec4 color;

mat2 GetRotationMatrix(float rad){
    mat2 matrix;
    matrix[0][0] = cos(rad);
    matrix[0][1] = -sin(rad);
    matrix[1][0] = sin(rad);
    matrix[1][1] = cos(rad);
    return matrix;
}

void main(){

    vec4 diff = (vec4(gl_FragCoord.xy, 0.0f, 1.0f) - vec4(fragCenterPos, 0.0, 1.0f));
    float dist = length(diff);

    float ringLowEdge = ringRadius * circleEdgeFactor;
    float lowEdge = radius * circleEdgeFactor;

    float radPermu = atan(abs(fragTextureTopLeft.x) - abs(fragTextureTopLeft.y)) / 7.0f;
    mat2 rotation = GetRotationMatrix(1.5707963f + radPermu);
    vec2 myTextCoor = rotation * fragTextureCoor;
    //vec2 myTextCoor = fragTextureCoor;

    color = mix(colorBlack, vec4(chessColor, 1.0f), texture(myTexture, myTextCoor).r);

    if(dist > ringLowEdge && dist < lowEdge){
        color = mix(color, vec4(ringColor,1.0f), smoothstep(ringLowEdge, ringRadius, dist));
    } else if (dist >= lowEdge){
        color = mix(vec4(ringColor, 1.0f), vec4(0.0f), smoothstep(lowEdge, radius, dist));
    }

}
)"
};


#endif //XIANGQI_XIANGQIUISHADER_HPP
