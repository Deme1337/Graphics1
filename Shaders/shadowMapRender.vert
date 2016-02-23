#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;

smooth out vec2 vCoord;

void main()
{
	gl_Position = vec4(inPosition * 2.0 - 1.0, 1.0);
  vCoord = inCoord;
}