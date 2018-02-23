#version 330 core

const vec4 colorBlack = vec4(vec3(0.0f), 0.8f);

const float circleEdgeFactor = 0.93f;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform vec3 chessColor;
uniform vec3 ringColor;
uniform float radius;
uniform float ringRadiusInner;
uniform float ringRadiusOuter;
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

    float ringInnerEdge = ringRadiusInner * circleEdgeFactor;
	float ringOuterEdge = ringRadiusOuter * circleEdgeFactor;
	
    float borderEdge = radius * circleEdgeFactor;

    float radPermu = atan(fragTextureTopLeft.x - fragTextureTopLeft.y);
    mat2 rotation = GetRotationMatrix(1.5707963f + radPermu);
    vec2 myTextCoor = rotation * fragTextureCoor;

    color = mix(colorBlack, vec4(chessColor, 1.0f), texture(myTexture, myTextCoor).r);
	
	if(dist <= ringInnerEdge){
		// do nothing
	} else if(dist > ringInnerEdge && dist <= ringRadiusInner){
        color = mix(color, vec4(ringColor,1.0f), smoothstep(ringInnerEdge, ringRadiusInner, dist));
    } else if (dist > ringRadiusInner && dist <= ringOuterEdge){
		color = vec4(ringColor, 1.0f);
    } else if(dist > ringOuterEdge && dist <= ringRadiusOuter){
		color = mix(vec4(ringColor,1.0f), color, smoothstep(ringOuterEdge, ringRadiusOuter, dist));
	} else if(dist > borderEdge){
		color = mix(color, vec4(0.0), smoothstep(borderEdge, radius, dist));
	} 

}