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