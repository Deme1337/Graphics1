#version 330

out vec4 outputColor;

uniform sampler2D shadowMap;

smooth in vec2 vCoord;

void main()
{
	float depth = texture2D(shadowMap, vCoord).r;

	outputColor = vec4(depth, depth, depth, 1.0);
}
